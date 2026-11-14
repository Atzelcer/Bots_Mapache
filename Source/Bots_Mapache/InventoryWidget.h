// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
class UImage;
class UTextBlock;
class UInventoryComponent;

UCLASS()
class BOTS_MAPACHE_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    UPROPERTY(meta = (BindWidget))
    UImage* ItemIcon0;

    UPROPERTY(meta = (BindWidget))
    UImage* ItemIcon1;

    UPROPERTY(meta = (BindWidget))
    UImage* ItemIcon2;

    UPROPERTY(meta = (BindWidget))
    UImage* ItemIcon3;


    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemName0;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemName1;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemName2;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemName3;


    UFUNCTION(BlueprintCallable)
    void RefreshInventory(UInventoryComponent* Inventory);
};
