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
class UMapacheW_Historia;
class UMapacheW_Carga;
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

	UPROPERTY(EditDefaultsOnly, Category = "UI|Historia")
	TSubclassOf<UUserWidget> HistoriaClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Carga")
	TSubclassOf<UUserWidget> CargaClass;

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

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UMapacheW_Historia* HistoriaWidget = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UMapacheW_Carga* CargaWidget = nullptr;

	void ShowPrincipal();
	void HidePrincipal();

	void ShowPause();
	void HidePause();

	void ShowGameOver();
	void HideGameOver();

	void ShowCreditos();
	void HideCreditos();
	void CerrarCreditos();

	void ShowAjustes();
	void HideAjustes();
	void CerrarAjustes();

	void GuardarAjustes(float NuevoVolumen, float NuevoBrillo, bool bNuevoMute);
	bool TieneAjustesGuardados() const { return bAjustesGuardados; }
	float GetVolumenGuardado() const { return VolumenGuardado; }
	float GetBrilloGuardado() const { return BrilloGuardado; }
	bool GetMuteGuardado() const { return bMuteGuardado; }

	void ShowEnJuego();
	void HideEnJuego();

	void ShowHistoria();
	void HideHistoria();

	void ShowCarga();
	void HideCarga();

	UFUNCTION(BlueprintCallable, Category = "UI|Carga")
	void SetCargaProgreso(float Percent);

	UFUNCTION(BlueprintCallable, Category = "UI|Carga")
	void SetCargaEstado(const FText& NuevoEstado);

	void StartNuevoJuego();
	void ReturnToPrincipal();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetEnergia(float Percent);

	UFUNCTION(BlueprintCallable, Category = "UI")
	UProgressBar* GetBarEnergia() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	UMapacheW_EnJuego* GetEnJuegoWidget() const { return EnJuegoWidget; }

	UFUNCTION(BlueprintCallable, Category = "UI|Carga")
	UMapacheW_Carga* GetCargaWidget() const { return CargaWidget; }

private:
	UPROPERTY()
	UAudioComponent* PrincipalAudioComponent = nullptr;

	bool bIsPaused = false;
	bool bAjustesDesdePausa = false;
	bool bAjustesGuardados = false;
	float VolumenGuardado = 1.f;
	float BrilloGuardado = 0.7f;
	bool bMuteGuardado = false;

	void SetUIInput(UUserWidget* FocusWidget);
	void SetGameAndUIInput(UUserWidget* FocusWidget);
	void SetGameInput();
	void PlayPrincipalMusic();
	void StopPrincipalMusic();
	void PausarMusicaPrincipal();
	void ReanudarMusicaPrincipal();
};
