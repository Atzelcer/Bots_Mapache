// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class BOTS_MAPACHE_API UInventoryItem : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReandOnly, Category = 'Item')
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReandOnly, Category = 'Item')
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReandOnly, Category = 'Item')
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReandOnly, Category = 'Item')
	UTexture2D icon;

	UPROPERTY(EditAnywhere, BlueprintReandOnly, Category = 'Item')
	int32 MaxStack = 4;
};
