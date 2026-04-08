// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroGrappleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CableComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UHeroGrappleComponent::UHeroGrappleComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    
    CableVisual = CreateDefaultSubobject<UCableComponent>(TEXT("CableVisual"));
    // Note: Attached to Root in BeginPlay as 'this' is not a SceneComponent
    CableVisual->SetVisibility(false);
}

void UHeroGrappleComponent::BeginPlay()
{
    Super::BeginPlay();
    if (GetOwner() && GetOwner()->GetRootComponent())
    {
        CableVisual->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    }
}

bool UHeroGrappleComponent::FindGrapplePoint(FVector& OutTarget)
{
    AActor* Owner = GetOwner();
    ACharacter* Char = Cast<ACharacter>(Owner);
    if (!Char) return false;

    FVector Start = Char->GetActorLocation();
    FVector Direction = Char->GetViewRotation().Vector();
    FVector End = Start + (Direction * MaxGrappleDistance);

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Owner);

    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    {
        OutTarget = Hit.ImpactPoint;
        return true;
    }
    return false;
}

void UHeroGrappleComponent::FireGrapple()
{
    if (FindGrapplePoint(GrappleTargetPoint))
    {
        bIsGrappling = true;
        CableVisual->SetVisibility(true);
        CableVisual->EndLocation = FVector::ZeroVector; // Attached to target via logic or socket
        
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Orange, TEXT("GRAPPLE FIRED!"));
    }
}

void UHeroGrappleComponent::StopGrapple()
{
    bIsGrappling = false;
    CableVisual->SetVisibility(false);
}

void UHeroGrappleComponent::HandleGrappleForce(float DeltaTime)
{
    if (!bIsGrappling) return;

    ACharacter* Char = Cast<ACharacter>(GetOwner());
    if (!Char) return;

    FVector CurrentPos = Char->GetActorLocation();
    FVector Direction = (GrappleTargetPoint - CurrentPos).GetSafeNormal();
    float Distance = FVector::Dist(CurrentPos, GrappleTargetPoint);

    if (Distance < 150.f)
    {
        StopGrapple();
        return;
    }

    // Direct Velocity Push (Dying Light Style)
    Char->GetCharacterMovement()->Velocity += Direction * PullForce * DeltaTime;
    
    // Update Cable Visual
    CableVisual->EndLocation = Char->GetTransform().InverseTransformPosition(GrappleTargetPoint);
}
