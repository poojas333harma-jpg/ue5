// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HeroCharacter.generated.h"

/**
 * AAA MASTER PLAYER CHARACTER
 * The central parent for all player-controlled avatars (SandboxCharacter_Mover).
 * Equipped with Navigation Invoker for high-speed dynamic NavMesh generation.
 */
class ULedgeDetectorComponent;

UCLASS()
class HERO_API AHeroCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AHeroCharacter();

    /** THE MASTER TRAVERSAL SYSTEM (PURE 80+) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hero|Traversal")
    TObjectPtr<ULedgeDetectorComponent> LedgeDetector;


protected:
	virtual void BeginPlay() override;

	/** Mandatory for Actor Replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	virtual void Tick(float DeltaTime) override;

	// ═════════════════════════════════════════════
	// 1. NAVIGATION INVOKER (Phase 19/Roadmap)
	// ═════════════════════════════════════════════

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hero|Navigation")
	class UNavigationInvokerComponent* NavigationInvoker;

    /** Range of NavMesh generation around the player (5000.f = 50m) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Navigation")
    float NavGenerationRadius = 5000.f;

    /** Range of NavMesh removal (8000.f = 80m) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Navigation")
    float NavRemovalRadius = 8000.f;
};
