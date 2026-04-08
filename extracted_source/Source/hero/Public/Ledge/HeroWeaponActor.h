// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeroWeaponActor.generated.h"

class UHeroWeaponData;
class USkeletalMeshComponent;
class UNiagaraComponent;

/**
 * AAA MASTER WEAPON ACTOR
 * The physical actor held by the character. 
 * Handles dynamic attachments, material overrides, and Anti-Gravity VFX.
 */
UCLASS(Blueprintable, meta=(DisplayName="Hero Weapon Actor"))
class HERO_API AHeroWeaponActor : public AActor
{
    GENERATED_BODY()

public:
    AHeroWeaponActor();

protected:
    virtual void BeginPlay() override;

public:
    // ═════════════════════════════════════════════
    // 1. COMPONENTS
    // ═════════════════════════════════════════════
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
    TObjectPtr<USkeletalMeshComponent> WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon|VFX")
    TObjectPtr<UNiagaraComponent> AntiGravityVFX;

    // ═════════════════════════════════════════════
    // 2. DATA BINDING
    // ═════════════════════════════════════════════
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon|Data")
    TObjectPtr<UHeroWeaponData> WeaponData;

    // ═════════════════════════════════════════════
    // 3. CORE LOGIC
    // ═════════════════════════════════════════════
    
    /** Spawns and attaches Scopes, Silencers, etc. based on WeaponData */
    UFUNCTION(BlueprintCallable, Category="Weapon|Logic")
    void InitializeWeapon();

    /** Applies custom skins/glow materials */
    UFUNCTION(BlueprintCallable, Category="Weapon|Logic")
    void ApplyDynamicMaterials();

    /** Triggers the Blue Glow and Floating Stones */
    UFUNCTION(BlueprintCallable, Category="Weapon|VFX")
    void SetupAntiGravityEffects();
};
