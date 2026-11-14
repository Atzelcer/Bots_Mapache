#pragma once

#include "CoreMinimal.h"
#include "MapacheUserWidget.h"
#include "MapacheW_Historia.generated.h"

class UButton;
class UImage;
class UTexture2D;

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
	int32 GetNumDiapositivas() const { return Diapositivas.Num(); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Historia")
	TArray<TSoftObjectPtr<UTexture2D>> Diapositivas;

protected:
	virtual void NativeConstruct() override;

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
	void MostrarDiapositiva(int32 Indice);
	void FinalizarHistoria();

	int32 IndiceActual = 0;
};
