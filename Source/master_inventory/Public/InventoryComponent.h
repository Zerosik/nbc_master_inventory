// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemData.h"
#include "InventoryComponent.generated.h"

class UItemDatabaseComponent;
class UAchievementComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MASTER_INVENTORY_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FItemSlotData> InventoryItems;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 MaxSlotCount=12;	

	UPROPERTY()
	UItemDatabaseComponent* ItemDatabase = nullptr;
	UPROPERTY()
	UAchievementComponent* AchievementComponent = nullptr;

private:
	int32 GetFirstEmptySlotIndex() const;
	int32 GetStackableSlotIndex(int32 ItemID) const;
	bool bIsInventoryOpen = false;
public:

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryChanged OnInventoryChanged;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(int32 ItemID, int32 count = 1);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(int32 ItemID, int32 count = 1);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	const FItemData GetItemData(int32 ItemID) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	const TArray<FItemSlotData>& GetInventory();

	//사용가능여부
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool CanUseItem(int32 ItemID) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool CanUseItemAtSlot(int32 SlotIndex) const;


	bool RemoveItemAtSlot(int32 SlotIndex, int32 count = 1);
	bool SwapSlot(int32 From, int32 To);
	bool HasItem(int32 ItemID, int32 RequireCount = 1) const;

	//test
	void UseAllItem();
	void PrintInventory() const;
	void ShowInventory();

		
};
