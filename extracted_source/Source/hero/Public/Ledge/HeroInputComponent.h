// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Ledge/HeroCoreTypes.h"
#include "HeroInputComponent.generated.h"

// Forward Declarations
class UHeroInputDataAsset;
class UInputMappingContext;

/**
 * MODULAR INPUT COMPONENT
 * Handles swapping Enhanced Input Mapping Contexts (IMC) based on the current pawn.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HERO_API UHeroInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHeroInputComponent();

    /** 
     * Master Swapper: Swaps IMCs based on the new pawn. 
     * @param NewPawn - The pawn that is being possessed.
     */
    UFUNCTION(BlueprintCallable, Category = "Hero|Input")
    void NotifyPossessedPawn(APawn* NewPawn);

protected:
	virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "Hero|Input")
    UHeroInputDataAsset* MasterInputData;

private:
    /** Reference to the last applied IMC to allow clean removal. */
    UPROPERTY()
    UInputMappingContext* LastAppliedIMC = nullptr;

    void ApplyMappingContext(UInputMappingContext* Context);
    void RemoveMappingContext(UInputMappingContext* Context);
};
