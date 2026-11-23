// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemInterface.h"
#include "ItemBread.generated.h"

/**
 * 
 */
UCLASS()
class BOTS_MAPACHE_API AItemBread : public AItem, public IItemInterface
{
	GENERATED_BODY()
public:
	AItemBread();
	virtual void Interact(AMapachePlayer* MapachePlayer) override;
};
