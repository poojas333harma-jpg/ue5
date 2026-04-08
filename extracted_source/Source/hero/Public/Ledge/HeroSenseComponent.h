// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HeroCoreTypes.h"
#include "HeroSenseComponent.generated.h"

/**
 * AAA SAMRAT SENSE (X-RAY VISION)
 * Handlers scanning actors through walls using CustomDepth/Stencil.
 * Linked to UHeroEnergyComponent for Stamina integration.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HERO_API UHeroSenseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHeroSenseComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // ═════════════════════════════════════════════
    // 1. SCANNING & HIGHLIGHTS
    // ═════════════════════════════════════════════

    /** Current Scan Radius (Meters). Adjustable in Editor (10m-50m) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Sense")
    float SenseRange = 25.0f;

    /** Active Highlight Color (Enemies=Red, Loot=Gold) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Sense")
    FLinearColor ScanColor = FLinearColor::Red;

    /** Triggers the Niagara Scan Wave and highlights nearby actors */
    UFUNCTION(BlueprintCallable, Category = "Hero|Sense")
    void TriggerScan();

    /** Manually toggles X-Ray Highlight state based on stamina */
    UFUNCTION(BlueprintCallable, Category = "Hero|Sense")
    void SetSenseActive(bool bActive);

    // ═════════════════════════════════════════════
    // 2. ENERGY & PERFORMANCE
    // ═════════════════════════════════════════════

    /** Stamina Consumption Rate per second */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Sense")
    float DurationStaminaCost = 5.0f;

    /** Level-Bound: Only scans actors in the current streaming level */
    UFUNCTION(BlueprintCallable, Category = "Hero|Sense")
    void PerformLevelBoundScan();

    /** Cooldown Timer (Seconds) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Sense")
    float ScanCooldown = 5.0f;

private:
    bool bIsSenseActive = false;
    float LastScanTime = 0.0f;

    /** Internal helper to enable/disable CustomDepth state */
    void SetActorHighLight(AActor* Target, bool bEnable, int32 StencilValue = 252);
};
