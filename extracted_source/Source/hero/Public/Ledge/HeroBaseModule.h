// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeroBaseModule.generated.h"

/**
 * MASTER MODULE BASE
 * Unified skeleton for all outpost structures (Forge, Library, Barracks).
 */
UCLASS()
class HERO_API AHeroBaseModule : public AActor
{
	GENERATED_BODY()

public:	
	AHeroBaseModule();

protected:
	virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Module")
    class UStaticMeshComponent* BaseMesh;

    /** Primary Interaction event */
    UFUNCTION(BlueprintImplementableEvent, Category = "Module")
    void OnModuleInteracted();
};

/**
 * REPAIR FORGE
 * Restores weapon durability and removes rust.
 */
UCLASS()
class HERO_API AHeroRepairForge : public AHeroBaseModule
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Module|Repair")
    void RepairWeapon(class UHeroWeaponData* Weapon);
};

/**
 * KNOWLEDGE LIBRARY
 * Unlocks combat perks via intel database.
 */
UCLASS()
class HERO_API AHeroKnowledgeLibrary : public AHeroBaseModule
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Module|Knowledge")
    void UnlockIntelPerk(FName PerkID);
};
