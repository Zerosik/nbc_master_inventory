// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

USTRUCT(BlueprintType)
struct MASTER_INVENTORY_API FItemSlotData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Amount;

    FItemSlotData() :ItemID(-1), Amount(0) {}
    bool IsEmpty() const {
        return ItemID < 0 || Amount <= 0;
    }

    void Clear() {
        ItemID = -1;
        Amount = 0;
    }
};

USTRUCT(BlueprintType)
struct MASTER_INVENTORY_API FItemData : public FTableRowBase
{
	GENERATED_BODY()
public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxStackSize;

    //업적에 따라 사용가능여부를 추가
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool bRequireAchievementToUse = false;

    // bRequireAchievementToUse가 true일 때 필요한 업적 ID
    // -1이면 없음
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 RequiredAchievementID = -1;

    FItemData() : ItemID(-1), MaxStackSize(0){}
};
