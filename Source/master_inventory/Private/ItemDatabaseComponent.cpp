// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDatabaseComponent.h"
#include "ItemData.h"

UItemDatabaseComponent::UItemDatabaseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	ItemDataTable = nullptr;
}


const FItemData UItemDatabaseComponent::GetItem(int32 ItemID) const
{
	if (Database.Contains(ItemID))
		return Database[ItemID];
	else
		return FItemData();
	//return FItemData();
}

TMap<int32, FItemData> UItemDatabaseComponent::GetItemDatabase()
{
	return Database;
	//return TMap<int32, FItemData>();
}

void UItemDatabaseComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<FItemData*> AllRows;
	static const FString ContextString(TEXT("ItemDatabaseConstext"));
	ItemDataTable->GetAllRows(ContextString, AllRows);

	for (const FItemData* row : AllRows) {
		int32 id = row->ItemID;
		FItemData data = *row;
		Database.Emplace(id, data);
	}
}

