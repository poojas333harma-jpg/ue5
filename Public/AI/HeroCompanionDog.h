// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Ledge/HeroAnimalBase.h"
#include "HeroCompanionDog.generated.h"

/**
 * PHASE 23-B: THE DOG ARMY - COMPANION AI
 * Implements a specialized pack AI for hunting dogs.
 * Coordination: Following, Guarding, and Coordinated Strikes with Samrat.
 */
UCLASS()
class HERO_API AHeroCompanionDog : public AHeroAnimalBase
{
	GENERATED_BODY()

public:
	AHeroCompanionDog();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

    // ═════════════════════════════════════════════
    // 1. COMPANION STATE
    // ═════════════════════════════════════════════

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Companion")
    bool bIsLeader = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hero|Companion")
    AActor* CurrentLeader; // Samrat or Alpha Dog

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Companion")
    float FollowDistance = 400.f;

    // ═════════════════════════════════════════════
    // 2. PACK LOGIC (Doga Interaction)
    // ═════════════════════════════════════════════

    UFUNCTION(BlueprintCallable, Category = "Hero|Companion")
    void CommandPackAttack(AActor* Target);

    UFUNCTION(BlueprintCallable, Category = "Hero|Companion")
    void CommandPackFormUp();

private:
	void ExecuteFormationFollow(float DeltaTime);
    void UpdateAggroLogic();

    FTimerHandle PackTacticsTimer;
};
