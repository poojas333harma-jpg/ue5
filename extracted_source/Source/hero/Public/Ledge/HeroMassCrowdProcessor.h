// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "HeroMassCrowdProcessor.generated.h"

/**
 * AAA MASS CROWD PROCESSOR
 * Technical heart of the 5,000+ NPC simulation.
 * Handles high-performance movement and state updates for Mass Entities.
 */
UCLASS()
class HERO_API UHeroMassCrowdProcessor : public UMassProcessor
{
	GENERATED_BODY()

public:
	UHeroMassCrowdProcessor();

protected:
	/** Main execution logic for Mass Entities */
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

	/** Configure requirements for Mass Entities */
	virtual void ConfigureQueries(const TSharedRef<FMassEntityManager>& EntityManager) override;

private:
	/** Query for updating entity positions on splines */
	FMassEntityQuery CrowdQuery;
};
