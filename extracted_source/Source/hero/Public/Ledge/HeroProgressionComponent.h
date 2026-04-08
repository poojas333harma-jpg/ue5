// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HeroCoreTypes.h"
#include "HeroProgressionComponent.generated.h"

class UHeroSkillData;

/** 
 * EVENT: For UI to update when XP or Points change 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProgressionChanged, float, NewXP, int32, NewPoints);

/**
 * AAA PROGRESSION COMPONENT (Dying Light 2 Inspired)
 * Manages XP, Leveling, and Skill Tree Unlocks.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), meta=(DisplayName="Hero Progression Component"))
class HERO_API UHeroProgressionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHeroProgressionComponent();

protected:
    virtual void BeginPlay() override;

public:
    // ═════════════════════════════════════════════
    // 1. STATS (XP & POINTS)
    // ═════════════════════════════════════════════
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Progression")
    float CurrentXP = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Progression")
    float XPToNextLevel = 1000.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Progression")
    int32 AvailableSkillPoints = 0;

    UPROPERTY(BlueprintAssignable, Category="Progression")
    FOnProgressionChanged OnProgressionChanged;

    // ═════════════════════════════════════════════
    // 2. SKILL UNLOCKS
    // ═════════════════════════════════════════════
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Progression|Skills")
    TArray<TObjectPtr<UHeroSkillData>> UnlockedSkills;

    UFUNCTION(BlueprintCallable, Category="Progression|Skills")
    bool CanUnlockSkill(UHeroSkillData* Skill);

    UFUNCTION(BlueprintCallable, Category="Progression|Skills")
    bool UnlockSkill(UHeroSkillData* Skill);

    // ═════════════════════════════════════════════
    // 3. XP LOGIC
    // ═════════════════════════════════════════════
    UFUNCTION(BlueprintCallable, Category="Progression")
    void AddXP(float Amount);
    
    UFUNCTION(BlueprintCallable, Category="Progression|Skills")
    bool IsSkillUnlocked(UHeroSkillData* Skill) const;
};
