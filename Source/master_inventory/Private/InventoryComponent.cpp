// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "ItemDatabaseComponent.h"
#include "AchievementComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ItemDatabase = GetOwner()->FindComponentByClass<UItemDatabaseComponent>();
	AchievementComponent = GetOwner()->FindComponentByClass<UAchievementComponent>();

	InventoryItems.Empty();
	InventoryItems.SetNum(MaxSlotCount);
	for (FItemSlotData& slot : InventoryItems) {
		slot.Clear();
	}

	OnInventoryChanged.Broadcast();

	//test
	InventoryItems[0].ItemID = 1;
	InventoryItems[0].Amount = 1;
	InventoryItems[1].ItemID = 2;
	InventoryItems[1].Amount = 1;

}



int32 UInventoryComponent::GetFirstEmptySlotIndex() const
{
	for (int32 i = 0; i < InventoryItems.Num(); i++) {
		if (InventoryItems[i].IsEmpty()) {
			return i;
		}
	}
	return INDEX_NONE;
}

int32 UInventoryComponent::GetStackableSlotIndex(int32 ItemID) const
{
	if (!ItemDatabase)
		return INDEX_NONE;

	const FItemData ItemData = ItemDatabase->GetItem(ItemID);
	if (ItemData.ItemID == -1 || ItemData.MaxStackSize <= 1)
	{
		return INDEX_NONE;
	}

	for (int32 i = 0; i < InventoryItems.Num(); i++) {
		if (InventoryItems[i].IsEmpty() == false and
			InventoryItems[i].ItemID == ItemID and
			InventoryItems[i].Amount < 64/*MAX_STACK*/) {
			return i;
		}
	}
	return INDEX_NONE;
}

bool UInventoryComponent::AddItem(int32 ItemID, int32 count)
{
	if (count <= 0 || ItemDatabase == nullptr)
	{
		return false;
	}

	const FItemData ItemData = ItemDatabase->GetItem(ItemID);
	if (ItemData.ItemID == -1)
	{
		return false;
	}

	int32 Remaining = count;

	while (Remaining > 0)
	{
		int32 SlotIndex = GetStackableSlotIndex(ItemID);

		if (SlotIndex != INDEX_NONE)
		{
			FItemSlotData& Slot = InventoryItems[SlotIndex];
			const int32 SpaceLeft = ItemData.MaxStackSize - Slot.Amount;
			const int32 AddAmount = FMath::Min(SpaceLeft, Remaining);

			Slot.Amount += AddAmount;
			Remaining -= AddAmount;
		}
		else
		{
			SlotIndex = GetFirstEmptySlotIndex();
			if (SlotIndex == INDEX_NONE)
			{
				OnInventoryChanged.Broadcast();
				return false;
			}

			FItemSlotData& Slot = InventoryItems[SlotIndex];
			Slot.ItemID = ItemID;
			Slot.Amount = FMath::Min(ItemData.MaxStackSize, Remaining);
			Remaining -= Slot.Amount;
		}
	}

	OnInventoryChanged.Broadcast();
	return true;
}

bool UInventoryComponent::RemoveItem(int32 ItemID, int32 count)
{
	if (count <= 0)
	{
		return false;
	}

	int32 Remaining = count;

	for (FItemSlotData& Slot : InventoryItems)
	{
		if (Slot.IsEmpty() || Slot.ItemID != ItemID)
		{
			continue;
		}

		const int32 RemoveAmount = FMath::Min(Slot.Amount, Remaining);
		Slot.Amount -= RemoveAmount;
		Remaining -= RemoveAmount;

		if (Slot.Amount <= 0)
		{
			Slot.Clear();
		}

		if (Remaining <= 0)
		{
			OnInventoryChanged.Broadcast();
			return true;
		}
	}

	OnInventoryChanged.Broadcast();
	return false;
}

const FItemData UInventoryComponent::GetItemData(int32 ItemID) const
{
	if (ItemDatabase == nullptr)
	{
		return FItemData();
	}

	return ItemDatabase->GetItem(ItemID);
}

const TArray<FItemSlotData>& UInventoryComponent::GetInventory()
{
	return InventoryItems;
}

bool UInventoryComponent::CanUseItem(int32 ItemID) const
{
	if (ItemDatabase == nullptr)
	{
		return false;
	}

	const FItemData ItemData = ItemDatabase->GetItem(ItemID);
	if (ItemData.ItemID == -1)
	{
		return false;
	}

	// 업적 제한이 없으면 사용 가능
	if (!ItemData.bRequireAchievementToUse)
	{
		return true;
	}

	// 업적 제한이 있는데 업적 컴포넌트가 없으면 사용 불가
	if (AchievementComponent == nullptr)
	{
		return false;
	}

	return AchievementComponent->HasAchievement(ItemData.RequiredAchievementID);
}

bool UInventoryComponent::CanUseItemAtSlot(int32 SlotIndex) const
{
	if (!InventoryItems.IsValidIndex(SlotIndex))
	{
		return false;
	}

	const FItemSlotData& Slot = InventoryItems[SlotIndex];
	if (Slot.IsEmpty())
	{
		return false;
	}

	return CanUseItem(Slot.ItemID);
}

void UInventoryComponent::UseAllItem()
{
	int32 slotNum = 0;
	int32 i = 1;
	for (const FItemSlotData& slot : InventoryItems) {
		slotNum++;
		if (slot.IsEmpty())
			continue;
		auto item = ItemDatabase->GetItem(slot.ItemID);
		if (item.bRequireAchievementToUse) {
			//조건필요
			if (AchievementComponent->HasAchievement(item.RequiredAchievementID)) {
				GEngine->AddOnScreenDebugMessage(i, 1, FColor::Green, FString::Printf(TEXT("Slot : %d, Item Name : %s 업적 완료되어 사용가능 "), slotNum, *(item.Name).ToString()));
			}
			else {

				GEngine->AddOnScreenDebugMessage(i, 1, FColor::Red, FString::Printf(TEXT("Slot : %d, Item Name : %s 업적 미완료로 사용 불가 "), slotNum, *(item.Name).ToString()));
			}
			
		}
		else {
			GEngine->AddOnScreenDebugMessage(i, 1, FColor::Green, FString::Printf(TEXT("Slot : %d, Item Name : %s 사용가능 "), slotNum, *(item.Name).ToString()));
		}
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, FString::Printf(TEXT("player damaged %d by MineItem"), ExplosionDamage));
		i++;
	}
}

void UInventoryComponent::PrintInventory() const
{
	int32 slotNum = 0;
	int32 i = 1;
	for (const FItemSlotData& slot : InventoryItems) {
		slotNum++;
		if (slot.IsEmpty())
			continue;
		auto item = ItemDatabase->GetItem(slot.ItemID);
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, FString::Printf(TEXT("player damaged %d by MineItem"), ExplosionDamage));
		GEngine->AddOnScreenDebugMessage(i, 1, FColor::White, FString::Printf(TEXT("Slot : %d, ItemID : %d, Item Name : %s, Count : %d"), slotNum, slot.ItemID, *(item.Name).ToString(), slot.Amount));
		i++;
	}
}



