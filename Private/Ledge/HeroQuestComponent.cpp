// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroQuestComponent.h"
#include "Ledge/HeroQuestData.h"
#include "Ledge/HeroInventoryComponent.h"
#include "Ledge/HeroProgressionComponent.h"
#include "Ledge/HeroItemData.h"
#include "Ledge/HeroWeaponData.h"
#include "Ledge/HeroNPCBase.h"
#include "GameFramework/Character.h"

UHeroQuestComponent::UHeroQuestComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UHeroQuestComponent::BeginPlay()
{
    Super::BeginPlay();
}

bool UHeroQuestComponent::StartQuest(UHeroQuestData* Quest)
{
    if (!Quest || CurrentActiveQuest) return false;
    
    CurrentActiveQuest = Quest;
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("QUEST STARTED: %s"), *Quest->QuestTitle.ToString()));
    return true;
}

void UHeroQuestComponent::UpdateObjective(FName QuestID, int32 ObjectiveIndex, int32 AddCount)
{
    if (!CurrentActiveQuest || !CurrentActiveQuest->QuestID.IsEqual(QuestID)) return;
    if (!CurrentActiveQuest->Objectives.IsValidIndex(ObjectiveIndex)) return;

    CurrentActiveQuest->Objectives[ObjectiveIndex].CurrentCount += AddCount;
    
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White, FString::Printf(TEXT("Progress: %s (%d/%d)"), *CurrentActiveQuest->Objectives[ObjectiveIndex].Description.ToString(), CurrentActiveQuest->Objectives[ObjectiveIndex].CurrentCount, CurrentActiveQuest->Objectives[ObjectiveIndex].TargetCount));

    CheckQuestCompletion();
}

void UHeroQuestComponent::CheckQuestCompletion()
{
    if (!CurrentActiveQuest) return;

    bool bAllDone = true;
    for (const FHeroQuestObjective& Obj : CurrentActiveQuest->Objectives)
    {
        if (!Obj.IsComplete())
        {
            bAllDone = false;
            break;
        }
    }

    if (bAllDone)
    {
        GiveQuestRewards();
        CompletedQuests.Add(CurrentActiveQuest);
        GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Green, FString::Printf(TEXT("QUEST COMPLETE: %s"), *CurrentActiveQuest->QuestTitle.ToString()));
        CurrentActiveQuest = nullptr;
    }
}

void UHeroQuestComponent::GiveQuestRewards()
{
    if (!CurrentActiveQuest) return;

    AActor* Owner = GetOwner();
    
    // 1. Give XP
    UHeroProgressionComponent* Progress = Owner->FindComponentByClass<UHeroProgressionComponent>();
    if (Progress)
    {
        Progress->AddXP(CurrentActiveQuest->RewardXP);
    }

    // 2. Give Item/Weapon
    UHeroInventoryComponent* Inventory = Owner->FindComponentByClass<UHeroInventoryComponent>();
    if (Inventory)
    {
        if (CurrentActiveQuest->RewardItem) Inventory->AddItemToSlot(CurrentActiveQuest->RewardItem->ItemID, 1);
        if (CurrentActiveQuest->RewardWeapon) Inventory->AddWeapon(CurrentActiveQuest->RewardWeapon);
    }
}
