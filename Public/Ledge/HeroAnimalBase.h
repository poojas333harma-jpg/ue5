// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Ledge/HeroCoreTypes.h"
#include "HeroAnimalBase.generated.h"

/**
 * MASTER ANIMAL BASE
 * Handles specialized behaviors for Horses, Wolves, and Birds.
 */
UCLASS()
class HERO_API AHeroAnimalBase : public ACharacter
{
	GENERATED_BODY()

public:
	AHeroAnimalBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

    // ═════════════════════════════════════════════
    // 1. ANIMAL SETTINGS
    // ═════════════════════════════════════════════

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animal|Profile")
    EHeroNPCProfile AnimalProfile = EHeroNPCProfile::Stray;

    /** Animation Table specifically for Animals */
    UPROPERTY(EditDefaultsOnly, Category = "Animal|Animation")
    class UDataTable* BehaviorDataTable;

    UPROPERTY(BlueprintReadOnly, Category = "Animal|Animation")
    FAnimalBehaviorSet CurrentBehaviorSet;

    // ═════════════════════════════════════════════
    // 2. COMPONENTS
    // ═════════════════════════════════════════════

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animal|Components")
    class UMotionWarpingComponent* MotionWarpingComponent;

    // ═════════════════════════════════════════════
    // 3. ACTIONS
    // ═════════════════════════════════════════════

    UFUNCTION(BlueprintCallable, Category = "Animal|Actions")
    void PerformVocalize();

    UFUNCTION(BlueprintCallable, Category = "Animal|Actions")
    void PerformAction(); // Graze/Sniff/Scratch

    // ═════════════════════════════════════════════
    // 4. TACTICAL LOGIC
    // ═════════════════════════════════════════════

    /** Coordinate attack with other nearby animals of the same profile. */
    UFUNCTION(BlueprintCallable, Category = "Animal|Tactics")
    void ExecutePackMentality();

    /** React to nearby combat by fleeing or rearing up. */
    UFUNCTION(BlueprintCallable, Category = "Animal|Tactics")
    void ExecuteFleeReaction(FVector DangerSource);
};
