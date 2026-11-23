#pragma once

#include "CoreMinimal.h"
#include "MapacheUserWidget.h"
#include "MapacheW_Historia.generated.h"

class UButton;
class UImage;
class UTexture2D;
class USoundBase;
class UAudioComponent;

UCLASS(Blueprintable, BlueprintType)
class BOTS_MAPACHE_API UMapacheW_Historia : public UMapacheUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI|Historia")
	void ReiniciarHistoria();

	UFUNCTION(BlueprintCallable, Category = "UI|Historia")
	void Siguiente();

	UFUNCTION(BlueprintCallable, Category = "UI|Historia")
	void Anterior();

	UFUNCTION(BlueprintCallable, Category = "UI|Historia")
	void Saltar();

	UFUNCTION(BlueprintCallable, Category = "UI|Historia")
	int32 GetIndiceActual() const { return IndiceActual; }

	UFUNCTION(BlueprintCallable, Category = "UI|Historia")
	int32 GetNumDiapositivas() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Historia")
	UTexture2D* Historia_01 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Historia")
	UTexture2D* Historia_02 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Historia")
	UTexture2D* Historia_03 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Historia")
	UTexture2D* Historia_04 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Historia")
	UTexture2D* Historia_05 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Historia")
	UTexture2D* Historia_06 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Historia")
	UTexture2D* Historia_07 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Historia|Audio")
	USoundBase* MusicaFondo = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Historia|Audio")
	USoundBase* SfxComunicacion = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Historia|Audio", meta = (ClampMin = "0", ClampMax = "1"))
	float VolumenComunicacion = 0.4f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Historia|Audio", meta = (ClampMin = "0", ClampMax = "6"))
	TArray<int32> IndicesConComunicacion;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly, Category = "UI")
	UImage* ImgHistoria = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnSiguiente = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnAnterior = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnSaltar = nullptr;

	UFUNCTION()
	void OnSiguienteClicked();

	UFUNCTION()
	void OnAnteriorClicked();

	UFUNCTION()
	void OnSaltarClicked();

	void CargarDiapositivasPorDefecto();
	void CargarAudioPorDefecto();
	void ReconstruirLista();
	UTexture2D* GetDiapositiva(int32 Indice) const;
	void MostrarDiapositiva(int32 Indice, bool bPlaySfx);
	void IniciarMusicaFondo();
	void DetenerMusicaFondo();
	bool DebeReproducirComunicacion(int32 Indice) const;
	void ReproducirComunicacion();
	void IrAlJuego();
	void IrAlPrincipal();

	UPROPERTY(Transient)
	TArray<UTexture2D*> Diapositivas;

	UPROPERTY(Transient)
	UAudioComponent* MusicaAudioComponent = nullptr;

	int32 IndiceActual = 0;
};
