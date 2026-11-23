#include "MapacheW_Creditos.h"
#include "MapacheHUD.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"

void UMapacheW_Creditos::NativeConstruct()
{
	Super::NativeConstruct();
	bIsFocusable = true;

	EnlazarPorNombre();
	LlenarCreditos();

	if (CajaCreditos)
	{
		CajaCreditos->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		CajaCreditos->SetClipping(EWidgetClipping::Inherit);
	}

	if (BtnSaltar)
	{
		BtnSaltar->OnClicked.AddDynamic(this, &UMapacheW_Creditos::OnSaltarClicked);
	}
}

void UMapacheW_Creditos::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!CajaCreditos)
	{
		return;
	}

	if (!bDesplazamientoListo)
	{
		FVector2D TamanoVista(1280.f, 720.f);
		if (GEngine && GEngine->GameViewport)
		{
			GEngine->GameViewport->GetViewportSize(TamanoVista);
		}

		AltoVista = FMath::Max(TamanoVista.Y, 720.f);
		CajaCreditos->ForceLayoutPrepass();
		PosY = AltoVista * 0.75f;
		bDesplazamientoListo = true;
	}

	PosY -= Velocidad * InDeltaTime;

	const float AltoCaja = FMath::Max(CajaCreditos->GetDesiredSize().Y, 400.f);
	if (PosY < -(AltoCaja + AltoVista * 0.25f))
	{
		PosY = AltoVista * 0.75f;
	}

	CajaCreditos->SetRenderTranslation(FVector2D(0.f, PosY));
}

void UMapacheW_Creditos::EnlazarPorNombre()
{
	if (!WidgetTree)
	{
		return;
	}

	TArray<UWidget*> Widgets;
	WidgetTree->GetAllWidgets(Widgets);
	for (UWidget* Widget : Widgets)
	{
		if (UImage* Imagen = Cast<UImage>(Widget))
		{
			Imagen->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			continue;
		}

		if (UTextBlock* Texto = Cast<UTextBlock>(Widget))
		{
			Texto->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else if (UButton* Boton = Cast<UButton>(Widget))
		{
			if (!BtnSaltar && Boton->GetName().ToLower().Contains(TEXT("saltar")))
			{
				BtnSaltar = Boton;
			}
		}
		else if (UVerticalBox* Caja = Cast<UVerticalBox>(Widget))
		{
			const FString Nombre = Caja->GetName().ToLower();
			if (!CajaCreditos && (Nombre.Contains(TEXT("credito")) || Nombre.Contains(TEXT("caja"))))
			{
				CajaCreditos = Caja;
			}
		}
	}
}

void UMapacheW_Creditos::EstilizarLinea(UTextBlock* Linea, int32 Tamano, const FLinearColor& Color) const
{
	if (!Linea)
	{
		return;
	}

	FSlateFontInfo Fuente = Linea->Font;
	Fuente.Size = Tamano;
	Fuente.OutlineSettings.OutlineSize = 2;
	Fuente.OutlineSettings.OutlineColor = FLinearColor::Black;
	Linea->SetFont(Fuente);
	Linea->SetColorAndOpacity(FSlateColor(Color));
	Linea->SetJustification(ETextJustify::Center);
	Linea->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

UTextBlock* UMapacheW_Creditos::AgregarLinea(const FString& Texto, int32 Tamano, const FLinearColor& Color, float PaddingArriba, float PaddingAbajo)
{
	if (!CajaCreditos || !WidgetTree)
	{
		return nullptr;
	}

	UTextBlock* Linea = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	if (!Linea)
	{
		return nullptr;
	}

	Linea->SetText(FText::FromString(Texto));
	EstilizarLinea(Linea, Tamano, Color);

	if (UVerticalBoxSlot* Hueco = CajaCreditos->AddChildToVerticalBox(Linea))
	{
		Hueco->SetHorizontalAlignment(HAlign_Center);
		Hueco->SetPadding(FMargin(0.f, PaddingArriba, 0.f, PaddingAbajo));
	}

	return Linea;
}

void UMapacheW_Creditos::LlenarCreditos()
{
	const FLinearColor Dorado(1.f, 0.82f, 0.35f);
	const FLinearColor Blanco(1.f, 1.f, 1.f);
	const FLinearColor Suave(0.86f, 0.86f, 0.86f);

	if (TxtTitulo)
	{
		TxtTitulo->SetText(FText::FromString(TEXT("MIDNIGHT RACCON")));
		EstilizarLinea(TxtTitulo, 52, Blanco);
	}

	bool bTituloDentro = false;
	if (TxtTitulo && CajaCreditos)
	{
		for (UWidget* Padre = TxtTitulo->GetParent(); Padre; Padre = Padre->GetParent())
		{
			if (Padre == CajaCreditos)
			{
				bTituloDentro = true;
				break;
			}
		}
	}

	if (TxtTitulo && !bTituloDentro)
	{
		TxtTitulo->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (!bTituloDentro)
	{
		AgregarLinea(TEXT("MIDNIGHT RACCON"), 52, Blanco, 0.f, 8.f);
	}
	else
	{
		AgregarLinea(TEXT(""), 16, Blanco, 0.f, 8.f);
	}

	AgregarLinea(TEXT("LA NOCHE EN QUE SE LOS LLEVARON"), 20, Suave, 0.f, 28.f);

	AgregarLinea(TEXT("DESARROLLADORES"), 22, Dorado, 8.f, 8.f);
	AgregarLinea(TEXT("Atzel Cervantes"), 34, Blanco, 4.f, 2.f);
	AgregarLinea(TEXT("Elmer Vela"), 34, Blanco, 4.f, 2.f);
	AgregarLinea(TEXT("Brayan Polo"), 34, Blanco, 4.f, 16.f);

	AgregarLinea(TEXT("DISEÑO DE PANELES"), 22, Dorado, 18.f, 2.f);
	AgregarLinea(TEXT("Lidia Choque"), 34, Blanco, 0.f, 2.f);
	AgregarLinea(TEXT("Camila"), 34, Blanco, 0.f, 16.f);

	AgregarLinea(TEXT("ARTE"), 22, Dorado, 18.f, 2.f);
	AgregarLinea(TEXT("Lidia Choque"), 34, Blanco, 0.f, 2.f);
	AgregarLinea(TEXT("Camila"), 34, Blanco, 0.f, 16.f);

	AgregarLinea(TEXT("MÚSICA"), 22, Dorado, 18.f, 2.f);
	AgregarLinea(TEXT("Lidia Choque"), 34, Blanco, 0.f, 2.f);
	AgregarLinea(TEXT("Camila"), 34, Blanco, 0.f, 16.f);

	AgregarLinea(TEXT("HECHO EN"), 22, Dorado, 28.f, 8.f);
	AgregarLinea(TEXT("C++"), 36, Blanco, 4.f, 2.f);
	AgregarLinea(TEXT("Unreal Engine 4"), 32, Blanco, 4.f, 2.f);
	AgregarLinea(TEXT("Unreal Motion Graphics"), 26, Blanco, 4.f, 2.f);
	AgregarLinea(TEXT("Paper2D"), 26, Blanco, 4.f, 16.f);

	AgregarLinea(TEXT("AGRADECIMIENTOS"), 22, Dorado, 28.f, 8.f);
	AgregarLinea(TEXT("A la familia que espera en la noche"), 24, Blanco, 6.f, 4.f);
	AgregarLinea(TEXT("A quien dejó una luz encendida"), 24, Blanco, 6.f, 4.f);
	AgregarLinea(TEXT("A los mapaches que no salieron en cámara"), 24, Blanco, 6.f, 4.f);
	AgregarLinea(TEXT("A la ciudad que no duerme"), 24, Blanco, 6.f, 16.f);

	AgregarLinea(TEXT("2026"), 20, Suave, 24.f, 8.f);
	AgregarLinea(TEXT("GRACIAS POR JUGAR"), 28, Dorado, 8.f, 0.f);
}

void UMapacheW_Creditos::OnSaltarClicked()
{
	Saltar();
}

void UMapacheW_Creditos::Saltar()
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		return;
	}

	if (AMapacheHUD* HUD = Cast<AMapacheHUD>(PC->GetHUD()))
	{
		HUD->CerrarCreditos();
	}
}
