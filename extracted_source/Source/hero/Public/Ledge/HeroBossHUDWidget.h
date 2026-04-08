// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HeroCoreTypes.h"
#include "HeroBossHUDWidget.generated.h"

/**
 * AAA CINEMATIC BOSS HUD
 * Handles smooth interpolation of boss health/power and displays data-driven attributes.
 */
UCLASS()
class HERO_API UHeroBossHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    /** Synchronizes the UI with a new Boss target */
    UFUNCTION(BlueprintCallable, Category = "Hero|UI")
    void InitializeBossHUD(FHeroBossAttributes InAttributes);

    /** Updates the visual progress of the Health bar (Interpolated in BP) */
    UFUNCTION(BlueprintCallable, Category = "Hero|UI")
    void UpdateHealth(float CurrentHealth, float MaxHealth);

    /** Updates the visual progress of the Power bar */
    UFUNCTION(BlueprintCallable, Category = "Hero|UI")
    void UpdatePower(float CurrentPower, float MaxPower);

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Hero|UI")
    FHeroBossAttributes BossData;

    UPROPERTY(BlueprintReadOnly, Category = "Hero|UI")
    float TargetHealthPercent = 1.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Hero|UI")
    float TargetPowerPercent = 1.0f;

    /** Blueprint event to trigger the 'Hurt' shake or flash effect */
    UFUNCTION(BlueprintImplementableEvent, Category = "Hero|UI")
    void OnBossDamaged();
};
