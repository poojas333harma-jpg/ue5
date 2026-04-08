// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HeroCoreTypes.h"
#include "HeroQuestData.generated.h"



/**
 * AAA MASTER QUEST DATA ASSET
 * Definitive structure for Epic Storytelling & Side Missions.
 */
UCLASS(BlueprintType, meta=(DisplayName="Hero Quest Data"))
class HERO_API UHeroQuestData : public UDataAsset
{
    GENERATED_BODY()

public:
    // --- 1. BASIC ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1. Basic")
    FName QuestID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1. Basic")
    FText QuestTitle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1. Basic", meta=(MultiLine=true))
    FText QuestDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1. Basic")
    bool bIsMainQuest = true;

    // --- 2. OBJECTIVES ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="2. Objectives")
    TArray<FHeroQuestObjective> Objectives;

    // --- 3. REWARDS ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="3. Rewards")
    float RewardXP = 500.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="3. Rewards")
    class UHeroItemData* RewardItem;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="3. Rewards")
    class UHeroWeaponData* RewardWeapon;
};
