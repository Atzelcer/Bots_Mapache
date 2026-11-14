// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryItem.h"
#include "ItemInterface.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOTS_MAPACHE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Inventory")
	int32 MaxSlots = 4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<TScriptInterface<IItemInterface>> Items;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(TScriptInterface<IItemInterface> InItem);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItem(TScriptInterface<IItemInterface> InItem);
};
