// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HeroCoreTypes.h"
#include "HeroQuestComponent.generated.h"

class UHeroQuestData;

/**
 * AAA QUEST SYSTEM COMPONENT
 * Manages storytelling, mission tracking, and reward handling.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), meta=(DisplayName="Hero Quest Component"))
class HERO_API UHeroQuestComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHeroQuestComponent();

protected:
    virtual void BeginPlay() override;

public:
    // ═════════════════════════════════════════════
    // 1. ACTIVE MISSIONS
    // ═════════════════════════════════════════════
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Quest")
    UHeroQuestData* CurrentActiveQuest;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Quest")
    TArray<UHeroQuestData*> CompletedQuests;

    // ═════════════════════════════════════════════
    // 2. CORE LOGIC
    // ═════════════════════════════════════════════
    UFUNCTION(BlueprintCallable, Category="Quest")
    bool StartQuest(UHeroQuestData* Quest);

    UFUNCTION(BlueprintCallable, Category="Quest")
    void UpdateObjective(FName QuestID, int32 ObjectiveIndex, int32 AddCount);

    UFUNCTION(BlueprintCallable, Category="Quest")
    void CheckQuestCompletion();

    UFUNCTION(BlueprintCallable, Category="Quest")
    void GiveQuestRewards();
};
