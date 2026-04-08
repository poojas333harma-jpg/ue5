// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroElementalManager.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Pawn.h"
#include "Ledge/HeroNPCBase.h"

void UHeroElementalManager::ExecuteElectricalDischargeInWater(FVector Location, float Radius)
{
    // 1. Define the Radial Trace parameters
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
    
    TArray<AActor*> IgnoreActors;
    TArray<AActor*> OutActors;

    // 2. Perform the Radial Trace (6m - 8m)
    bool bHit = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Location, Radius, ObjectTypes, nullptr, IgnoreActors, OutActors);

    if (bHit)
    {
        for (AActor* Actor : OutActors)
        {
            if (APawn* Pawn = Cast<APawn>(Actor))
            {
                // Check if the Pawn is touching water (Tag: Water)
                if (Pawn->ActorHasTag("Water") || Pawn->GetRootComponent()->ComponentHasTag("Water"))
                {
                    ApplyStunAndDamage(Pawn, 4.0f);
                    
                    // 3. Chain Reaction Logic: Jumps to nearby partners
                    HandleChainReaction(Pawn->GetActorLocation(), OutActors);
                }
            }
        }
    }

    // 4. Spawn NIAGARA Arcs (VFX)
    // NS_WaterElectricityArc should be assigned via a DataAsset in a real implementation
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), nullptr, Location, FRotator::ZeroRotator, FVector(Radius / 100.f));
    
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Elemental: WATER ELECTRIFIED!"));
}

void UHeroElementalManager::SpreadFire(FVector Location, float PropagationRadius)
{
    // Implementation: Sphere trace for Flammable actors, trigger their CatchFire()
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Elemental: FIRE SPREADING..."));
}

void UHeroElementalManager::ApplyStunAndDamage(AActor* Target, float Duration)
{
    if (AHeroNPCBase* NPC = Cast<AHeroNPCBase>(Target))
    {
        // Stun logic: Blocks AI movement and triggers 'Stun' montage
        // NPC->ReceiveElementalEffect(EHeroElementalType::Electric, Duration);
        
        UGameplayStatics::ApplyDamage(Target, 10.f, nullptr, nullptr, UDamageType::StaticClass());
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("Stunned: %s"), *Target->GetName()));
    }
}

void UHeroElementalManager::HandleChainReaction(FVector SourceLocation, TArray<AActor*> IgnoredActors)
{
    // Look for pawns within 2 meters (Jump Range)
    TArray<AActor*> Nearby;
    UKismetSystemLibrary::SphereOverlapActors(GetWorld(), SourceLocation, 200.f, TArray<TEnumAsByte<EObjectTypeQuery>>(), nullptr, IgnoredActors, Nearby);

    for (AActor* Actor : Nearby)
    {
        ApplyStunAndDamage(Actor, 2.0f); // Shorter jump stun
    }
}
