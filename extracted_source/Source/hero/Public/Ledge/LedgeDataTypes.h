// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LedgeDataTypes.generated.h"

UENUM(BlueprintType)
enum class ELedgeType : uint8
{
    Ledge_None = 0,
    Wall,
    Beam,
    Pipe_Thin,
    Pipe_Thick,
    Ladder,
    Squeeze_Gap
};

UENUM(BlueprintType)
enum class ELedgeAction : uint8
{
    None = 0,
    Vault = 1,
    Grab_Idle = 2,
    Mantle = 3
};

UENUM(BlueprintType)
enum class EHeroLedgeTraversalState : uint8
{
    Default,
    Hanging,
    Parkour
};

USTRUCT(BlueprintType)
struct FLedgeExecutionResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Ledge|Result")
    bool bSuccess = false;

    UPROPERTY(BlueprintReadWrite, Category = "Ledge|Result")
    ELedgeType LedgeType = ELedgeType::Wall;

    UPROPERTY(BlueprintReadWrite, Category = "Ledge|Result")
    ELedgeAction ActionType = ELedgeAction::None;

    UPROPERTY(BlueprintReadWrite, Category = "Ledge|Result")
    FTransform WarpTransform = FTransform::Identity;

    UPROPERTY(BlueprintReadWrite, Category = "Ledge|Result")
    TObjectPtr<class UAnimMontage> MontageToPlay = nullptr;
};
