#include "MapacheW_Pause.h"
#include "MapacheHUD.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"

void UMapacheW_Pause::NativeConstruct()
{
	Super::NativeConstruct();
	bIsFocusable = true;

	if (BtnContinuar)
	{
		BtnContinuar->OnClicked.AddDynamic(this, &UMapacheW_Pause::OnContinuarClicked);
	}

	if (BtnAjustes)
	{
		BtnAjustes->OnClicked.AddDynamic(this, &UMapacheW_Pause::OnAjustesClicked);
	}

	if (BtnSalir)
	{
		BtnSalir->OnClicked.AddDynamic(this, &UMapacheW_Pause::OnSalirClicked);
	}
}

void UMapacheW_Pause::OnContinuarClicked()
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		return;
	}

	AMapacheHUD* HUD = Cast<AMapacheHUD>(PC->GetHUD());
	if (HUD)
	{
		HUD->HidePause();
	}
}

void UMapacheW_Pause::OnAjustesClicked()
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		return;
	}

	AMapacheHUD* HUD = Cast<AMapacheHUD>(PC->GetHUD());
	if (HUD)
	{
		HUD->HidePause();
		HUD->ShowAjustes();
	}
}

void UMapacheW_Pause::OnSalirClicked()
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		return;
	}

	AMapacheHUD* HUD = Cast<AMapacheHUD>(PC->GetHUD());
	if (HUD)
	{
		HUD->ReturnToPrincipal();
		return;
	}

	UKismetSystemLibrary::QuitGame(GetWorld(), PC, EQuitPreference::Quit, true);
}
