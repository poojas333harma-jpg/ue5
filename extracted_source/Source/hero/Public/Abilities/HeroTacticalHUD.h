// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneCaptureComponent2D.h"
#include "HeroTacticalHUD.generated.h"

/**
 * PHASE 23-B: VIGILANTE MASK - TACTICAL HUD
 * Implements a thermal/heat-map sensing overlay for Samrat's mask.
 * Automatically highlights NPCs and threats in the Dark/Gloomy environment.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HERO_API UHeroTacticalHUD : public USceneCaptureComponent2D
{
	GENERATED_BODY()

public:	
	UHeroTacticalHUD();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // ═════════════════════════════════════════════
    // 1. HUD CONFIG
    // ═════════════════════════════════════════════

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|HUD")
    bool bIsHUDActive = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|HUD")
    float ScanRadius = 3000.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|HUD")
    FLinearColor HeatIntensity = FLinearColor(1.0f, 0.2f, 0.0f); // Warm Red

    UFUNCTION(BlueprintCallable, Category = "Hero|HUD")
    void ToggleHUD();

private:
	void UpdateHighlightLogic();
};
