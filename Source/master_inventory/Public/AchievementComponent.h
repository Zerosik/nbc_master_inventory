// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AchievementData.h"
#include "AchievementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAchievementUnlocked, int32, AchievementID);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MASTER_INVENTORY_API UAchievementComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAchievementComponent();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AchivementDatatable")
    UDataTable* AchivementDatatable;
protected:
    virtual void BeginPlay() override;
private:


    // 업적 정의 테이블
    UPROPERTY()
    TMap<int32, FAchievementData> AchievementTable;

    // 플레이어가 획득한 아이템
    UPROPERTY()
    TSet<int32> OwnedItemIDs;

    // 클리어한 업적
    UPROPERTY()
    TSet<int32> ClearedAchievementIDs;

private:
    void InitializeAchievements();

    bool CanUnlockAchievement(int32 AchievementID) const;
    bool TryUnlockAchievement(int32 AchievementID);
    void ProcessAchievementChain();

public:
    UPROPERTY(BlueprintAssignable, Category = "Achievement")
    FOnAchievementUnlocked OnAchievementUnlocked;

    UFUNCTION(BlueprintCallable, Category = "Achievement")
    void OnItemAcquired(int32 ItemID);

    UFUNCTION(BlueprintCallable, Category = "Achievement")
    bool HasAchievement(int32 AchievementID) const;

    UFUNCTION(BlueprintCallable, Category = "Achievement")
    bool HasItem(int32 ItemID) const;
};