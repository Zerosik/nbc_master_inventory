// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AchievementData.generated.h"


USTRUCT(BlueprintType)
struct FAchievementData : public FTableRowBase
{
	GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Achievement")
    int32 AchievementID = -1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Achievement")
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Achievement")
    int32 RequiredItemID = -1;

    // -1이면 선행 업적 없음
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Achievement")
    int32 PrerequisiteAchievementID = -1;
};
