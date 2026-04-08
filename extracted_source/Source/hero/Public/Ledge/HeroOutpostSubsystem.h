// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Ledge/HeroCoreTypes.h"
#include "HeroOutpostSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOutpostStateChanged, FName, OutpostID, EHeroOutpostState, NewState);

/**
 * MASTER OUTPOST SUBSYSTEM
 * Tracks the liberation and rejuvenation progress of all outposts in the world.
 */
UCLASS()
class HERO_API UHeroOutpostSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Hero|Outpost")
    void SetOutpostState(FName OutpostID, EHeroOutpostState NewState);

    UFUNCTION(BlueprintPure, Category = "Hero|Outpost")
    EHeroOutpostState GetOutpostState(FName OutpostID) const;

    UFUNCTION(BlueprintCallable, Category = "Hero|Outpost")
    FVector SnapLocationToGrid(FVector Location, float GridSize = 50.f);

    UFUNCTION(BlueprintCallable, Category = "Hero|Outpost")
    void RegisterFurniture(FName OutpostID, AActor* Furniture);

    UFUNCTION(BlueprintCallable, Category = "Hero|Outpost")
    void AssignGuard(FName OutpostID, class AHeroNPCBase* GuardNPC, FVector PostLocation);

    UPROPERTY(BlueprintAssignable, Category = "Hero|Outpost")
    FOnOutpostStateChanged OnOutpostStateChanged;

private:
    /** Map of OutpostID to its current state */
    UPROPERTY()
    TMap<FName, EHeroOutpostState> OutpostStates;

    /** Map of OutpostID to its placed furniture */
    UPROPERTY()
    TMap<FName, FHeroOutpostFurnitureList> FurnitureData;

    /** Map of OutpostID to its assigned guards */
    UPROPERTY()
    TMap<FName, FHeroGuardAssignmentList> GuardAssignments;
};
