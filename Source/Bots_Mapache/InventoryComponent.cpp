// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::AddItem(TScriptInterface<IItemInterface> InItem)
{
	if (!InItem) return;

	if (Items.Num() >= MaxSlots) return;

	// Evitar objetos duplicados
	for (const TScriptInterface<IItemInterface>& Item : Items)
	{
		if (Item.GetObject() == InItem.GetObject())
		{
			return;
		}
	}

	Items.Add(InItem);
}

void UInventoryComponent::RemoveItem(TScriptInterface<IItemInterface> InItem)
{
	if (!InItem) return;
	AActor* ItemActor = Cast<AActor>(InItem.GetObject());

	for (auto It = Items.CreateIterator(); It; ++It)
	{
		if (It->GetObject() == InItem.GetObject())
		{
			It.RemoveCurrent();

			if (IsValid(ItemActor))
			{
				ItemActor->Destroy();
			}
			break;
		}
	}
}

TScriptInterface<IItemInterface> UInventoryComponent::GetItemByIndex(int32 Index)
{
	TScriptInterface<IItemInterface> Item;

	if (Items.IsValidIndex(Index))
	{
		Item = Items[Index];
		//Items.RemoveAt(Index);
	}

	return Item;
}
