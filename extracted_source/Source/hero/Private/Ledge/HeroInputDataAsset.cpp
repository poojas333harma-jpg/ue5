// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroInputDataAsset.h"
#include "InputMappingContext.h"

UInputMappingContext* UHeroInputDataAsset::GetIMCForPawnType(EVehicleType VehicleType)
{
    TSoftObjectPtr<UInputMappingContext> TargetIMC;

    switch (VehicleType)
    {
        case EVehicleType::Car:
        case EVehicleType::Bike:
        case EVehicleType::Truck:
        case EVehicleType::VoidDrive:
        case EVehicleType::Trolley:
        case EVehicleType::Rail:
            TargetIMC = IMC_Driving;
            break;

        case EVehicleType::Mount:
        case EVehicleType::Carriage:
            TargetIMC = IMC_Riding;
            break;

        case EVehicleType::Air:
        case EVehicleType::Water:
        case EVehicleType::Glider:
            TargetIMC = IMC_Flying;
            break;

        default:
            TargetIMC = IMC_Walking;
            break;
    }

    if (TargetIMC.IsPending())
    {
        return TargetIMC.LoadSynchronous();
    }
    return TargetIMC.Get();
}
