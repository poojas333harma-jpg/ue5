// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroMountComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

UHeroMountComponent::UHeroMountComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHeroMountComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHeroMountComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    UpdateLegIK(DeltaTime);
    UpdateFearInstincts();
}

void UHeroMountComponent::UpdateLegIK(float DeltaTime)
{
    // Quad-Leg Tracing Logic
    FootOffset_FL = FMath::FInterpTo(FootOffset_FL, CalculateFootOffset(TEXT("Foot_FL")), DeltaTime, IKInterpSpeed);
    FootOffset_FR = FMath::FInterpTo(FootOffset_FR, CalculateFootOffset(TEXT("Foot_FR")), DeltaTime, IKInterpSpeed);
    FootOffset_BL = FMath::FInterpTo(FootOffset_BL, CalculateFootOffset(TEXT("Foot_BL")), DeltaTime, IKInterpSpeed);
    FootOffset_BR = FMath::FInterpTo(FootOffset_BR, CalculateFootOffset(TEXT("Foot_BR")), DeltaTime, IKInterpSpeed);
}

float UHeroMountComponent::CalculateFootOffset(FName SocketName)
{
    AActor* Owner = GetOwner();
    ACharacter* Character = Cast<ACharacter>(Owner);
    if (!Character) return 0.f;

    FVector SocketLocation = Character->GetMesh()->GetSocketLocation(SocketName);
    FVector Start = SocketLocation + FVector(0, 0, TraceDistance);
    FVector End = SocketLocation - FVector(0, 0, TraceDistance);

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Owner);

    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    {
        // Calculate offset from current mesh height to hit point
        float IdealZ = Hit.Location.Z;
        float Offset = IdealZ - SocketLocation.Z;
        return FMath::Clamp(Offset, -TraceDistance, TraceDistance);
    }

    return 0.f;
}

void UHeroMountComponent::UpdateFearInstincts()
{
    AActor* Owner = GetOwner();
    if (!Owner) return;

    // 1. Detect Howlers within radius
    TArray<AActor*> OverlappingActors;
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // Setup in implementation
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

    TArray<AActor*> IgnoreActors;
    IgnoreActors.Add(Owner);

    TArray<AActor*> OutActors;
    UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Owner->GetActorLocation(), FearRadius, ObjectTypes, nullptr, IgnoreActors, OutActors);

    for (AActor* Actor : OutActors)
    {
        // Logic: Search for Howler NPC type
        if (Actor && Actor->GetName().Contains(TEXT("Howler")))
        {
            if (!bIsFearing && RearUpMontage)
            {
                ACharacter* Character = Cast<ACharacter>(Owner);
                if (Character)
                {
                    Character->PlayAnimMontage(RearUpMontage);
                    bIsFearing = true;
                    
                    // Reset fear after 5s or distance exit
                    FTimerHandle FearHandle;
                    GetWorld()->GetTimerManager().SetTimer(FearHandle, [this](){ bIsFearing = false; }, 5.f, false);
                }
            }
            break;
        }
    }
}
