// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"

#include "InventoryComponent.h"
#include "ItemSlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "ItemData.h"


void UInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UInventoryWidget::InitializeInventoryWidget(UInventoryComponent* InInventoryComponent)
{
    InventoryComponent = InInventoryComponent;

    if (InventoryComponent == nullptr || SlotGrid == nullptr || SlotWidgetClass == nullptr)
    {
        return;
    }

    InventoryComponent->OnInventoryChanged.AddDynamic(this, &UInventoryWidget::RefreshInventory);

    CreateSlotWidgets();
    RefreshInventory();
}

void UInventoryWidget::CreateSlotWidgets()
{
    if (InventoryComponent == nullptr || SlotGrid == nullptr || SlotWidgetClass == nullptr)
    {
        return;
    }

    SlotGrid->ClearChildren();
    SlotWidgets.Empty();

    const TArray<FItemSlotData>& Slots = InventoryComponent->GetInventory();

    for (int32 i = 0; i < Slots.Num(); ++i)
    {
        UItemSlot* SlotWidget = CreateWidget<UItemSlot>(this, SlotWidgetClass);
        if (SlotWidget == nullptr)
        {
            continue;
        }

        SlotWidgets.Add(SlotWidget);

        UUniformGridSlot* GridSlot = SlotGrid->AddChildToUniformGrid(
            SlotWidget,
            i / ColumnCount,
            i % ColumnCount
        );

        if (GridSlot)
        {
            // 필요하면 Padding 등 설정 가능
        }
    }
}

void UInventoryWidget::RefreshInventory()
{
    if (InventoryComponent == nullptr)
    {
        return;
    }

    const TArray<FItemSlotData>& Slots = InventoryComponent->GetInventory();

    for (int32 i = 0; i < Slots.Num(); ++i)
    {
        if (!SlotWidgets.IsValidIndex(i) || SlotWidgets[i] == nullptr)
        {
            continue;
        }

        const FItemSlotData& inventorySlot = Slots[i];

        if (inventorySlot.IsEmpty())
        {
            SlotWidgets[i]->SetEmpty();
            continue;
        }

        const FItemData ItemData = InventoryComponent->GetItemData(inventorySlot.ItemID);
        if (ItemData.ItemID == -1)
        {
            SlotWidgets[i]->SetEmpty();
            continue;
        }

        SlotWidgets[i]->SetItem(ItemData, inventorySlot.Amount);
    }
}