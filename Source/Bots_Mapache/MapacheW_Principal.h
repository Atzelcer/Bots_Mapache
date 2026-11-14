#pragma once

#include "CoreMinimal.h"
#include "MapacheUserWidget.h"
#include "MapacheW_Principal.generated.h"

class UButton;

UCLASS(Blueprintable, BlueprintType)
class BOTS_MAPACHE_API UMapacheW_Principal : public UMapacheUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnNuevoJuego = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnAjustes = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnSalir = nullptr;

	UFUNCTION()
	void OnNuevoJuegoClicked();

	UFUNCTION()
	void OnAjustesClicked();

	UFUNCTION()
	void OnSalirClicked();
};
