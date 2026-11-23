#pragma once

#include "CoreMinimal.h"
#include "MapacheUserWidget.h"
#include "MapacheW_Pause.generated.h"

class UButton;

UCLASS(Blueprintable, BlueprintType)
class BOTS_MAPACHE_API UMapacheW_Pause : public UMapacheUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnContinuar = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnAjustes = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnSalir = nullptr;

	UFUNCTION()
	void OnContinuarClicked();

	UFUNCTION()
	void OnAjustesClicked();

	UFUNCTION()
	void OnSalirClicked();
};
