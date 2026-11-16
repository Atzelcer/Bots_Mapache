#pragma once

#include "CoreMinimal.h"
#include "MapacheUserWidget.h"
#include "MapacheW_GameOver.generated.h"

class UButton;

UCLASS(Blueprintable, BlueprintType)
class BOTS_MAPACHE_API UMapacheW_GameOver : public UMapacheUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnContinuarGO = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnSalirGO = nullptr;

	UFUNCTION()
	void OnContinuarClicked();

	UFUNCTION()
	void OnSalirClicked();
};
