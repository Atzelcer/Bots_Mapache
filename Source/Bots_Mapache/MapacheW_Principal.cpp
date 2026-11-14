#include "MapacheW_Principal.h"
#include "MapacheHUD.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"

void UMapacheW_Principal::NativeConstruct()
{
	Super::NativeConstruct();

	if (BtnNuevoJuego)
	{
		BtnNuevoJuego->OnClicked.AddDynamic(this, &UMapacheW_Principal::OnNuevoJuegoClicked);
	}

	if (BtnAjustes)
	{
		BtnAjustes->OnClicked.AddDynamic(this, &UMapacheW_Principal::OnAjustesClicked);
	}

	if (BtnSalir)
	{
		BtnSalir->OnClicked.AddDynamic(this, &UMapacheW_Principal::OnSalirClicked);
	}
}

void UMapacheW_Principal::OnNuevoJuegoClicked()
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		return;
	}

	AMapacheHUD* HUD = Cast<AMapacheHUD>(PC->GetHUD());
	if (HUD)
	{
		HUD->StartNuevoJuego();
	}
}

void UMapacheW_Principal::OnAjustesClicked()
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		return;
	}

	AMapacheHUD* HUD = Cast<AMapacheHUD>(PC->GetHUD());
	if (HUD)
	{
		HUD->ShowAjustes();
	}
}

void UMapacheW_Principal::OnSalirClicked()
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		return;
	}

	UKismetSystemLibrary::QuitGame(GetWorld(), PC, EQuitPreference::Quit, true);
}
