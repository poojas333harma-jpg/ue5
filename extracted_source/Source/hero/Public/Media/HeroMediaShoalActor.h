// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeroMediaShoalActor.generated.h"

class UMediaPlayer;
class UMediaTexture;
class UMediaSource;

/**
 * PHASE 20-F: ALPHA MEDIA SHOALS
 * High-performance fish schools implemented as Alpha-Masked video plates.
 * Cycles through 5 unique videos and uses distance-based fading via DMIs.
 */
UCLASS()
class HERO_API AHeroMediaShoalActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AHeroMediaShoalActor();

protected:
	virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	virtual void Tick(float DeltaTime) override;

    // ═════════════════════════════════════════════
    // 1. CONFIGURATION
    // ═════════════════════════════════════════════

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|MediaShoal")
    TArray<TObjectPtr<UMediaSource>> ShoalAssets;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|MediaShoal")
    float MaxVisibilityDistance = 1500.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|MediaShoal")
    float FadeSpeed = 5.f;

    // ═════════════════════════════════════════════
    // 2. COMPONENTS
    // ═════════════════════════════════════════════

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hero|MediaShoal")
    TObjectPtr<UStaticMeshComponent> ShoalMesh;

private:
	UPROPERTY()
	TObjectPtr<UMediaPlayer> ActivePlayer;

    UPROPERTY()
    TObjectPtr<UMediaTexture> ActiveTexture;

    UPROPERTY()
    TObjectPtr<UMaterialInstanceDynamic> ShoalDMI;

    float CurrentOpacity = 0.f;

    void ApplyDistanceFading(float DeltaTime);
    void CycleShoalAsset();
};
