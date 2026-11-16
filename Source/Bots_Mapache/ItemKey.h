// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemInterface.h"
#include "ItemKey.generated.h"

/**
 * 
 */
UCLASS()
class BOTS_MAPACHE_API AItemKey : public AItem, public IItemInterface
{
	GENERATED_BODY()
public:
	AItemKey();
	virtual void Interact(AMapachePlayer* MapachePlayer) override;
};
