// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemInterface.h"
#include "CurrentInventoryWidget.generated.h"

/**
 * 
 */
class UImage;
class UTextBlock;
UCLASS()
class BOTS_MAPACHE_API UCurrentInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

    UPROPERTY(meta = (BindWidget))
    UImage* ItemIcon;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemName;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemDescription;

	UFUNCTION(BlueprintCallable)
	void RefreshCurrentInventory(TScriptInterface<IItemInterface> InItem);
};
