// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroWeaponActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "Ledge/HeroWeaponData.h"
#include "Ledge/HeroAttachmentData.h"

AHeroWeaponActor::AHeroWeaponActor()
{
    PrimaryActorTick.bCanEverTick = false;

    // 1. Base Weapon Mesh
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    RootComponent = WeaponMesh;

    // 2. Anti-Gravity VFX
    AntiGravityVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("AntiGravityVFX"));
    AntiGravityVFX->SetupAttachment(WeaponMesh);
    AntiGravityVFX->bAutoActivate = false;
}

void AHeroWeaponActor::BeginPlay()
{
    Super::BeginPlay();
    if (WeaponData) InitializeWeapon();
}

void AHeroWeaponActor::InitializeWeapon()
{
    if (!WeaponData) return;

    // Set Base Mesh
    WeaponMesh->SetSkeletalMesh(WeaponData->SkeletalMesh);

    // Spawn Attachments
    for (const UHeroAttachmentData* Attachment : WeaponData->DefaultAttachments)
    {
        if (Attachment && Attachment->Mesh)
        {
            UStaticMeshComponent* NewAttach = NewObject<UStaticMeshComponent>(this);
            NewAttach->SetStaticMesh(Attachment->Mesh);
            NewAttach->RegisterComponent();
            NewAttach->AttachToComponent(WeaponMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, Attachment->SocketName);
            
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Attached: %s to %s"), *Attachment->AttachmentName.ToString(), *Attachment->SocketName.ToString()));
        }
    }

    ApplyDynamicMaterials();
    SetupAntiGravityEffects();
}

void AHeroWeaponActor::ApplyDynamicMaterials()
{
    if (!WeaponData) return;

    for (const TPair<FName, UMaterialInterface*>& MaterialEntry : WeaponData->MaterialOverrides)
    {
        // Search index by slot name (simplified)
        int32 SlotIndex = WeaponMesh->GetMaterialIndex(MaterialEntry.Key);
        if (SlotIndex != INDEX_NONE)
        {
            WeaponMesh->SetMaterial(SlotIndex, MaterialEntry.Value);
        }
    }
}

void AHeroWeaponActor::SetupAntiGravityEffects()
{
    if (WeaponData && WeaponData->bHasAntiGravityMod)
    {
        AntiGravityVFX->Activate(true);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("ANTI-GRAVITY ACTIVATED: WEIGHT DROPPED!"));
    }
}
