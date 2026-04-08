// Copyright 2026 Hero Project. All Rights Reserved.
// TraversalTrigger.cpp
// ──────────────────────────────────────────────────────
// Master Actor for Traversal Objects.
// Automates the detection and initiation of Ladder, Pipe, Rope, and Squeeze actions.
// ──────────────────────────────────────────────────────

#include "Ledge/TraversalTrigger.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Ledge/LedgeDetectorComponent.h"

ATraversalTrigger::ATraversalTrigger()
{
    PrimaryActorTick.bCanEverTick = false;

    // 1. Scene Root
    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    RootComponent = SceneRoot;

    // 2. Trigger Box (Root component for collision events)
    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetupAttachment(RootComponent);
    TriggerBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    TriggerBox->SetBoxExtent(FVector(100.f, 100.f, 150.f));
    
    // Bind Overlap Events
    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATraversalTrigger::OnOverlapBegin);
    TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ATraversalTrigger::OnOverlapEnd);

    // 3. Spline for Path Traversal
    TraversalSpline = CreateDefaultSubobject<USplineComponent>(TEXT("TraversalSpline"));
    TraversalSpline->SetupAttachment(RootComponent);

    // 4. Visual Meshes (Shaft and Top)
    ShaftMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShaftMesh"));
    ShaftMesh->SetupAttachment(RootComponent);
    ShaftMesh->SetCollisionProfileName(TEXT("NoCollision"));

    TopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TopMesh"));
    TopMesh->SetupAttachment(RootComponent);
    TopMesh->SetCollisionProfileName(TEXT("NoCollision"));
}

void ATraversalTrigger::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    if (ShaftMesh && TopMesh)
    {
        // Default visibility
        TopMesh->SetVisibility(false);
        ShaftMesh->SetVisibility(true);

        if (TriggerType == ELedgeType::Ladder)
        {
            TopMesh->SetVisibility(true);

            // --- AAA FIX for Stretch ---
            // TopMesh ko 1:1 rakho bhale hi actor scale badla ho (Absolute Scale)
            TopMesh->SetUsingAbsoluteScale(true);
            TopMesh->SetWorldScale3D(FVector(1.1f, 1.1f, 0.2f)); // Fixed Platform Size
            TopMesh->SetRelativeLocation(FVector::ZeroVector);

            // Shaft ko Height ke hisaab se scale karo
            // Hum Relative Scale use karenge taaki user height control kar sake
            float VerticalScale = LadderHeight / 100.f; // Base cube 1m ka hai
            ShaftMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, VerticalScale));
            
            // Shaft hamesha origin se neeche start hoga
            ShaftMesh->SetRelativeLocation(FVector(0, 0, -LadderHeight));

            // Adjust Trigger Box
            if (TriggerBox)
            {
                TriggerBox->SetBoxExtent(FVector(60.f, 60.f, LadderHeight * 0.5f));
                TriggerBox->SetRelativeLocation(FVector(0, 0, -LadderHeight * 0.5f));
            }
        }
        else
        {
            // Reset for non-ladder
            ShaftMesh->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
            ShaftMesh->SetRelativeLocation(FVector::ZeroVector);
            TopMesh->SetUsingAbsoluteScale(false);
        }
    }

    if (TraversalSpline)
    {
        TraversalSpline->SetVisibility(bUseSpline);
    }
}

void ATraversalTrigger::BeginPlay()
{
    Super::BeginPlay();
}

void ATraversalTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
                                       bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor) return;

    // Look for Ledge Detector Component on the overlapping Actor
    ULedgeDetectorComponent* Detector = OtherActor->FindComponentByClass<ULedgeDetectorComponent>();
    
    if (Detector)
    {
        // SPECIFIC CASE: Squeeze Gaps
        if (bIsSqueezePoint && TriggerType == ELedgeType::Squeeze_Gap)
        {
            Detector->StartSqueeze(TraversalSpline);
        }

        // Add additional logic here for UI Prompts (e.g. "Press E to Climb")
        // Or store this trigger reference in the detector for precise interaction
    }
}

void ATraversalTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    // Interaction End Logic (e.g. Hide UI Prompt)
}
