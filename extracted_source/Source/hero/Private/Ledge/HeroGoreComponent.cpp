// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroGoreComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Ledge/HeroNPCBase.h"
#include "Components/PrimitiveComponent.h"

UHeroGoreComponent::UHeroGoreComponent()
{
}

void UHeroGoreComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (Owner)
	{
		OwnerMesh = Owner->FindComponentByClass<USkeletalMeshComponent>();
	}
}

void UHeroGoreComponent::ProcessLimbHit(FName BoneName, float HitForce, FVector HitNormal)
{
	if (!OwnerMesh) return;

	// Determine if the bone hit is a "Dismemberment" point.
	// Common Sockets: Neck, Shoulder_L, Shoulder_R, Hips, Knee_L, Knee_R
	bool bIsLimbPoint = BoneName.ToString().Contains("Shoulder") || 
						BoneName.ToString().Contains("Knee") || 
						BoneName.ToString().Contains("Neck") ||
                        BoneName.ToString().Contains("Elbow");

	if (bIsLimbPoint && HitForce > 500.f)
	{
		FVector Impulse = -HitNormal * HitForce;
		DismemberLimb(BoneName, Impulse);

		if (BoneName.ToString().Contains("Knee"))
		{
			StartPersistentBloodTrail();
		}
	}
}

void UHeroGoreComponent::DismemberLimb(FName BoneName, FVector Impulse)
{
	if (!OwnerMesh) return;

	// 1. HIDE THE BONE ON THE MASTER MESH
	// This hides the bone and all its children.
	OwnerMesh->HideBoneByName(BoneName, PBO_None);

	// 2. SPAWN BLOOD SPURT (Niagara)
    // We attach it to the parent of the hidden bone.
    FName ParentBone = OwnerMesh->GetParentBone(BoneName);
    if (ParentBone != NAME_None)
    {
        // TODO: Load a specific Gore Niagara System. 
        // For now, we manually spawn a placeholder at the socket.
        UNiagaraFunctionLibrary::SpawnSystemAttached(
            nullptr, // User must assign GFX in Blueprint
            OwnerMesh,
            BoneName,
            FVector::ZeroVector,
            FRotator::ZeroRotator,
            EAttachLocation::SnapToTarget,
            true
        );
    }
}

void UHeroGoreComponent::StartPersistentBloodTrail()
{
    if (!OwnerMesh || ActiveBloodTrail) return;

    // Attach a blood trail to the foot or knee area
    // This Niagara system should leave decals on the ground via collision.
    ActiveBloodTrail = UNiagaraFunctionLibrary::SpawnSystemAttached(
        nullptr, // User assigned
        OwnerMesh,
        FName("Foot_R"), // Example
        FVector::ZeroVector,
        FRotator::ZeroRotator,
        EAttachLocation::SnapToTarget,
        true
    );
}

void UHeroGoreComponent::StartPostMortemDissolve(float Duration)
{
    bIsDissolving = true;
    // This logic would typically update a Material Parameter Collection (MPC) 
    // or a Dynamic Material Instance's 'Dissolve' scalar parameter.
    // In C++, we start a timer or handle it in Tick.
}
