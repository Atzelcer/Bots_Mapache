#include "MapacheW_Ajustes.h"
#include "MapacheHUD.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Engine/Engine.h"
#include "Engine/Texture2D.h"
#include "GameFramework/PlayerController.h"
#include "AudioDevice.h"
#include "Kismet/GameplayStatics.h"

namespace
{
	constexpr float PasoAjuste = 0.1f;
}

void UMapacheW_Ajustes::NativeConstruct()
{
	Super::NativeConstruct();
	bIsFocusable = true;

	CargarAssetsPorDefecto();
	EnlazarPorNombre();
	CargarAjustesGuardados();

	if (BtnMute)
	{
		BtnMute->OnClicked.AddDynamic(this, &UMapacheW_Ajustes::OnMuteClicked);
	}

	if (BtnVolver)
	{
		BtnVolver->OnClicked.AddDynamic(this, &UMapacheW_Ajustes::OnVolverClicked);
	}

	if (SliderBrillo)
	{
		SliderBrillo->SetMinValue(0.f);
		SliderBrillo->SetMaxValue(1.f);
		SliderBrillo->SetValue(Brillo);
		SliderBrillo->OnValueChanged.AddDynamic(this, &UMapacheW_Ajustes::OnBrilloChanged);
	}

	if (SliderSonido)
	{
		SliderSonido->SetMinValue(0.f);
		SliderSonido->SetMaxValue(1.f);
		SliderSonido->SetValue(bMuteado ? 0.f : Volumen);
		SliderSonido->OnValueChanged.AddDynamic(this, &UMapacheW_Ajustes::OnSonidoChanged);
	}

	if (BtnBrilloMenos)
	{
		BtnBrilloMenos->OnClicked.AddDynamic(this, &UMapacheW_Ajustes::OnBrilloMenos);
	}
	if (BtnBrilloMas)
	{
		BtnBrilloMas->OnClicked.AddDynamic(this, &UMapacheW_Ajustes::OnBrilloMas);
	}
	if (BtnSonidoMenos)
	{
		BtnSonidoMenos->OnClicked.AddDynamic(this, &UMapacheW_Ajustes::OnSonidoMenos);
	}
	if (BtnSonidoMas)
	{
		BtnSonidoMas->OnClicked.AddDynamic(this, &UMapacheW_Ajustes::OnSonidoMas);
	}

	if (BarBrillo)
	{
		BarBrillo->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	if (BarSonido)
	{
		BarSonido->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	ActualizarBarras();
	ActualizarBotonMute();
	AplicarVolumen();
	AplicarBrillo();
}

void UMapacheW_Ajustes::EnlazarPorNombre()
{
	if (!WidgetTree)
	{
		return;
	}

	TArray<UWidget*> Todos;
	WidgetTree->GetAllWidgets(Todos);

	TArray<UProgressBar*> BarrasSueltas;
	for (UWidget* Widget : Todos)
	{
		if (UImage* Imagen = Cast<UImage>(Widget))
		{
			Imagen->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			continue;
		}

		if (UTextBlock* Texto = Cast<UTextBlock>(Widget))
		{
			Texto->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			continue;
		}

		if (UButton* Boton = Cast<UButton>(Widget))
		{
			const FString Nombre = Boton->GetName().ToLower();
			if (!BtnVolver && (Nombre.Contains(TEXT("atras")) || Nombre.Contains(TEXT("volver")) || Nombre.Contains(TEXT("back"))))
			{
				BtnVolver = Boton;
			}
			else if (!BtnMute && Nombre.Contains(TEXT("mute")))
			{
				BtnMute = Boton;
			}
			continue;
		}

		if (UProgressBar* Barra = Cast<UProgressBar>(Widget))
		{
			const FString Nombre = Barra->GetName().ToLower();
			if (!BarSonido && (Nombre.Contains(TEXT("sonido")) || Nombre.Contains(TEXT("volumen")) || Nombre.Contains(TEXT("audio"))))
			{
				BarSonido = Barra;
			}
			else if (!BarBrillo && Nombre.Contains(TEXT("brillo")))
			{
				BarBrillo = Barra;
			}
			else
			{
				BarrasSueltas.Add(Barra);
			}
		}
	}

	if (!BarSonido && BarrasSueltas.Num() > 0)
	{
		BarSonido = BarrasSueltas[0];
	}
	if (!BarBrillo && BarrasSueltas.Num() > 1)
	{
		BarBrillo = BarrasSueltas[1];
	}

	SetVisibility(ESlateVisibility::Visible);
}

void UMapacheW_Ajustes::CargarAjustesGuardados()
{
	APlayerController* PC = GetOwningPlayer();
	AMapacheHUD* HUD = PC ? Cast<AMapacheHUD>(PC->GetHUD()) : nullptr;
	if (!HUD || !HUD->TieneAjustesGuardados())
	{
		return;
	}

	Volumen = HUD->GetVolumenGuardado();
	Brillo = HUD->GetBrilloGuardado();
	bMuteado = HUD->GetMuteGuardado();
	VolumenAntesDeMute = Volumen > 0.f ? Volumen : 1.f;
}

void UMapacheW_Ajustes::GuardarAjustesActuales()
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		return;
	}

	if (AMapacheHUD* HUD = Cast<AMapacheHUD>(PC->GetHUD()))
	{
		HUD->GuardarAjustes(Volumen, Brillo, bMuteado);
	}
}

void UMapacheW_Ajustes::CargarAssetsPorDefecto()
{
	if (!TexMutearNOR)
	{
		TexMutearNOR = LoadObject<UTexture2D>(nullptr, TEXT("/Game/FULLBUTTON/Mutear/MUTEAR_NOR.MUTEAR_NOR"));
	}
	if (!TexMutearHOV)
	{
		TexMutearHOV = LoadObject<UTexture2D>(nullptr, TEXT("/Game/FULLBUTTON/Mutear/MUTEAR_HOV.MUTEAR_HOV"));
	}
	if (!TexMutearCLI)
	{
		TexMutearCLI = LoadObject<UTexture2D>(nullptr, TEXT("/Game/FULLBUTTON/Mutear/MUTEAR_CLI.MUTEAR_CLI"));
	}
	if (!TexDesmutearNOR)
	{
		TexDesmutearNOR = LoadObject<UTexture2D>(nullptr, TEXT("/Game/FULLBUTTON/Desmutear/DESMUTEAR_NOR.DESMUTEAR_NOR"));
	}
	if (!TexDesmutearHOV)
	{
		TexDesmutearHOV = LoadObject<UTexture2D>(nullptr, TEXT("/Game/FULLBUTTON/Desmutear/DESMUTEAR_HOV.DESMUTEAR_HOV"));
	}
	if (!TexDesmutearCLI)
	{
		TexDesmutearCLI = LoadObject<UTexture2D>(nullptr, TEXT("/Game/FULLBUTTON/Desmutear/DESMUTEAR_CLI.DESMUTEAR_CLI"));
	}
}

void UMapacheW_Ajustes::SetMuteado(bool bNuevoMute)
{
	if (bNuevoMute && !bMuteado)
	{
		VolumenAntesDeMute = Volumen;
	}

	bMuteado = bNuevoMute;

	if (bMuteado)
	{
		AplicarVolumen();
	}
	else
	{
		SetVolumen(VolumenAntesDeMute > 0.f ? VolumenAntesDeMute : 1.f);
	}

	ActualizarBotonMute();
	GuardarAjustesActuales();
}

void UMapacheW_Ajustes::SetVolumen(float NuevoVolumen)
{
	Volumen = FMath::Clamp(NuevoVolumen, 0.f, 1.f);

	if (Volumen > 0.f)
	{
		bMuteado = false;
	}

	if (SliderSonido && !FMath::IsNearlyEqual(SliderSonido->GetValue(), Volumen))
	{
		SliderSonido->SetValue(Volumen);
	}

	ActualizarBarras();
	ActualizarBotonMute();
	AplicarVolumen();
	GuardarAjustesActuales();
}

void UMapacheW_Ajustes::SetBrillo(float NuevoBrillo)
{
	Brillo = FMath::Clamp(NuevoBrillo, 0.f, 1.f);

	if (SliderBrillo && !FMath::IsNearlyEqual(SliderBrillo->GetValue(), Brillo))
	{
		SliderBrillo->SetValue(Brillo);
	}

	ActualizarBarras();
	AplicarBrillo();
	GuardarAjustesActuales();
}

void UMapacheW_Ajustes::AplicarVolumen()
{
	const float VolumenFinal = bMuteado ? 0.f : Volumen;

	if (GEngine)
	{
		if (FAudioDeviceHandle AudioDevice = GEngine->GetMainAudioDevice())
		{
			AudioDevice->SetTransientMasterVolume(VolumenFinal);
		}
	}
}

void UMapacheW_Ajustes::AplicarBrillo()
{
	if (GEngine)
	{
		GEngine->DisplayGamma = FMath::Lerp(1.4f, 3.2f, Brillo);
	}
}

void UMapacheW_Ajustes::ActualizarBarras()
{
	if (BarBrillo)
	{
		BarBrillo->SetPercent(Brillo);
	}

	if (BarSonido)
	{
		BarSonido->SetPercent(bMuteado ? 0.f : Volumen);
	}
}

void UMapacheW_Ajustes::AplicarEstiloBotonMute(UTexture2D* NOR, UTexture2D* HOV, UTexture2D* CLI)
{
	if (!BtnMute)
	{
		return;
	}

	FButtonStyle Style = BtnMute->WidgetStyle;

	if (NOR)
	{
		Style.Normal.SetResourceObject(NOR);
		Style.Normal.DrawAs = ESlateBrushDrawType::Image;
		Style.Normal.ImageSize = FVector2D(NOR->GetSizeX(), NOR->GetSizeY());
	}
	if (HOV)
	{
		Style.Hovered.SetResourceObject(HOV);
		Style.Hovered.DrawAs = ESlateBrushDrawType::Image;
		Style.Hovered.ImageSize = FVector2D(HOV->GetSizeX(), HOV->GetSizeY());
	}
	if (CLI)
	{
		Style.Pressed.SetResourceObject(CLI);
		Style.Pressed.DrawAs = ESlateBrushDrawType::Image;
		Style.Pressed.ImageSize = FVector2D(CLI->GetSizeX(), CLI->GetSizeY());
	}

	BtnMute->SetStyle(Style);
}

void UMapacheW_Ajustes::ActualizarBotonMute()
{
	if (bMuteado)
	{
		AplicarEstiloBotonMute(TexDesmutearNOR, TexDesmutearHOV, TexDesmutearCLI);
	}
	else
	{
		AplicarEstiloBotonMute(TexMutearNOR, TexMutearHOV, TexMutearCLI);
	}
}

void UMapacheW_Ajustes::OnMuteClicked()
{
	SetMuteado(!bMuteado);
}

void UMapacheW_Ajustes::OnVolverClicked()
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		return;
	}

	AMapacheHUD* HUD = Cast<AMapacheHUD>(PC->GetHUD());
	if (HUD)
	{
		HUD->CerrarAjustes();
	}
}

void UMapacheW_Ajustes::OnBrilloChanged(float Value)
{
	SetBrillo(Value);
}

void UMapacheW_Ajustes::OnSonidoChanged(float Value)
{
	SetVolumen(Value);
}

void UMapacheW_Ajustes::OnBrilloMenos()
{
	SetBrillo(Brillo - PasoAjuste);
}

void UMapacheW_Ajustes::OnBrilloMas()
{
	SetBrillo(Brillo + PasoAjuste);
}

void UMapacheW_Ajustes::OnSonidoMenos()
{
	SetVolumen(Volumen - PasoAjuste);
}

void UMapacheW_Ajustes::OnSonidoMas()
{
	SetVolumen(Volumen + PasoAjuste);
}

bool UMapacheW_Ajustes::AplicarDesdeCursor(UProgressBar* Barra, const FVector2D& ScreenPos, bool bEsBrillo)
{
	if (!Barra)
	{
		return false;
	}

	const FGeometry& Geo = Barra->GetCachedGeometry();
	if (!Geo.IsUnderLocation(ScreenPos))
	{
		return false;
	}

	const float Ancho = Geo.GetLocalSize().X;
	if (Ancho <= 1.f)
	{
		return false;
	}

	const float Percent = FMath::Clamp(Geo.AbsoluteToLocal(ScreenPos).X / Ancho, 0.f, 1.f);
	if (bEsBrillo)
	{
		SetBrillo(Percent);
	}
	else
	{
		SetVolumen(Percent);
	}

	return true;
}

FReply UMapacheW_Ajustes::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	const FVector2D Pos = InMouseEvent.GetScreenSpacePosition();

	if (AplicarDesdeCursor(BarSonido, Pos, false))
	{
		bArrastrandoSonido = true;
		bArrastrandoBrillo = false;
		return FReply::Handled().CaptureMouse(TakeWidget());
	}

	if (AplicarDesdeCursor(BarBrillo, Pos, true))
	{
		bArrastrandoBrillo = true;
		bArrastrandoSonido = false;
		return FReply::Handled().CaptureMouse(TakeWidget());
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UMapacheW_Ajustes::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!bArrastrandoSonido && !bArrastrandoBrillo)
	{
		return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
	}

	const FVector2D Pos = InMouseEvent.GetScreenSpacePosition();
	UProgressBar* Barra = bArrastrandoBrillo ? BarBrillo : BarSonido;
	if (Barra)
	{
		const FGeometry& Geo = Barra->GetCachedGeometry();
		const float Ancho = Geo.GetLocalSize().X;
		if (Ancho > 1.f)
		{
			const float Percent = FMath::Clamp(Geo.AbsoluteToLocal(Pos).X / Ancho, 0.f, 1.f);
			if (bArrastrandoBrillo)
			{
				SetBrillo(Percent);
			}
			else
			{
				SetVolumen(Percent);
			}
		}
	}

	return FReply::Handled();
}

FReply UMapacheW_Ajustes::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!bArrastrandoSonido && !bArrastrandoBrillo)
	{
		return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	}

	SoltarArrastre();
	return FReply::Handled().ReleaseMouseCapture();
}

void UMapacheW_Ajustes::SoltarArrastre()
{
	bArrastrandoSonido = false;
	bArrastrandoBrillo = false;
}
