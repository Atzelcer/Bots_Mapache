#include "MapacheHUD.h"
#include "MapachePlayer.h"
#include "MapacheW_Principal.h"
#include "MapacheW_Pause.h"
#include "MapacheW_GameOver.h"
#include "MapacheW_Creditos.h"
#include "MapacheW_Ajustes.h"
#include "MapacheW_EnJuego.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"
#include "Components/ProgressBar.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundWave.h"
#include "UObject/ConstructorHelpers.h"

AMapacheHUD::AMapacheHUD()
{
	PrincipalAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("PrincipalAudioComponent"));
	PrincipalAudioComponent->bAutoActivate = false;
	PrincipalAudioComponent->bIsUISound = true;
	PrincipalAudioComponent->bAllowSpatialization = false;

	static ConstructorHelpers::FObjectFinder<USoundWave> PrincipalMusicAsset(TEXT("/Game/Audio/UI/Friendship_Town_Theme"));
	if (PrincipalMusicAsset.Succeeded())
	{
		PrincipalMusic = PrincipalMusicAsset.Object;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> PrincipalBP(TEXT("/Game/HUD/WBP_Principal"));
	if (PrincipalBP.Succeeded())
	{
		PrincipalClass = PrincipalBP.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> PauseBP(TEXT("/Game/HUD/WBP_Pause"));
	if (PauseBP.Succeeded())
	{
		PauseClass = PauseBP.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> GameOverBP(TEXT("/Game/HUD/WBP_GameOver"));
	if (GameOverBP.Succeeded())
	{
		GameOverClass = GameOverBP.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> CreditosBP(TEXT("/Game/HUD/WBP_Creditos"));
	if (CreditosBP.Succeeded())
	{
		CreditosClass = CreditosBP.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> AjustesBP(TEXT("/Game/HUD/WBP_Ajustes"));
	if (AjustesBP.Succeeded())
	{
		AjustesClass = AjustesBP.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> EnJuegoBP(TEXT("/Game/HUD/WBP_EnJuego"));
	if (EnJuegoBP.Succeeded())
	{
		EnJuegoClass = EnJuegoBP.Class;
	}
}

void AMapacheHUD::BeginPlay()
{
	Super::BeginPlay();

	if (!PrincipalMusic)
	{
		PrincipalMusic = LoadObject<USoundWave>(nullptr, TEXT("/Game/Audio/UI/Friendship_Town_Theme.Friendship_Town_Theme"));
	}

	if (!PrincipalClass)
	{
		PrincipalClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/HUD/WBP_Principal.WBP_Principal_C"));
	}
	if (!PauseClass)
	{
		PauseClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/HUD/WBP_Pause.WBP_Pause_C"));
	}
	if (!GameOverClass)
	{
		GameOverClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/HUD/WBP_GameOver.WBP_GameOver_C"));
	}
	if (!CreditosClass)
	{
		CreditosClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/HUD/WBP_Creditos.WBP_Creditos_C"));
	}
	if (!AjustesClass)
	{
		AjustesClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/HUD/WBP_Ajustes.WBP_Ajustes_C"));
	}
	if (!EnJuegoClass)
	{
		EnJuegoClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/HUD/WBP_EnJuego.WBP_EnJuego_C"));
	}

	ShowPrincipal();
}

void AMapacheHUD::SetUIInput(UUserWidget* FocusWidget)
{
	APlayerController* PC = GetOwningPlayerController();
	if (!PC)
	{
		return;
	}

	PC->bShowMouseCursor = true;
	PC->bEnableClickEvents = true;
	PC->bEnableMouseOverEvents = true;

	FInputModeUIOnly InputMode;
	if (FocusWidget)
	{
		InputMode.SetWidgetToFocus(FocusWidget->TakeWidget());
	}
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PC->SetInputMode(InputMode);
}

void AMapacheHUD::SetGameAndUIInput(UUserWidget* FocusWidget)
{
	APlayerController* PC = GetOwningPlayerController();
	if (!PC)
	{
		return;
	}

	PC->bShowMouseCursor = true;
	PC->bEnableClickEvents = true;
	PC->bEnableMouseOverEvents = true;

	FInputModeGameAndUI InputMode;
	if (FocusWidget)
	{
		InputMode.SetWidgetToFocus(FocusWidget->TakeWidget());
	}
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PC->SetInputMode(InputMode);
}

void AMapacheHUD::SetGameInput()
{
	APlayerController* PC = GetOwningPlayerController();
	if (!PC)
	{
		return;
	}

	FInputModeGameOnly InputMode;
	PC->SetInputMode(InputMode);
	PC->bShowMouseCursor = false;
}

void AMapacheHUD::PlayPrincipalMusic()
{
	if (!PrincipalAudioComponent || !PrincipalMusic)
	{
		return;
	}

	if (PrincipalAudioComponent->IsPlaying())
	{
		return;
	}

	PrincipalAudioComponent->SetSound(PrincipalMusic);
	if (USoundWave* Wave = Cast<USoundWave>(PrincipalMusic))
	{
		Wave->bLooping = true;
	}
	PrincipalAudioComponent->Play();
}

void AMapacheHUD::StopPrincipalMusic()
{
	if (PrincipalAudioComponent && PrincipalAudioComponent->IsPlaying())
	{
		PrincipalAudioComponent->Stop();
	}
}

void AMapacheHUD::ShowPrincipal()
{
	if (!PrincipalClass)
	{
		UE_LOG(LogTemp, Error, TEXT("MapacheHUD: PrincipalClass is null"));
		return;
	}

	if (!PrincipalWidget)
	{
		PrincipalWidget = CreateWidget<UMapacheW_Principal>(GetWorld(), PrincipalClass);
	}

	if (!PrincipalWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("MapacheHUD: failed to create PrincipalWidget"));
		return;
	}

	if (!PrincipalWidget->IsInViewport())
	{
		PrincipalWidget->bIsFocusable = true;
		PrincipalWidget->AddToViewport(10);
		SetUIInput(PrincipalWidget);
		PlayPrincipalMusic();
	}
}

void AMapacheHUD::HidePrincipal()
{
	StopPrincipalMusic();

	if (PrincipalWidget)
	{
		PrincipalWidget->RemoveFromParent();
		PrincipalWidget = nullptr;
	}
}

void AMapacheHUD::ShowPause()
{
	if (bIsPaused)
	{
		return;
	}

	if (!PauseClass)
	{
		PauseClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/HUD/WBP_Pause.WBP_Pause_C"));
	}

	if (!PauseClass)
	{
		UE_LOG(LogTemp, Error, TEXT("MapacheHUD: PauseClass is null"));
		return;
	}

	if (!PauseWidget)
	{
		PauseWidget = CreateWidget<UMapacheW_Pause>(GetWorld(), PauseClass);
	}

	if (PauseWidget && !PauseWidget->IsInViewport())
	{
		PauseWidget->bIsFocusable = true;
		PauseWidget->AddToViewport(20);
		PauseWidget->SetVisibility(ESlateVisibility::Visible);
		SetUIInput(PauseWidget);
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		bIsPaused = true;
	}
}

void AMapacheHUD::HidePause()
{
	if (PauseWidget)
	{
		PauseWidget->RemoveFromParent();
		PauseWidget = nullptr;
	}

	UGameplayStatics::SetGamePaused(GetWorld(), false);
	bIsPaused = false;

	if (EnJuegoWidget)
	{
		SetGameAndUIInput(EnJuegoWidget);
	}
	else
	{
		SetGameInput();
	}
}

void AMapacheHUD::ShowGameOver()
{
}

void AMapacheHUD::HideGameOver()
{
}

void AMapacheHUD::ShowCreditos()
{
}

void AMapacheHUD::HideCreditos()
{
}

void AMapacheHUD::ShowAjustes()
{
	HidePrincipal();

	if (!AjustesClass)
	{
		return;
	}

	if (!AjustesWidget)
	{
		AjustesWidget = CreateWidget<UMapacheW_Ajustes>(GetWorld(), AjustesClass);
	}

	if (AjustesWidget && !AjustesWidget->IsInViewport())
	{
		AjustesWidget->AddToViewport(10);
		SetUIInput(AjustesWidget);
	}
}

void AMapacheHUD::HideAjustes()
{
	if (AjustesWidget)
	{
		AjustesWidget->RemoveFromParent();
		AjustesWidget = nullptr;
	}
}

void AMapacheHUD::ShowEnJuego()
{
	if (!EnJuegoClass)
	{
		EnJuegoClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/HUD/WBP_EnJuego.WBP_EnJuego_C"));
	}

	if (!EnJuegoClass)
	{
		UE_LOG(LogTemp, Error, TEXT("MapacheHUD: EnJuegoClass is null"));
		return;
	}

	if (!EnJuegoWidget)
	{
		EnJuegoWidget = CreateWidget<UMapacheW_EnJuego>(GetWorld(), EnJuegoClass);
	}

	if (!EnJuegoWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("MapacheHUD: failed to create EnJuegoWidget"));
		return;
	}

	if (!EnJuegoWidget->IsInViewport())
	{
		EnJuegoWidget->bIsFocusable = true;
		EnJuegoWidget->AddToViewport(5);
	}

	EnJuegoWidget->SetEnergia(1.f);

	if (APlayerController* PC = GetOwningPlayerController())
	{
		if (AMapachePlayer* Player = Cast<AMapachePlayer>(PC->GetPawn()))
		{
			Player->BindEnJuegoUI(EnJuegoWidget);
			Player->SetEnergia(1.f);
		}
	}

	SetGameAndUIInput(EnJuegoWidget);
}

void AMapacheHUD::HideEnJuego()
{
	if (APlayerController* PC = GetOwningPlayerController())
	{
		if (AMapachePlayer* Player = Cast<AMapachePlayer>(PC->GetPawn()))
		{
			Player->BindEnJuegoUI(nullptr);
		}
	}

	if (EnJuegoWidget)
	{
		EnJuegoWidget->RemoveFromParent();
		EnJuegoWidget = nullptr;
	}
}

void AMapacheHUD::SetEnergia(float Percent)
{
	if (EnJuegoWidget)
	{
		EnJuegoWidget->SetEnergia(Percent);
	}
}

UProgressBar* AMapacheHUD::GetBarEnergia() const
{
	return EnJuegoWidget ? EnJuegoWidget->GetBarEnergia() : nullptr;
}

void AMapacheHUD::StartNuevoJuego()
{
	HidePrincipal();
	HideAjustes();
	HidePause();
	ShowEnJuego();
}

void AMapacheHUD::ReturnToPrincipal()
{
	HidePause();
	HideEnJuego();
	HideAjustes();
	ShowPrincipal();
}
