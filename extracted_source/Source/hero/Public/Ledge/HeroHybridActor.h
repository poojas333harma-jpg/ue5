// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeroCoreTypes.h"
#include "HeroHybridActor.generated.h"

/**
 * CINEMATIC VIDEO-SPRITE HYBRID ACTOR
 * Manages 4K/1080p Alpha Videos and Skeletal Mesh swapping for massive crowds.
 */
UCLASS()
class HERO_API AHeroHybridActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AHeroHybridActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

    /** 0. Default Root for Transformation */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hybrid|Components")
    class USceneComponent* DefaultRoot;

    /** 1. Media Plate for Alpha Video Sprites (Non-visual Controller) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hybrid|Components")
    class UMediaPlateComponent* MediaPlate;

    /** 2. Visual Mesh for the Video Sprite (Plane/Billboard) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hybrid|Components")
    class UStaticMeshComponent* VideoMesh;

    /** 2. 3D Skeletal Mesh for Close Proximity */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hybrid|Components")
    class USkeletalMeshComponent* Mesh3D;

    /** 3. Decal Shadow for Grounding */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hybrid|Components")
    class UDecalComponent* FootShadow;

    // ═════════════════════════════════════════════
    // HYBRID CONFIGURATION
    // ═════════════════════════════════════════════

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hybrid|Settings")
    float SwapDistance = 500.f; // 5 Meters

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hybrid|Settings")
    float ActiveMediaRadius = 5000.f; // 50 Meters (Media Pooling)

    /** Media Sources for different perspectives */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hybrid|Media")
    class UMediaSource* Video_Front;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hybrid|Media")
    class UMediaSource* Video_Left;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hybrid|Media")
    class UMediaSource* Video_Right;

    /** The Material used for the Billboard Sprite */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hybrid|Media")
    class UMaterialInterface* BillboardMaterial;

private:
    void UpdateHybridState();
    void UpdateMediaPerspective();

    bool bIsMediaActive = false;
    bool bIsCloseRange = false;
};
