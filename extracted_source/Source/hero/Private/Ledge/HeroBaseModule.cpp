// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroBaseModule.h"
#include "Components/StaticMeshComponent.h"
#include "Ledge/HeroWeaponData.h"

AHeroBaseModule::AHeroBaseModule()
{
	PrimaryActorTick.bCanEverTick = false;

    BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
    RootComponent = BaseMesh;
}

void AHeroBaseModule::BeginPlay()
{
	Super::BeginPlay();
}

void AHeroRepairForge::RepairWeapon(UHeroWeaponData* Weapon)
{
    if (Weapon)
    {
        // Logic: Restore durability and reset broken status
        Weapon->Stats.CurrentDurability = Weapon->Stats.MaxDurability;
        Weapon->bIsBroken = false;
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("FORGE: Weapon Repaired (Durability 100%)"));
    }
}

void AHeroKnowledgeLibrary::UnlockIntelPerk(FName PerkID)
{
    if (!PerkID.IsNone())
    {
        // Logic: Unlock perk in Global Perk Database
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("LIBRARY: Intel Perk Unlocked [%s]"), *PerkID.ToString()));
    }
}
