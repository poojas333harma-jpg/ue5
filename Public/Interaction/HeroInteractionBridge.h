// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeroInteractionBridge.generated.h"

class UMediaPlayer;
class UMediaTexture;
class UMediaSource;
class AHeroNPCBase;

/**
 * PHASE 22-B: SEAMLESS VIDEO INTERACTION
 * Swaps a 3D NPC Mesh with an Alpha-Media plate upon interaction.
 * Syncs environment states (Cold 8000K) to the Media Material.
 */
UCLASS()
class HERO_API AHeroInteractionBridge : public AActor
{
	GENERATED_BODY()
	
public:	
	AHeroInteractionBridge();

protected:
	virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	virtual void Tick(float DeltaTime) override;

    // ═════════════════════════════════════════════
    // 1. ASSETS & TARGETS
    // ═════════════════════════════════════════════

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Cinematic")
    TSoftObjectPtr<AHeroNPCBase> TargetNPC;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Cinematic")
    TSoftObjectPtr<UMediaSource> CinematicSource;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hero|Cinematic")
    TObjectPtr<UStaticMeshComponent> MediaPlate;

    // ═════════════════════════════════════════════
    // 2. INTERACTION BRIDGE LOGIC
    // ═════════════════════════════════════════════

    /** Triggers the zero-latency swap from 3D to Video */
    UFUNCTION(BlueprintCallable, Category = "Hero|Cinematic")
    void TriggerCinematicInteraction();

    /** Switches back from Video to 3D NPC Mesh */
    UFUNCTION(BlueprintCallable, Category = "Hero|Cinematic")
    void RevertTo3DMesh();

private:
	UPROPERTY()
	TObjectPtr<UMediaPlayer> ActivePlayer;

    UPROPERTY()
    TObjectPtr<UMediaTexture> ActiveTexture;

    UPROPERTY()
    TObjectPtr<UMaterialInstanceDynamic> PlateDMI;

    void SyncEnvironmentToMaterial();

    UFUNCTION()
    void OnCinematicFinished();

    bool bIsCinematicActive = false;
};
