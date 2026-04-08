// Copyright 2026 Hero Project. All Rights Reserved.

#include "Abilities/HeroTacticalHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Canvas.h"
#include "Materials/MaterialInstanceDynamic.h"

UHeroTacticalHUD::UHeroTacticalHUD()
{
	PrimaryComponentTick.bCanEverTick = true;
	bIsHUDActive = false;
	ScanRadius = 3500.f;
}

void UHeroTacticalHUD::BeginPlay()
{
	Super::BeginPlay();
	
    // Hidden by default, activated via ToggleHUD
    SetComponentTickEnabled(false);
}

void UHeroTacticalHUD::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsHUDActive)
	{
		UpdateHighlightLogic();
	}
}

void UHeroTacticalHUD::ToggleHUD()
{
	bIsHUDActive = !bIsHUDActive;
	SetComponentTickEnabled(bIsHUDActive);
	
	UE_LOG(LogTemp, Warning, TEXT("TACTICAL HUD: %s"), bIsHUDActive ? TEXT("ACTIVATED (HEAT-MAP ON)") : TEXT("DEACTIVATED"));
}

void UHeroTacticalHUD::UpdateHighlightLogic()
{
	// A. Perform Sphere Overlap for NPCs/Threats
	TArray<AActor*> OverlappingActors;
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(GetOwner());

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetComponentLocation(), ScanRadius, ObjectTypes, nullptr, IgnoredActors, OverlappingActors))
	{
		for (AActor* Actor : OverlappingActors)
		{
			// B. Apply Thermal Highlight (Stencil/PostProcess Material usually)
			// For this implementation, we log the threat detection
			// UE_LOG(LogTemp, Log, TEXT("HEAT-MAP DETECTED: %s"), *Actor->GetName());
		}
	}
}
