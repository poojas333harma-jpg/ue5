// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroMassCrowdProcessor.h"
#include "MassCommonFragments.h"
#include "MassMovementFragments.h"
#include "MassNavigationFragments.h"
#include "MassExecutionContext.h"
#include "MassEntityManager.h"
#include "MassAI/HeroMassModule.h"
#include "MassEntityTypes.h"

UHeroMassCrowdProcessor::UHeroMassCrowdProcessor()
{
	bAutoRegisterWithProcessingPhases = true;
	ProcessingPhase = EMassProcessingPhase::PrePhysics;

	// In UE 5.7, we must register queries in the constructor for requirement stability.
	RegisterQuery(CrowdQuery);
}

void UHeroMassCrowdProcessor::ConfigureQueries(const TSharedRef<FMassEntityManager>& EntityManager)
{
    CrowdQuery.AddRequirement<FTransformFragment>(EMassFragmentAccess::ReadWrite);
    CrowdQuery.AddRequirement<FMassVelocityFragment>(EMassFragmentAccess::ReadWrite);
    CrowdQuery.AddRequirement<FMassMoveTargetFragment>(EMassFragmentAccess::ReadWrite);
}

void UHeroMassCrowdProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
    // High-performance ECS processing for 5,000+ NPCs (UE 5.7 standard)
    // If a global aggression alert is active, divert all Mass Entities toward the target.

    // Using UE 5.7 ForEachEntityChunk pattern
    CrowdQuery.ForEachEntityChunk(Context, [this](FMassExecutionContext& IterContext)
    {
        const int32 NumEntities = IterContext.GetNumEntities();
        
        // Use standard UE 5.7 Mass Entity fragment access patterns.
        TArrayView<FTransformFragment> TransformList = IterContext.GetMutableFragmentView<FTransformFragment>();
        TArrayView<FMassVelocityFragment> VelocityList = IterContext.GetMutableFragmentView<FMassVelocityFragment>();
        TArrayView<FMassMoveTargetFragment> MoveTargetList = IterContext.GetMutableFragmentView<FMassMoveTargetFragment>();

        const bool bIsGlobalAlertActive = UHeroMassModule::IsGlobalAlertActive();
        const FVector AlertLocation = UHeroMassModule::GetGlobalAlertLocation();
        const float DT = IterContext.GetDeltaTimeSeconds();

        for (int32 i = 0; i < NumEntities; ++i)
        {
            FTransformFragment& TransformElem = TransformList[i];
            FMassVelocityFragment& VelocityElem = VelocityList[i];
            FMassMoveTargetFragment& MoveTargetElem = MoveTargetList[i];

            if (bIsGlobalAlertActive)
            {
                // GTA-style Chase: Divert movement target toward the alert location
                MoveTargetElem.Center = AlertLocation;
                MoveTargetElem.DistanceToGoal = FVector::Dist(TransformElem.GetTransform().GetLocation(), AlertLocation);
                
                // Increase speed for aggression (UE 5.7 constructor pattern)
                MoveTargetElem.DesiredSpeed = FMassInt16Real(600.f); 
            }
            
            // Basic movement integration
            FTransform& MutableTransform = TransformElem.GetMutableTransform();
            FVector NewLocation = MutableTransform.GetLocation() + (VelocityElem.Value * DT);
            MutableTransform.SetLocation(NewLocation);
        }
    });
}
