#include "MapacheW_Historia.h"
#include "MapacheHUD.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"

void UMapacheW_Historia::NativeConstruct()
{
	Super::NativeConstruct();
	bIsFocusable = true;

	if (BtnSiguiente)
	{
		BtnSiguiente->OnClicked.AddDynamic(this, &UMapacheW_Historia::OnSiguienteClicked);
	}

	if (BtnAnterior)
	{
		BtnAnterior->OnClicked.AddDynamic(this, &UMapacheW_Historia::OnAnteriorClicked);
	}

	if (BtnSaltar)
	{
		BtnSaltar->OnClicked.AddDynamic(this, &UMapacheW_Historia::OnSaltarClicked);
	}

	if (Diapositivas.Num() == 0)
	{
		CargarDiapositivasPorDefecto();
	}

	ReiniciarHistoria();
}

void UMapacheW_Historia::CargarDiapositivasPorDefecto()
{
	static const TCHAR* Paths[] = {
		TEXT("/Game/UI/Historia/Historia_01.Historia_01"),
		TEXT("/Game/UI/Historia/Historia_02.Historia_02"),
		TEXT("/Game/UI/Historia/Historia_03.Historia_03"),
		TEXT("/Game/UI/Historia/Historia_04.Historia_04"),
		TEXT("/Game/UI/Historia/Historia_05.Historia_05"),
		TEXT("/Game/UI/Historia/Historia_06.Historia_06"),
		TEXT("/Game/UI/Historia/Historia_07.Historia_07")
	};

	Diapositivas.Reset();
	for (const TCHAR* Path : Paths)
	{
		Diapositivas.Add(TSoftObjectPtr<UTexture2D>(FSoftObjectPath(Path)));
	}
}

void UMapacheW_Historia::ReiniciarHistoria()
{
	MostrarDiapositiva(0);
}

void UMapacheW_Historia::Siguiente()
{
	if (IndiceActual >= Diapositivas.Num() - 1)
	{
		FinalizarHistoria();
		return;
	}

	MostrarDiapositiva(IndiceActual + 1);
}

void UMapacheW_Historia::Anterior()
{
	if (IndiceActual <= 0)
	{
		return;
	}

	MostrarDiapositiva(IndiceActual - 1);
}

void UMapacheW_Historia::Saltar()
{
	FinalizarHistoria();
}

void UMapacheW_Historia::MostrarDiapositiva(int32 Indice)
{
	if (Diapositivas.Num() == 0)
	{
		return;
	}

	IndiceActual = FMath::Clamp(Indice, 0, Diapositivas.Num() - 1);

	if (!ImgHistoria)
	{
		return;
	}

	UTexture2D* Texture = Diapositivas[IndiceActual].LoadSynchronous();
	if (Texture)
	{
		ImgHistoria->SetBrushFromTexture(Texture, true);
	}

	if (BtnAnterior)
	{
		BtnAnterior->SetIsEnabled(IndiceActual > 0);
	}
}

void UMapacheW_Historia::FinalizarHistoria()
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		return;
	}

	AMapacheHUD* HUD = Cast<AMapacheHUD>(PC->GetHUD());
	if (HUD)
	{
		HUD->HideHistoria();
		HUD->ShowEnJuego();
	}
}

void UMapacheW_Historia::OnSiguienteClicked()
{
	Siguiente();
}

void UMapacheW_Historia::OnAnteriorClicked()
{
	Anterior();
}

void UMapacheW_Historia::OnSaltarClicked()
{
	Saltar();
}
