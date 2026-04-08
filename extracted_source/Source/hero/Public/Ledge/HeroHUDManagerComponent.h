// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HeroHUDManagerComponent.generated.h"

/**
 * MODULAR HUD MANAGER
 * Toggles between Character HUD and Vehicle HUD (Fuel/Speed).
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HERO_API UHeroHUDManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHeroHUDManagerComponent();

    /** 
     * Switches the UI state based on possession. 
     * @param bInVehicle - If true, show Speedo / Energy.
     */
    UFUNCTION(BlueprintCallable, Category = "Hero|HUD")
    void ToggleVehicleHUD(bool bInVehicle);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class UUserWidget* ActiveVehicleHUD;

    UPROPERTY(EditDefaultsOnly, Category = "Hero|HUD")
    TSubclassOf<UUserWidget> VehicleHUDClass;
};
