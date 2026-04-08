// Copyright 2026 Hero Project. All Rights Reserved.
// TraversalTrigger.h
// ──────────────────────────────────────────────────────
// Master Actor for All Traversal Triggers (Ladder, Pipe, Rope, Squeeze).
// BP mein components (Box, Spline, Mesh) pehle se hi jude honge!
// ──────────────────────────────────────────────────────

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LedgeDataTypes.h"
#include "TraversalTrigger.generated.h"

class UBoxComponent;
class USplineComponent;
class UStaticMeshComponent;

UCLASS(Blueprintable)
class HERO_API ATraversalTrigger : public AActor
{
    GENERATED_BODY()

public:
    ATraversalTrigger();

protected:
    virtual void BeginPlay() override;
    virtual void OnConstruction(const FTransform& Transform) override;

public:
    // ═══════════════════════════════════════════════
    // COMPONENTS (Pre-fabricated in C++)
    // ═══════════════════════════════════════════════

    // Root Scene Component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Traversal|Components")
    USceneComponent* SceneRoot;

    // Trigger Area (Box)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Traversal|Components")
    UBoxComponent* TriggerBox;

    // Spline for Squeeze / Rope / Zipline paths
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Traversal|Components")
    USplineComponent* TraversalSpline;

    // Visual Mesh: Shaft (Scales downwards)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Traversal|Components")
    UStaticMeshComponent* ShaftMesh;

    // Visual Mesh: Top (Fixed at origin, marks exit)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Traversal|Components")
    UStaticMeshComponent* TopMesh;

    // ═══════════════════════════════════════════════
    // SETTINGS (Details Panel)
    // ═══════════════════════════════════════════════

    // What kind of traversal is this? (Ladder, Pipe, Rope, Squeeze)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal|Settings")
    ELedgeType TriggerType = ELedgeType::Ledge_None;

    // AAA POLISH: If true, this trigger will auto-start a Squeeze action
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal|Settings")
    bool bIsSqueezePoint = false;

    // LADDER SETTINGS
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal|Settings", meta = (EditCondition = "TriggerType == ELedgeType::Ladder"))
    float LadderHeight = 300.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal|Settings", meta = (EditCondition = "TriggerType == ELedgeType::Ladder"))
    FVector ExitPointOffset = FVector(0, 0, 0);

    // Should we use the spline for movement? (True for Squeeze/Rope)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal|Settings")
    bool bUseSpline = true;

    // ═══════════════════════════════════════════════
    // OVERLAP LOGIC
    // ═══════════════════════════════════════════════

    UFUNCTION()
    virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
                                bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
