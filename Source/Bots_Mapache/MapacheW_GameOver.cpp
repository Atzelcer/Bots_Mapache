#include "MapacheW_GameOver.h"
#include "MapacheHUD.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"

void UMapacheW_GameOver::NativeConstruct()
{
	Super::NativeConstruct();
	bIsFocusable = true;

	if (BtnContinuarGO)
	{
		BtnContinuarGO->OnClicked.AddDynamic(this, &UMapacheW_GameOver::OnContinuarClicked);
	}

	if (BtnSalirGO)
	{
		BtnSalirGO->OnClicked.AddDynamic(this, &UMapacheW_GameOver::OnSalirClicked);
	}
}

void UMapacheW_GameOver::OnContinuarClicked()
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		return;
	}

	AMapacheHUD* HUD = Cast<AMapacheHUD>(PC->GetHUD());
	if (HUD)
	{
		HUD->HideGameOver();
		HUD->ShowEnJuego();
	}
}

void UMapacheW_GameOver::OnSalirClicked()
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
	}
}
