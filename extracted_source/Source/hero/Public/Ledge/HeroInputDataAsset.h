// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputMappingContext.h"
#include "Ledge/HeroCoreTypes.h"
#include "HeroInputDataAsset.generated.h"

/**
 * MASTER INPUT DATA ASSET
 * Maps Vehicle/Pawn types to their respective Mapping Contexts (IMC).
 */
UCLASS()
class HERO_API UHeroInputDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Maps")
    TSoftObjectPtr<UInputMappingContext> IMC_Walking;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Maps")
    TSoftObjectPtr<UInputMappingContext> IMC_Driving;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Maps")
    TSoftObjectPtr<UInputMappingContext> IMC_Riding;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Maps")
    TSoftObjectPtr<UInputMappingContext> IMC_Flying;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Maps")
    int32 MappingPriority = 0;

    /** Returns the correct IMC based on the Pawn/Vehicle Type */
    UFUNCTION(BlueprintPure, Category = "Input")
    UInputMappingContext* GetIMCForPawnType(EVehicleType VehicleType);
};
