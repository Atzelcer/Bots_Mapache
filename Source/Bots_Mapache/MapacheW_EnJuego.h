#pragma once

#include "CoreMinimal.h"
#include "MapacheUserWidget.h"
#include "MapacheW_EnJuego.generated.h"

class UButton;
class UProgressBar;

UCLASS(Blueprintable, BlueprintType)
class BOTS_MAPACHE_API UMapacheW_EnJuego : public UMapacheUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetEnergia(float Percent);

	UFUNCTION(BlueprintCallable, Category = "UI")
	UProgressBar* GetBarEnergia() const { return BarEnergia; }

	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly, Category = "UI")
	UProgressBar* BarEnergia = nullptr;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnPausa = nullptr;

	UFUNCTION()
	void OnPausaClicked();
};
