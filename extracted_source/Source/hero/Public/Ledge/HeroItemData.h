// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HeroCoreTypes.h"
#include "HeroItemData.generated.h"

/**
 * AAA MASTER ITEM DATA ASSET
 * Structure for Consumables, Crafting Mats, and Quest Items.
 */
UCLASS(BlueprintType, meta=(DisplayName="Hero Item Data"))
class HERO_API UHeroItemData : public UDataAsset
{
    GENERATED_BODY()

public:
    // --- 1. BASIC ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1. Basic")
    FName ItemID = FName("Item_001");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1. Basic")
    FText ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1. Basic")
    EHeroItemRarity Rarity = EHeroItemRarity::Common;

    // --- 2. STATS ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="2. Stats")
    FHeroItemStats Stats;

    // --- 3. AESTHETICS ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="3. Aesthetics")
    class UTexture2D* Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="3. Aesthetics")
    class USoundBase* PickupSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="3. Aesthetics")
    class UStaticMesh* WorldMesh;

    // --- 4. INTERACTION ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="4. Interaction")
    class UAnimMontage* UseMontage;

    // --- 5. 3D INSPECTION ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="5. Inspection")
    TObjectPtr<UStaticMesh> InspectionMeshOverride;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="5. Inspection")
    class UNiagaraSystem* InspectionBackgroundVFX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="5. Inspection")
    class USoundBase* MenuAmbientMusic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="5. Inspection")
    FLinearColor MenuAccentLight = FLinearColor::White;

    // --- 6. LORE (STORY) ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="6. Lore")
    FHeroLoreEntry LoreData;
};
