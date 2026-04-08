// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Ledge/HeroCoreTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "HeroAIController.generated.h"

/**
 * MASTER AI CONTROLLER
 * Handles AI Perception (Sight/Hearing), Tactical Logic, and Group Alerts.
 */
UCLASS()
class HERO_API AHeroAIController : public AAIController
{
	GENERATED_BODY()

public:
    AHeroAIController();

protected:
    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;

public:
    /** 
     * Dynamically updates the Sight Radius based on environment light.
     * @param LightMultiplier - 1.0f (Full Light) to 0.5f (Full Darkness).
     */
    UFUNCTION(BlueprintCallable, Category = "Hero|AI")
    void UpdatePerceptionByLight(float LightMultiplier);

    /** Tactical: Request backup from nearby allies of the same faction. */
    UFUNCTION(BlueprintCallable, Category = "Hero|AI")
    void RequestBackup();

    /** Tactical: Move to a flanking position relative to the player. */
    UFUNCTION(BlueprintCallable, Category = "Hero|AI")
    void ExecuteFlanking(AActor* PlayerTarget);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hero|AI")
    UAIPerceptionComponent* HeroPerception;

    class UAISenseConfig_Sight* SightConfig;
    class UAISenseConfig_Hearing* HearingConfig;

    FHeroAIPerceptionConfig BasePerception;

private:
    UFUNCTION()
    void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
