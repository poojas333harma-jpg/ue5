// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Ledge/HeroCoreTypes.h"
#include "HeroNPCDataAsset.generated.h"

/**
 * MASTER NPC ARCHETYPE CONFIGURATION
 * Maps AI Brain (BehaviorTree) and Gore VFX (Niagara) to NPC Profiles.
 */
USTRUCT(BlueprintType)
struct FHeroNPCProfileConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<class UBehaviorTree> BehaviorTree;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<class UNiagaraSystem> GoreVFX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<class UAnimMontage> ActionMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FHeroAIPerceptionConfig PerceptionConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EHeroGender Gender = EHeroGender::Male;
};

UCLASS()
class HERO_API UHeroNPCDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Config")
    TMap<EHeroNPCProfile, FHeroNPCProfileConfig> ProfileConfigurations;

    UFUNCTION(BlueprintPure, Category = "NPC|Config")
    void GetConfigForProfile(EHeroNPCProfile Profile, bool& bSuccess, FHeroNPCProfileConfig& Config)
    {
        if (ProfileConfigurations.Contains(Profile))
        {
            Config = ProfileConfigurations[Profile];
            bSuccess = true;
        }
        else
        {
            bSuccess = false;
        }
    }

    /** Applies the profile configuration to the target NPC actor */
    UFUNCTION(BlueprintCallable, Category = "NPC|Config")
    void PopulateNPC(class AHeroNPCBase* NPC, EHeroNPCProfile Profile)
    {
        if (!NPC) return;

        FHeroNPCProfileConfig Config;
        bool bSuccess = false;
        GetConfigForProfile(Profile, bSuccess, Config);

        if (bSuccess)
        {
            // Logic to apply BT, VFX, etc. 
            // In a real project, you would call NPC->SetBehaviorTree(Config.BehaviorTree.LoadSynchronous());
        }
    }
};
