// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HeroCoreTypes.h"
#include "HeroWeaponData.generated.h"

/**
 * AAA MASTER WEAPON DATA ASSET
 * Definitive structure for Combat, Durability, and AAA Animations.
 */
UCLASS(BlueprintType, meta=(DisplayName="Hero Weapon Data"))
class HERO_API UHeroWeaponData : public UDataAsset
{
    GENERATED_BODY()

public:
    // ═════════════════════════════════════════════
    // 1. BASIC INFORMATION
    // ═════════════════════════════════════════════
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1. Basic")
    FName WeaponID = FName("Wep_001");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1. Basic")
    FText WeaponName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1. Basic")
    EHeroWeaponType WeaponType = EHeroWeaponType::OneHanded_Sword;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1. Basic")
    TObjectPtr<USkeletalMesh> SkeletalMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1. Basic")
    TObjectPtr<UStaticMesh> StaticMesh;

    // ═════════════════════════════════════════════
    // 2. COMBAT STATS
    // ═════════════════════════════════════════════
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="2. Combat")
    FHeroWeaponStats Stats;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="2. Combat")
    EHeroAttackType PrimaryAttackType = EHeroAttackType::Light;

    // ═════════════════════════════════════════════
    // 3. ANIMATION & MONTAGES
    // ═════════════════════════════════════════════
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="3. Animation")
    FOverlayMontageSet AnimationSet;

    // ═════════════════════════════════════════════
    // 4. EFFECTS (AESTHETICS)
    // ═════════════════════════════════════════════
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="4. Effects")
    class UNiagaraSystem* ImpactVFX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="4. Effects")
    class USoundBase* ImpactSFX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="4. Effects")
    FHeroSurfaceDecals SurfaceDecals;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="4. Effects")
    bool bIsDivine = false; // Triggers Special Divine VFX

    // ═════════════════════════════════════════════
    // 5. OVERLAY & LOCOMOTION
    // ═════════════════════════════════════════════
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="5. Overlay")
    EHeroOverlayState TargetOverlayState = EHeroOverlayState::Unarmed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="5. Overlay")
    float MoveSpeedModifier = 1.0f;

    // ═════════════════════════════════════════════
    // 6. SPECIAL LOGIC (GOW AXE)
    // ═════════════════════════════════════════════
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="6. Special")
    bool bIsThrowable = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="6. Special")
    bool bReturnsToHand = false;

    // ═════════════════════════════════════════════
    // 7. UI & INVENTORY
    // ═════════════════════════════════════════════
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="7. UI")
    EHeroItemRarity Rarity = EHeroItemRarity::Common;

    // ═════════════════════════════════════════════
    // 8. 3D INSPECTION (DYING LIGHT 2 STYLE)
    // ═════════════════════════════════════════════
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="8. Inspection")
    TObjectPtr<UMeshComponent> InspectionMeshOverride; // High-poly menu mesh

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="8. Inspection")
    class UNiagaraSystem* InspectionBackgroundVFX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="8. Inspection")
    class USoundBase* MenuAmbientMusic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="8. Inspection")
    FLinearColor MenuAccentLight = FLinearColor::White;

    // ═════════════════════════════════════════════
    // 9. LORE & CODEX (DYING LIGHT 2 STYLE)
    // ═════════════════════════════════════════════
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="9. Lore")
    FHeroLoreEntry LoreData;

    // ═════════════════════════════════════════════
    // DURABILITY & REPAIR (PHASE 10)
    // ═════════════════════════════════════════════
    UPROPERTY(BlueprintReadOnly, Category="Weapon|State")
    bool bIsBroken = false;

    /** Returns current damage scaled by durability (Sharpness logic) */
    UFUNCTION(BlueprintCallable, Category="Weapon|Logic")
    float GetCalculatedDamage() const
    {
        if (bIsBroken) return 0.0f;
        
        // Multiplier from 1.0 (New) to 0.4 (Very Worn)
        float DurabilityScale = GetCurrentPowerMultiplier();
        return Stats.BaseDamage * DurabilityScale;
    }

    /** Logic to break or destroy weapon at 0 durability */
    UFUNCTION(BlueprintCallable, Category="Weapon|Logic")
    void ProcessDurabilityLoss(float Amount)
    {
        Stats.CurrentDurability = FMath::Max(0.f, Stats.CurrentDurability - Amount);
        
        if (Stats.CurrentDurability <= 0.f)
        {
            // Logic: Rare+ items break, Common items destroy (handled by inventory)
            if (Rarity >= EHeroItemRarity::Rare)
            {
                bIsBroken = true;
            }
        }
    }

    // ═════════════════════════════════════════════
    // 10. MODULAR ATTACHMENTS & UPGRADES
    // ═════════════════════════════════════════════
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="10. Modular")
    TArray<TObjectPtr<class UHeroAttachmentData>> DefaultAttachments;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="10. Modular")
    TMap<FName, TObjectPtr<class UMaterialInterface>> MaterialOverrides;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="10. Modular")
    bool bHasAntiGravityMod = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="10. Modular")
    bool bEnemiesFloatOnDeath = false;

    /** Calculates the total weight including attachments */
    UFUNCTION(BlueprintCallable, Category="Weapon|Logic")
    float GetCalculatedWeight() const;

    /** Helper to calculate current power based on durability */

    /** Helper to calculate current power based on durability */
    UFUNCTION(BlueprintCallable, Category="Weapon|Logic")
    float GetCurrentPowerMultiplier() const
    {
        if (Stats.MaxDurability <= 0.f) return 1.0f;
        float Ratio = Stats.CurrentDurability / Stats.MaxDurability;
        // Example: Power drops to 50% if durability is 0
        return FMath::Lerp(0.5f, 1.0f, Ratio);
    }
};
