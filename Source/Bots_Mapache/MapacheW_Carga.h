#pragma once

#include "CoreMinimal.h"
#include "MapacheUserWidget.h"
#include "MapacheW_Carga.generated.h"

class UImage;
class UTextBlock;
class UTexture2D;
class USoundBase;
class UAudioComponent;

UCLASS(Blueprintable, BlueprintType)
class BOTS_MAPACHE_API UMapacheW_Carga : public UMapacheUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI|Carga")
	void IniciarCarga();

	UFUNCTION(BlueprintCallable, Category = "UI|Carga")
	void SetProgreso(float Percent);

	UFUNCTION(BlueprintCallable, Category = "UI|Carga")
	float GetProgreso() const { return Progreso; }

	UFUNCTION(BlueprintCallable, Category = "UI|Carga")
	void SetEstado(const FText& NuevoEstado);

	UFUNCTION(BlueprintCallable, Category = "UI|Carga")
	UImage* GetBarraCarga() const { return barraCarga; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Carga", meta = (ClampMin = "0", ClampMax = "1"))
	float Progreso = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Carga", meta = (ClampMin = "0.5"))
	float DuracionCarga = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Carga")
	UTexture2D* Frame_1c = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Carga")
	UTexture2D* Frame_2c = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Carga")
	UTexture2D* Frame_3c = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Carga")
	UTexture2D* Frame_4c = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Carga")
	UTexture2D* Frame_5c = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Carga|Audio")
	USoundBase* AudioAmbiente = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Carga|Audio")
	USoundBase* AudioMagia = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Carga|Audio", meta = (ClampMin = "0", ClampMax = "1"))
	float VolumenAmbiente = 0.35f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Carga|Audio", meta = (ClampMin = "0", ClampMax = "1"))
	float VolumenMagia = 0.55f;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly, Category = "UI")
	UImage* ImgFondo = nullptr;

	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly, Category = "UI")
	UImage* barraCarga = nullptr;

	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly, Category = "UI")
	UTextBlock* TxtEstado = nullptr;

	void CargarAssetsPorDefecto();
	void ReconstruirFrames();
	void ActualizarFrame();
	void IniciarAudio();
	void DetenerAudio();
	void FinalizarCarga();

	UPROPERTY(Transient)
	TArray<UTexture2D*> Frames;

	UPROPERTY(Transient)
	UAudioComponent* AmbienteAudioComponent = nullptr;

	UPROPERTY(Transient)
	UAudioComponent* MagiaAudioComponent = nullptr;

	FTimerHandle CargaTimerHandle;
	int32 IndiceFrame = 0;
	bool bCargaActiva = false;
};
