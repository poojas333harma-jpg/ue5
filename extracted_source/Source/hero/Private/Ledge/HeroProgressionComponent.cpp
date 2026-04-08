// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroProgressionComponent.h"
#include "Ledge/HeroSkillData.h"

UHeroProgressionComponent::UHeroProgressionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UHeroProgressionComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UHeroProgressionComponent::AddXP(float Amount)
{
    CurrentXP += Amount;
    if (CurrentXP >= XPToNextLevel)
    {
        CurrentXP -= XPToNextLevel;
        XPToNextLevel *= 1.2f; // Progressive scale
        AvailableSkillPoints += 1;
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("LEVEL UP! +1 Skill Point"));
    }
    OnProgressionChanged.Broadcast(CurrentXP, AvailableSkillPoints);
}

bool UHeroProgressionComponent::CanUnlockSkill(UHeroSkillData* Skill)
{
    if (!Skill) return false;
    if (IsSkillUnlocked(Skill)) return false;
    if (AvailableSkillPoints < Skill->SkillPointCost) return false;

    // Check prerequisites
    for (UHeroSkillData* Prereq : Skill->PrerequisiteSkills)
    {
        if (!IsSkillUnlocked(Prereq)) return false;
    }

    return true;
}

bool UHeroProgressionComponent::UnlockSkill(UHeroSkillData* Skill)
{
    if (CanUnlockSkill(Skill))
    {
        AvailableSkillPoints -= Skill->SkillPointCost;
        UnlockedSkills.AddUnique(Skill);
        OnProgressionChanged.Broadcast(CurrentXP, AvailableSkillPoints);
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("SKILL UNLOCKED: %s"), *Skill->SkillName.ToString()));
        return true;
    }
    return false;
}

bool UHeroProgressionComponent::IsSkillUnlocked(UHeroSkillData* Skill) const
{
    return UnlockedSkills.Contains(Skill);
}
