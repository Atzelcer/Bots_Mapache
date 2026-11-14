#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "MapacheHUD.generated.h"

class UMapacheW_Principal;
class UMapacheW_Pause;
class UMapacheW_GameOver;
class UMapacheW_Creditos;
class UMapacheW_Ajustes;
class UMapacheW_EnJuego;
class UProgressBar;
class USoundBase;
class UAudioComponent;

UCLASS()
class BOTS_MAPACHE_API AMapacheHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMapacheHUD();

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Principal")
	TSubclassOf<UUserWidget> PrincipalClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Pause")
	TSubclassOf<UUserWidget> PauseClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI|GameOver")
	TSubclassOf<UUserWidget> GameOverClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Creditos")
	TSubclassOf<UUserWidget> CreditosClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Ajustes")
	TSubclassOf<UUserWidget> AjustesClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI|EnJuego")
	TSubclassOf<UUserWidget> EnJuegoClass;

	UPROPERTY(EditDefaultsOnly, Category = "Audio|Principal")
	USoundBase* PrincipalMusic = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UMapacheW_Principal* PrincipalWidget = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UMapacheW_Pause* PauseWidget = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UMapacheW_GameOver* GameOverWidget = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UMapacheW_Creditos* CreditosWidget = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UMapacheW_Ajustes* AjustesWidget = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UMapacheW_EnJuego* EnJuegoWidget = nullptr;

	void ShowPrincipal();
	void HidePrincipal();

	void ShowPause();
	void HidePause();

	void ShowGameOver();
	void HideGameOver();

	void ShowCreditos();
	void HideCreditos();

	void ShowAjustes();
	void HideAjustes();

	void ShowEnJuego();
	void HideEnJuego();

	void StartNuevoJuego();
	void ReturnToPrincipal();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetEnergia(float Percent);

	UFUNCTION(BlueprintCallable, Category = "UI")
	UProgressBar* GetBarEnergia() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	UMapacheW_EnJuego* GetEnJuegoWidget() const { return EnJuegoWidget; }

private:
	UPROPERTY()
	UAudioComponent* PrincipalAudioComponent = nullptr;

	bool bIsPaused = false;

	void SetUIInput(UUserWidget* FocusWidget);
	void SetGameAndUIInput(UUserWidget* FocusWidget);
	void SetGameInput();
	void PlayPrincipalMusic();
	void StopPrincipalMusic();
};
