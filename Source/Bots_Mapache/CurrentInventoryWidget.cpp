#include "CurrentInventoryWidget.h"
#include "Item.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"

namespace
{
    const FString IconPath = TEXT("/Game/UI/current_inventory.current_inventory");
}

void UCurrentInventoryWidget::RefreshCurrentInventory(TScriptInterface<IItemInterface> InItem)
{
    if (!ItemIcon || !ItemName || !ItemDescription)
    {
        UE_LOG(LogTemp, Error, TEXT("Faltan componentes en CurrentInventoryWidget"));
        return;
    }

    ItemIcon->SetVisibility(ESlateVisibility::Hidden);
    ItemName->SetText(FText::GetEmpty());
    ItemDescription->SetText(FText::GetEmpty());

    if (!InItem || !InItem.GetObject())
    {
        UE_LOG(LogTemp, Warning, TEXT("El item seleccionado es invalido"));
        return;
    }

    AItem* Item = Cast<AItem>(InItem.GetObject());

    if (!Item)
    {
        UE_LOG(LogTemp, Error, TEXT("El objeto no es de tipo AItem"));
        return;
    }

    if (Item->icon)
    {
        ItemIcon->SetBrushFromTexture(Item->icon);
        ItemIcon->SetVisibility(ESlateVisibility::Visible);
    }

    ItemName->SetText(Item->Name);
    ItemDescription->SetText(Item->Description);

    UE_LOG(LogTemp, Warning, TEXT("Item seleccionado: %s"), *Item->Name.ToString());
}