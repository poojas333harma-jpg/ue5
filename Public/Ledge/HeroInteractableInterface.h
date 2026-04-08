// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HeroCoreTypes.h"
#include "HeroInteractableInterface.generated.h"

/**
 * AAA INTERACTABLE INTERFACE
 * Standardizes how the player interacts with world objects (Chests, Doors, Items).
 */
UINTERFACE(MinimalAPI, BlueprintType)
class UHeroInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class HERO_API IHeroInteractableInterface
{
	GENERATED_BODY()

public:
    /** Main interaction trigger (e.g. Press E) */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
	void Interact(AActor* Interactor);

    /** When the crosshair enters the interactive zone */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
	void BeginHover(AActor* Interactor);

    /** When the crosshair leaves the interactive zone */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
	void EndHover(AActor* Interactor);

    /** Returns the floating text to display (e.g. "Open Chest [Locked]") */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
    FText GetInteractionText() const;

    /** Returns the security type (None, Lockpick, Hack, etc.) */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
    EHeroSecurityType GetSecurityType() const;

    /** Returns the lock difficulty (Easy to Legendary) */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
    EHeroLockDifficulty GetLockDifficulty() const;
};
