// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "Components/Image.h"
#include "InventoryComponent.h"
#include "Item.h"
#include "Components/TextBlock.h"


void UInventoryWidget::RefreshInventory(UInventoryComponent* Inventory)
{
    if (!Inventory) {
        return;
    }


    UImage* Icons[4] =
    {
        ItemIcon0,
        ItemIcon1,
        ItemIcon2,
        ItemIcon3
    };

    UTextBlock* Names[4] =
    {
        ItemName0,
        ItemName1,
        ItemName2,
        ItemName3
    };

    for (int32 i = 0; i < 4; i++)
    {
        if (Icons[i])
        {
            Icons[i]->SetVisibility(ESlateVisibility::Hidden);
        }

        if (Names[i])
        {
            Names[i]->SetText(FText::GetEmpty());
        }

        if (!Inventory->Items.IsValidIndex(i))
            continue;

        AItem* Item = Cast<AItem>(Inventory->Items[i].GetObject());

        if (!Item)
            continue;

        if (Icons[i] && Item->icon)
        {
            Icons[i]->SetBrushFromTexture(Item->icon);
            Icons[i]->SetVisibility(ESlateVisibility::Visible);
        }

        if (Names[i])
        {
            Names[i]->SetText(Item->Name);
        }
    }
}
