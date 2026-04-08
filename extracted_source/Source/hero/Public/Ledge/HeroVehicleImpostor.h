// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeroVehicleImpostor.generated.h"

/**
 * AAA MULTI-ANGLE VEHICLE IMPOSTOR
 * Uses Octagonal Sprites (8 Angles) for massive traffic simulation.
 * Features: View-Angle Swapping (Yaw/Pitch), Wheel Rotation Illusion, 
 * and LHD/RHD texture mirroring.
 */
UCLASS(Blueprintable, meta = (DisplayName = "Hero Vehicle Impostor"))
class HERO_API AHeroVehicleImpostor : public AActor
{
	GENERATED_BODY()

public:
	AHeroVehicleImpostor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
    /** 1. Main Billboard for the Vehicle Sprite */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Traffic|Impostor")
    class UStaticMeshComponent* BillboardMesh;

    /** 2. Wheel Sprites for Animation Illusion (Phase 18-C) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Traffic|Impostor")
    class UStaticMeshComponent* WheelSprite_FL;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Traffic|Impostor")
    class UStaticMeshComponent* WheelSprite_FR;

    /** Atlas containing all 8 angles (4K) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic|Impostor")
    class UTexture2D* ImpostorAtlas;

    /** Current speed (controls wheel rotation speed) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic|Impostor")
    float CurrentSpeedKmh = 0.f;

private:
	/** Updates the Sprite Angle based on Camera Perspective (Dot Product Logic) */
	void UpdateViewAngle();

    /** Rotates wheel sprites to simulate movement */
    void AnimateWheels(float DeltaTime);

    /** Current yaw value of the player camera relative to vehicle */
    float RelativeYaw = 0.f;
};
