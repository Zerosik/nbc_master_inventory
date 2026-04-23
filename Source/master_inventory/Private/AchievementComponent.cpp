#include "AchievementComponent.h"

UAchievementComponent::UAchievementComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UAchievementComponent::BeginPlay()
{
    Super::BeginPlay();
    InitializeAchievements();
}

void UAchievementComponent::InitializeAchievements()
{
    if (AchivementDatatable == nullptr) {
        GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, FString::Printf(TEXT("AchivementDatatable is NOT CONNECTED!!!!")));
        return;
    }
    AchievementTable.Empty();
    TArray<FAchievementData*> AllRows;
    static const FString ContextString(TEXT("AchievementDatatableConstext"));
    AchivementDatatable->GetAllRows(ContextString, AllRows);

    for (const FAchievementData* row : AllRows) {
        int32 id = row->AchievementID;
        FAchievementData data = *row;
        AchievementTable.Emplace(id, data);
    }
}

bool UAchievementComponent::HasAchievement(int32 AchievementID) const
{
    return ClearedAchievementIDs.Contains(AchievementID);
}

bool UAchievementComponent::HasItem(int32 ItemID) const
{
    return OwnedItemIDs.Contains(ItemID);
}

bool UAchievementComponent::CanUnlockAchievement(int32 AchievementID) const
{
    if (ClearedAchievementIDs.Contains(AchievementID))
    {
        return false;
    }

    const FAchievementData* Data = AchievementTable.Find(AchievementID);
    if (Data == nullptr)
    {
        return false;
    }

    // 필요한 아이템을 이미 먹었는가
    if (!OwnedItemIDs.Contains(Data->RequiredItemID))
    {
        return false;
    }

    // 선행 업적이 있다면 먼저 클리어되어야 함
    if (Data->PrerequisiteAchievementID != -1 &&
        !ClearedAchievementIDs.Contains(Data->PrerequisiteAchievementID))
    {
        return false;
    }

    return true;
}

bool UAchievementComponent::TryUnlockAchievement(int32 AchievementID)
{
    if (!CanUnlockAchievement(AchievementID))
    {
        return false;
    }

    ClearedAchievementIDs.Add(AchievementID);
    OnAchievementUnlocked.Broadcast(AchievementID);

    GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, FString::Printf(TEXT("Achivement Unlock!! : %s"), *(AchievementTable[AchievementID]).Name));
    UE_LOG(LogTemp, Log, TEXT("Achievement Unlocked: %d"), AchievementID);
    return true;
}

void UAchievementComponent::ProcessAchievementChain()
{
    // 연쇄적으로 해금 가능한 업적을 전부 처리
    bool bUnlockedAny = false;

    do
    {
        bUnlockedAny = false;

        for (const TPair<int32, FAchievementData>& Pair : AchievementTable)
        {
            const int32 AchievementID = Pair.Key;

            if (TryUnlockAchievement(AchievementID))
            {
                bUnlockedAny = true;
            }
        }

    } while (bUnlockedAny);
}

void UAchievementComponent::OnItemAcquired(int32 ItemID)
{
    OwnedItemIDs.Add(ItemID);

    //UE_LOG(LogTemp, Log, TEXT("Item Acquired: %d"), ItemID);

    // 아이템 하나를 먹었을 때 현재 상태에서 가능한 업적을 전부 다시 검사
    ProcessAchievementChain();
}