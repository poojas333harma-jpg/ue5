// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroHealthComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Ledge/HeroNPCBase.h"
#include "Ledge/HeroAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "Materials/MaterialInterface.h"
#include "Ledge/HeroCoreTypes.h"

UHeroHealthComponent::UHeroHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// 1. Initialize Body Parts
	BodyPartHealth.Add("Head", 100.f);
	BodyPartHealth.Add("Torso", 100.f);
	BodyPartHealth.Add("LeftArm", 100.f);
	BodyPartHealth.Add("RightArm", 100.f);
	BodyPartHealth.Add("LeftLeg", 100.f);
	BodyPartHealth.Add("RightLeg", 100.f);
	
	CurrentHealth = MaxHealth;
}

void UHeroHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHeroHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Check if in Radioactive Fog (handled by Weather Manager or specific triggers)
	// ApplyEnvironmentalDamage(0.5f, DeltaTime);
}

void UHeroHealthComponent::ApplyRegionalDamage(float Damage, FName BoneName, AActor* DamageCauser)
{
	if (Damage <= 0.f || bIsLimping && BoneName == "Torso") return;

	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);

	// Trigger the Damage Event (Phase 17)
	OnDamageTaken.Broadcast(Damage, GetOwner()->GetActorLocation());

	// 1. Map Bone to Body Part
	FName BodyPart = "Torso"; // Default
	if (BoneName.ToString().Contains("head")) BodyPart = "Head";
	else if (BoneName.ToString().Contains("leg_l") || BoneName.ToString().Contains("knee_l")) BodyPart = "LeftLeg";
	else if (BoneName.ToString().Contains("leg_r") || BoneName.ToString().Contains("knee_r")) BodyPart = "RightLeg";
	else if (BoneName.ToString().Contains("arm_l") || BoneName.ToString().Contains("elbow_l")) BodyPart = "LeftArm";
	else if (BoneName.ToString().Contains("arm_r") || BoneName.ToString().Contains("elbow_r")) BodyPart = "RightArm";

	// 2. NPC SPECIFIC LOGIC: CRITICAL HITS
	if (BodyPart == "Head" && GetOwner()->IsA(AHeroNPCBase::StaticClass()))
	{
		// Instant Death for most NPCs on Headshot
		CurrentHealth = 0.f;
		if (AHeroNPCBase* HostNPC = Cast<AHeroNPCBase>(GetOwner()))
		{
			HostNPC->Die();
		}
		return;
	}

	// 3. Apply Damage to specific part
	if (BodyPartHealth.Contains(BodyPart))
	{
		BodyPartHealth[BodyPart] -= Damage;
		BodyPartHealth[BodyPart] = FMath::Clamp(BodyPartHealth[BodyPart], 0.f, 100.f);
	}

	// 4. Update Overall Health
	CurrentHealth -= Damage;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHealth);

	// 5. Trigger Blood VFX
	if (GetOwner())
	{
		SpawnBloodDecal(GetOwner()->GetActorLocation(), GetOwner()->GetActorForwardVector());
	}

	// 6. Check for Limping/Fleeing
	CheckLimbCondition();

	// 7. Handle Death
	if (CurrentHealth <= 0.f)
	{
		if (AHeroNPCBase* HostNPC = Cast<AHeroNPCBase>(GetOwner()))
		{
			HostNPC->Die();
		}
	}
}

void UHeroHealthComponent::ApplyEnvironmentalDamage(float DamagePerSecond, float DeltaTime)
{
	CurrentHealth -= (DamagePerSecond * DeltaTime);
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHealth);
}

void UHeroHealthComponent::SpawnBloodDecal(FVector Location, FVector Normal)
{
	if (!BloodDecalMaterial) return;

	// Randomize Decal Rotation for "Messy" look
	FRotator RandomRot = Normal.Rotation();
	RandomRot.Roll = FMath::FRandRange(-180.f, 180.f);

	// Trace down to floor
	FHitResult Hit;
	FVector Start = Location;
	FVector End = Location - (FVector::UpVector * 200.f);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		UGameplayStatics::SpawnDecalAtLocation(GetWorld(), BloodDecalMaterial, FVector(50.f, 50.f, 50.f), Hit.ImpactPoint, RandomRot, 10.f);
	}
}

void UHeroHealthComponent::CheckLimbCondition()
{
	float LeftLegHealth = BodyPartHealth["LeftLeg"];
	float RightLegHealth = BodyPartHealth["RightLeg"];

	// 1. LEG INJURY: LIMPING
	if (LeftLegHealth < 30.f || RightLegHealth < 30.f)
	{
		if (!bIsLimping)
		{
			bIsLimping = true;
			// Switch to Injured Overlay (Mover System)
		}
	}
	else
	{
		bIsLimping = false;
	}

	// 2. PROFILE SPECIFIC BEHAVIOR: FLEEING
	if (AHeroNPCBase* HostNPC = Cast<AHeroNPCBase>(GetOwner()))
	{
		if (HostNPC->NPCProfile == EHeroNPCProfile::Coward && CurrentHealth < (MaxHealth * 0.4f))
		{
			// TODO: Trigger AI Fleeing Task in Behavior Tree
			UE_LOG(LogTemp, Warning, TEXT("%s: COWARD PROFILE TRIGGERED FLEEING!"), *HostNPC->GetName());
		}
	}
}
