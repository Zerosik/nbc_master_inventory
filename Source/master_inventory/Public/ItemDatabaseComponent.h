// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemData.h"
#include "ItemDatabaseComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MASTER_INVENTORY_API UItemDatabaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UItemDatabaseComponent();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemDatabase")
	UDataTable* ItemDataTable;
	UPROPERTY()
	TMap<int32, FItemData> Database;

	const FItemData GetItem(int32 ItemID) const;
	TMap<int32, FItemData> GetItemDatabase();

protected:
	virtual void BeginPlay() override;
	
};
