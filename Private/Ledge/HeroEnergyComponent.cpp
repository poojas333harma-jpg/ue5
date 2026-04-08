// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroEnergyComponent.h"

UHeroEnergyComponent::UHeroEnergyComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CurrentEnergy = MaxEnergy;
}

void UHeroEnergyComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHeroEnergyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Regenerate if energy isn't full
	if (CurrentEnergy < MaxEnergy)
	{
		HandleRegeneration(DeltaTime);
	}
}

bool UHeroEnergyComponent::ConsumeEnergy(float Amount)
{
	if (CurrentEnergy >= Amount)
	{
		CurrentEnergy -= Amount;
		return true;
	}

	// Signal overheat if energy is depleted
	if (CurrentEnergy <= 0.f)
	{
		bIsOverheated = true;
	}

	return false;
}

void UHeroEnergyComponent::DepleteEnergy()
{
	CurrentEnergy = 0.f;
	bIsOverheated = true;
}

void UHeroEnergyComponent::HandleRegeneration(float DeltaTime)
{
	// Basic linear regeneration
	CurrentEnergy += (RegenRate * DeltaTime);
	CurrentEnergy = FMath::Clamp(CurrentEnergy, 0.f, MaxEnergy);

	// Recover from overheat when energy reaches 50%
	if (bIsOverheated && (CurrentEnergy > (MaxEnergy * 0.5f)))
	{
		bIsOverheated = false;
	}
}
