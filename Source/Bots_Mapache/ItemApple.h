// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemInterface.h"
#include "ItemApple.generated.h"

/**
 * 
 */
class AMapachePlayer;
UCLASS()
class BOTS_MAPACHE_API AItemApple : public AItem,public IItemInterface
{
	GENERATED_BODY()
	
public:
	AItemApple();
	virtual void Interact(AMapachePlayer* MapachePlayer) override;
};
