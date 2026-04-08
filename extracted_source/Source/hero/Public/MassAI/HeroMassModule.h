// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HeroMassModule.generated.h"

/**
 * AAA MASS AI MODULE
 * Central entry point for Entity synchronization and world subsystem registration.
 * Handles the lifecycle of 5,000+ NPC entities.
 */
UCLASS()
class HERO_API UHeroMassModule : public UObject
{
	GENERATED_BODY()

public:
	// ═════════════════════════════════════════════
	// 1. MASS ENTITY REGISTRATION
	// ═════════════════════════════════════════════

	/** Registers the Mass Entity World Subsystem with Hero project configurations */
	UFUNCTION(BlueprintCallable, Category = "Hero|MassAI")
	static void InitializeMassAI(UObject* WorldContextObject);

	/** Scales entity processing based on distance from player */
	UFUNCTION(BlueprintCallable, Category = "Hero|MassAI")
	static void ConfigureLODParameters(float NearRange, float FarRange);

	/** PHASE 58: GLOBAL AGGRESSION BROADCAST
	 * Triggers a city-wide alert that diverts all Mass Entities to the target location.
	 */
	UFUNCTION(BlueprintCallable, Category = "Hero|MassAI")
	static void TriggerGlobalAlert(FVector AlertLocation, float AlertDuration = 30.0f);

	UFUNCTION(BlueprintPure, Category = "Hero|MassAI")
	static bool IsGlobalAlertActive() { return bIsGlobalAlertActive; }

	UFUNCTION(BlueprintPure, Category = "Hero|MassAI")
	static FVector GetGlobalAlertLocation() { return GlobalAlertLocation; }

private:
	// Internal tracking for entity pool stability
	static bool bIsMassReady;

	// Phase 58: Global Aggression State
	static bool bIsGlobalAlertActive;
	static FVector GlobalAlertLocation;
	static float AlertTimer;
};
