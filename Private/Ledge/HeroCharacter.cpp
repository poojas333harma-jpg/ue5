// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroCharacter.h"
#include "Net/UnrealNetwork.h"
#include "NavigationInvokerComponent.h"
#include "Ledge/LedgeDetectorComponent.h"
#include "DrawDebugHelpers.h"

AHeroCharacter::AHeroCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Enable Replication for Phase 60.1
	bReplicates = true;
	SetReplicateMovement(true);

	// --- Step 2: Dynamic NavMesh Generation ---
	NavigationInvoker = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavigationInvoker"));
	NavigationInvoker->SetGenerationRadii(NavGenerationRadius, NavRemovalRadius);

    // --- Step 3: AAA TRAVERSAL SYSTEM (PURE 80+) ---
    LedgeDetector = CreateDefaultSubobject<ULedgeDetectorComponent>(TEXT("LedgeDetector"));
}


void AHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
	
    // Register the invoker (in modern UE)
    // Invokers ensure NavMesh only exists around active characters
}

void AHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHeroCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Add gameplay-critical properties here for Phase 60.1
	// DOREPLIFETIME(AHeroCharacter, SomeProperty);
}
