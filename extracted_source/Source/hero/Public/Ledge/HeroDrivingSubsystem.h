// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HeroDrivingSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDrivingSymmetryChanged, bool, bNewIsLeftHandDrive);

/**
 * AAA GLOBAL DRIVING SYMMETRY SUBSYSTEM
 * Central authority for LHD vs RHD transition logic.
 * Manages steering mirroring, seat location swapping, and lane priorities.
 */
UCLASS()
class HERO_API UHeroDrivingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/** Global toggle for Left-Hand Drive (True for India/UK, False for USA/Europe) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Driving|Symmetry")
	bool bIsLeftHandDrive = true;

    /** Broadcasts when the driving side is changed at runtime */
    UPROPERTY(BlueprintAssignable, Category = "Driving|Symmetry")
    FOnDrivingSymmetryChanged OnSymmetryChanged;

    /** Toggles the driving side and notifies all listeners */
    UFUNCTION(BlueprintCallable, Category = "Driving|Symmetry")
    void SetDrivingSide(bool bInIsLeftHandDrive);

    /** Utility to get the current driving side */
    UFUNCTION(BlueprintPure, Category = "Driving|Settings")
    bool IsLeftHandDrive() const { return bIsLeftHandDrive; }

	// --- 1. PERSISTENCE ---
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
