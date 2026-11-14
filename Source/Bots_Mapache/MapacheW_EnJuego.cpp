#include "MapacheW_EnJuego.h"
#include "MapacheHUD.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "GameFramework/PlayerController.h"

void UMapacheW_EnJuego::NativeConstruct()
{
	Super::NativeConstruct();

	if (BtnPausa)
	{
		BtnPausa->OnClicked.AddDynamic(this, &UMapacheW_EnJuego::OnPausaClicked);
	}

	SetEnergia(1.f);
}

void UMapacheW_EnJuego::SetEnergia(float Percent)
{
	if (BarEnergia)
	{
		BarEnergia->SetPercent(FMath::Clamp(Percent, 0.f, 1.f));
	}
}

void UMapacheW_EnJuego::OnPausaClicked()
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		return;
	}

	AMapacheHUD* HUD = Cast<AMapacheHUD>(PC->GetHUD());
	if (HUD)
	{
		HUD->ShowPause();
	}
}
