#include "MapacheHUD.h"
#include "MapachePlayer.h"
#include "MapacheW_Principal.h"
#include "MapacheW_Pause.h"
#include "MapacheW_GameOver.h"
#include "MapacheW_Creditos.h"
#include "MapacheW_Ajustes.h"
#include "MapacheW_EnJuego.h"
#include "MapacheW_Historia.h"
#include "MapacheW_Carga.h"
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

	static ConstructorHelpers::FClassFinder<UUserWidget> HistoriaBP(TEXT("/Game/HUD/WBP_Historia"));
	if (HistoriaBP.Succeeded())
	{
		HistoriaClass = HistoriaBP.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> CargaBP(TEXT("/Game/HUD/WBP_Carga"));
	if (CargaBP.Succeeded())
	{
		CargaClass = CargaBP.Class;
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
	if (!HistoriaClass)
	{
		HistoriaClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/HUD/WBP_Historia.WBP_Historia_C"));
	}
	if (!CargaClass)
	{
		CargaClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/HUD/WBP_Carga.WBP_Carga_C"));
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

void AMapacheHUD::PausarMusicaPrincipal()
{
	if (PrincipalAudioComponent && PrincipalAudioComponent->IsPlaying())
	{
		PrincipalAudioComponent->SetPaused(true);
	}
}

void AMapacheHUD::ReanudarMusicaPrincipal()
{
	if (!PrincipalAudioComponent)
	{
		return;
	}

	PrincipalAudioComponent->SetPaused(false);
	if (!PrincipalAudioComponent->IsPlaying())
	{
		PlayPrincipalMusic();
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
	HidePause();

	if (!GameOverClass)
	{
		GameOverClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/HUD/WBP_GameOver.WBP_GameOver_C"));
	}

	if (!GameOverClass)
	{
		UE_LOG(LogTemp, Error, TEXT("MapacheHUD: GameOverClass is null"));
		return;
	}

	if (!GameOverWidget)
	{
		GameOverWidget = CreateWidget<UMapacheW_GameOver>(GetWorld(), GameOverClass);
	}

	if (!GameOverWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("MapacheHUD: failed to create GameOverWidget"));
		return;
	}

	if (!GameOverWidget->IsInViewport())
	{
		GameOverWidget->bIsFocusable = true;
		GameOverWidget->AddToViewport(50);
	}

	UGameplayStatics::SetGamePaused(GetWorld(), true);
	bIsPaused = true;
	SetUIInput(GameOverWidget);
}

void AMapacheHUD::HideGameOver()
{
	if (GameOverWidget)
	{
		GameOverWidget->RemoveFromParent();
		GameOverWidget = nullptr;
	}

	UGameplayStatics::SetGamePaused(GetWorld(), false);
	bIsPaused = false;
}

void AMapacheHUD::ShowCreditos()
{
	if (PrincipalWidget)
	{
		PrincipalWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (!CreditosClass)
	{
		CreditosClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/HUD/WBP_Creditos.WBP_Creditos_C"));
	}

	if (!CreditosClass)
	{
		return;
	}

	if (!CreditosWidget)
	{
		CreditosWidget = CreateWidget<UMapacheW_Creditos>(GetWorld(), CreditosClass);
	}

	if (CreditosWidget && !CreditosWidget->IsInViewport())
	{
		CreditosWidget->AddToViewport(30);
	}

	if (CreditosWidget)
	{
		SetUIInput(CreditosWidget);
	}

	ReanudarMusicaPrincipal();
}

void AMapacheHUD::HideCreditos()
{
	if (CreditosWidget)
	{
		CreditosWidget->RemoveFromParent();
		CreditosWidget = nullptr;
	}
}

void AMapacheHUD::CerrarCreditos()
{
	HideCreditos();

	if (PrincipalWidget && PrincipalWidget->IsInViewport())
	{
		PrincipalWidget->SetVisibility(ESlateVisibility::Visible);
		SetUIInput(PrincipalWidget);
		ReanudarMusicaPrincipal();
	}
	else
	{
		ShowPrincipal();
	}
}

void AMapacheHUD::ShowAjustes()
{
	bAjustesDesdePausa = PauseWidget && PauseWidget->IsInViewport() && PauseWidget->GetVisibility() == ESlateVisibility::Visible;
	if (!bAjustesDesdePausa && PrincipalWidget && PrincipalWidget->IsInViewport())
	{
		PausarMusicaPrincipal();
	}

	if (PrincipalWidget)
	{
		PrincipalWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (PauseWidget)
	{
		PauseWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

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
		AjustesWidget->AddToViewport(40);
		SetUIInput(AjustesWidget);
	}
	else if (AjustesWidget)
	{
		SetUIInput(AjustesWidget);
	}
}

void AMapacheHUD::GuardarAjustes(float NuevoVolumen, float NuevoBrillo, bool bNuevoMute)
{
	VolumenGuardado = FMath::Clamp(NuevoVolumen, 0.f, 1.f);
	BrilloGuardado = FMath::Clamp(NuevoBrillo, 0.f, 1.f);
	bMuteGuardado = bNuevoMute;
	bAjustesGuardados = true;
}

void AMapacheHUD::HideAjustes()
{
	if (AjustesWidget)
	{
		AjustesWidget->RemoveFromParent();
		AjustesWidget = nullptr;
	}
}

void AMapacheHUD::CerrarAjustes()
{
	const bool bVolverAPausa = bAjustesDesdePausa;
	bAjustesDesdePausa = false;
	HideAjustes();

	if (bVolverAPausa && PauseWidget && PauseWidget->IsInViewport())
	{
		PauseWidget->SetVisibility(ESlateVisibility::Visible);
		SetUIInput(PauseWidget);
		return;
	}

	ReanudarMusicaPrincipal();

	if (PrincipalWidget && PrincipalWidget->IsInViewport())
	{
		PrincipalWidget->SetVisibility(ESlateVisibility::Visible);
		SetUIInput(PrincipalWidget);
		return;
	}

	ShowPrincipal();
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

void AMapacheHUD::ShowHistoria()
{
	if (!HistoriaClass)
	{
		HistoriaClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/HUD/WBP_Historia.WBP_Historia_C"));
	}

	if (!HistoriaClass)
	{
		UE_LOG(LogTemp, Error, TEXT("MapacheHUD: HistoriaClass is null"));
		ShowEnJuego();
		return;
	}

	if (!HistoriaWidget)
	{
		HistoriaWidget = CreateWidget<UMapacheW_Historia>(GetWorld(), HistoriaClass);
	}

	if (!HistoriaWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("MapacheHUD: failed to create HistoriaWidget"));
		ShowEnJuego();
		return;
	}

	if (!HistoriaWidget->IsInViewport())
	{
		HistoriaWidget->bIsFocusable = true;
		HistoriaWidget->AddToViewport(20);
	}

	HistoriaWidget->ReiniciarHistoria();
	SetUIInput(HistoriaWidget);
}

void AMapacheHUD::HideHistoria()
{
	if (HistoriaWidget)
	{
		HistoriaWidget->RemoveFromParent();
		HistoriaWidget = nullptr;
	}
}

void AMapacheHUD::ShowCarga()
{
	if (!CargaClass)
	{
		CargaClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/HUD/WBP_Carga.WBP_Carga_C"));
	}

	if (!CargaClass)
	{
		UE_LOG(LogTemp, Error, TEXT("MapacheHUD: CargaClass is null"));
		return;
	}

	if (!CargaWidget)
	{
		CargaWidget = CreateWidget<UMapacheW_Carga>(GetWorld(), CargaClass);
	}

	if (!CargaWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("MapacheHUD: failed to create CargaWidget"));
		return;
	}

	if (!CargaWidget->IsInViewport())
	{
		CargaWidget->bIsFocusable = true;
		CargaWidget->AddToViewport(100);
	}

	CargaWidget->IniciarCarga();
	SetUIInput(CargaWidget);
}

void AMapacheHUD::HideCarga()
{
	if (CargaWidget)
	{
		CargaWidget->RemoveFromParent();
		CargaWidget = nullptr;
	}
}

void AMapacheHUD::SetCargaProgreso(float Percent)
{
	if (CargaWidget)
	{
		CargaWidget->SetProgreso(Percent);
	}
}

void AMapacheHUD::SetCargaEstado(const FText& NuevoEstado)
{
	if (CargaWidget)
	{
		CargaWidget->SetEstado(NuevoEstado);
	}
}

void AMapacheHUD::StartNuevoJuego()
{
	HidePrincipal();
	HideCreditos();
	HideAjustes();
	HidePause();
	HideEnJuego();
	HideCarga();
	ShowHistoria();
}

void AMapacheHUD::ReturnToPrincipal()
{
	HidePause();
	HideGameOver();
	HideEnJuego();
	HideHistoria();
	HideCarga();
	HideAjustes();
	HideCreditos();
	ShowPrincipal();
}
