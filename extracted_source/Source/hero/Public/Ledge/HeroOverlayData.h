// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HeroCoreTypes.h"
#include "HeroOverlayData.generated.h"

/**
 * AAA OVERLAY DATA ASSET
 * Mapping a pose to its respective animations and physical behaviors.
 */
UCLASS(BlueprintType, meta=(DisplayName="Hero Overlay Data"))
class HERO_API UHeroOverlayData : public UDataAsset
{
    GENERATED_BODY()

public:
    // --- State ID ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Overlay|State")
    EHeroOverlayState OverlayState = EHeroOverlayState::Unarmed;

    // --- Core Animations ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Overlay|Animations")
    class UAnimSequence* StancePose; // The base posture

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Overlay|Animations")
    FOverlayMontageSet Montages; // Combat/Action animations

    // --- Physical Movement Modifiers ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Overlay|Movement")
    float WalkSpeedMultiplier = 1.0f; // Slow down for heavy objects

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Overlay|Movement")
    float RunSpeedMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Overlay|Movement")
    float SprintEnabled = true;

    // --- Interaction ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Overlay|Interaction")
    bool bCanPickupItems = true;

    // --- State Priority (Optional) ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Overlay|Priority")
    int32 StatePriority = 0; // Higher = Overrides others
};
