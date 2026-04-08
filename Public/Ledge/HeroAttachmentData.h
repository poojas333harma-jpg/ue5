// Copyright 2026 Hero Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HeroCoreTypes.h"
#include "HeroAttachmentData.generated.h"

/**
 * AAA WEAPON ATTACHMENT DATA
 * Defines physical modules (Scopes, Silencers, Modules) that attach to weapon sockets.
 */
UCLASS(BlueprintType, meta=(DisplayName="Hero Weapon Attachment"))
class HERO_API UHeroAttachmentData : public UDataAsset
{
    GENERATED_BODY()

public:
    // --- 1. BASIC ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1. Basic")
    FName AttachmentName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="1. Basic")
    EHeroWeaponAttachmentSlot SlotType = EHeroWeaponAttachmentSlot::Scope;

    // --- 2. PHYSICAL ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="2. Model")
    TObjectPtr<UStaticMesh> Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="2. Model")
    FName SocketName = FName("Scope_Socket"); // Muzzle_Socket, Mag_Socket, etc.

    // --- 3. MODIFIERS ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="3. Stats")
    FHeroWeaponModifier Modifiers;

    // --- 4. EFFECTS ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="4. Effects")
    class USoundBase* SoundModifier; // For Silencer

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="4. Effects")
    class UNiagaraSystem* AttachmentVFX; // For Glowing modules
};
