// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemInterface.h"
#include "ItemFish.generated.h"
/**
 * 
 */

UCLASS()
class BOTS_MAPACHE_API AItemFish : public AItem, public IItemInterface
{
	GENERATED_BODY()

public:
	AItemFish();
	virtual void Interact(AMapachePlayer* Player) override;
};
