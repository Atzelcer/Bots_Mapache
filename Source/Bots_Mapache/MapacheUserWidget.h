#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapacheUserWidget.generated.h"


UCLASS(Abstract, Blueprintable, BlueprintType)
class BOTS_MAPACHE_API UMapacheUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Called after the widget is constructed (native + BlueprintImplementable). */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UI")
	void OnWidgetReady();
	virtual void OnWidgetReady_Implementation();

protected:
	virtual void NativeConstruct() override;
};
