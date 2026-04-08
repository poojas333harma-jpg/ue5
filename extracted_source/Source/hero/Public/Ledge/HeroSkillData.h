// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HeroCoreTypes.h"
#include "HeroSkillData.generated.h"

/**
 * AAA MASTER SKILL DATA ASSET
 * Definitive structure for Dying Light 2 style Skill Trees.
 */
UCLASS(BlueprintType, meta=(DisplayName="Hero Skill Data"))
class HERO_API UHeroSkillData : public UDataAsset
{
    GENERATED_BODY()

public:
    // --- 1. BASIC INFO ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1. Basic")
    FName SkillID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1. Basic")
    FText SkillName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1. Basic", meta=(MultiLine=true))
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1. Basic")
    EHeroSkillCategory Category = EHeroSkillCategory::Combat;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1. Basic")
    EHeroSkillType SkillType = EHeroSkillType::Active;

    // --- 2. PREREQUISITES (The Tree Logic) ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="2. Requirements")
    TArray<TObjectPtr<UHeroSkillData>> PrerequisiteSkills;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="2. Requirements")
    int32 SkillPointCost = 1;

    // --- 3. AESTHETICS ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="3. Aesthetics")
    TObjectPtr<UTexture2D> Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="3. Aesthetics")
    TObjectPtr<class UAnimMontage> SkillMontage; // If Active

    // --- 4. EFFECTS ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="4. Effects")
    float StatMultiplier = 1.0f; // If Passive (e.g. Stamina x1.2)
};
