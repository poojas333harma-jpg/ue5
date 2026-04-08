// Copyright 2026 Hero Project. All Rights Reserved.
// LedgeDetectorComponent.h
// ──────────────────────────────────────────────────────

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/EngineTypes.h"
#include "LedgeDataTypes.h"
#include "HeroCoreTypes.h"
#include "LedgeMontageSet.h" 
#include "LedgeDetectorComponent.generated.h"

class UPrimitiveComponent;
class USplineComponent;

/** 
 * FULL AAA TRAVERSAL PARAMETERS
 */
USTRUCT(BlueprintType)
struct FLedgeTraceInputs
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge|Trace")
    float TraceForwardDistance = 75.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge|Trace")
    float TraceRadius = 30.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge|Trace")
    float TraceHalfHeight = 90.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge|Trace")
    FVector TraceForwardDirection = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge|Trace")
    FVector TraceOriginOffset = FVector(0.f, 0.f, 20.f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge|Trace")
    float TraceMinGrabHeight = 40.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge|Trace")
    float TraceMaxGrabHeight = 250.f;
};

USTRUCT(BlueprintType)
struct FLedgeCheckResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Ledge|Front")
    bool HasFrontLedge = false;

    UPROPERTY(BlueprintReadWrite, Category = "Ledge|Front")
    FVector FrontLedgeLocation = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, Category = "Ledge|Front")
    FVector FrontLedgeNormal = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, Category = "Ledge|Obstacle")
    float ObstacleHeight = 0.f;

    UPROPERTY(BlueprintReadWrite, Category = "Ledge|Obstacle")
    float ObstacleDepth = 0.f;

    UPROPERTY(BlueprintReadWrite, Category = "Ledge|Obstacle")
    TObjectPtr<UPrimitiveComponent> HitComponent = nullptr;

    UPROPERTY(BlueprintReadWrite, Category = "Ledge|Action")
    uint8 ActionType = 0;
};

UCLASS(ClassGroup=(Hero), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable, DisplayName="Ledge Detector")
class HERO_API ULedgeDetectorComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    ULedgeDetectorComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    /** Master logic node returning randomized montages from the 80+ set. */
    UFUNCTION(BlueprintCallable, Category = "Ledge|Master")
    FLedgeExecutionResult ExecuteLedgeLogic(FLedgeTraceInputs Inputs, ULedgeMontageSet* MontageSet);

    /** The TRUE One-Node Solution for character interaction. */
    UFUNCTION(BlueprintCallable, Category = "Ledge|Master")
    bool TryPlayLedgeInteraction(
        FLedgeTraceInputs Inputs, 
        ULedgeMontageSet* MontageSet, 
        float InputForward, 
        float InputRight, 
        bool bJumpPressed, 
        bool bIsHanging,
        ELedgeType CurrentLedgeType
    );

    /**
     * ═══ THE SINGLE MAGIC NODE (First Class) ═══
     * Call this on Jump/Interact input. It will:
     * 1. Detect the wall/ledge in front
     * 2. Pick the correct montage (Hang, Vault, Climb)
     * 3. Play it automatically on the character
     * Returns true if a ledge action was triggered.
     */
    UFUNCTION(BlueprintCallable, Category = "Ledge|Master")
    bool TryGrabLedge(FLedgeTraceInputs Inputs);

    /** True when character is currently hanging on a ledge */
    UPROPERTY(BlueprintReadWrite, Category = "Ledge|State")
    bool bIsHanging = false;

    /** Current type of surface being interacted with */
    UPROPERTY(BlueprintReadOnly, Category = "Ledge|State")
    ELedgeType CurrentHangType = ELedgeType::Wall;

    /** Saved hang position for Tick-based lock (Mover 2.0 compatible) */
    FVector CurrentHangTargetLocation = FVector::ZeroVector;
    FRotator CurrentHangTargetRotation = FRotator::ZeroRotator;
    bool bHangPositionValid = false;

    /** Wall state saved during grab — used by Shimmy/ClimbUp/Corner */
    UPROPERTY(BlueprintReadOnly, Category = "Ledge|State")
    FVector CurrentHangWallNormal = FVector::ZeroVector;

    UPROPERTY(BlueprintReadOnly, Category = "Ledge|State")
    FVector CurrentHangLedgeTop = FVector::ZeroVector;

    /**
     * Call this when player presses Jump/Drop while hanging.
     * Stops the hang montage and restores movement.
     */
    UFUNCTION(BlueprintCallable, Category = "Ledge|Actions")
    void ReleaseHang();

    UFUNCTION(BlueprintCallable, Category = "Ledge|Actions")
    bool JumpEject();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge|Detection")
    float MaxHangTime = 8.f;

    /** Yaw offset to fix character facing direction (e.g. set to -90 or 180 if character faces sideways/away) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge|Detection")
    float MeshRotationOffset = 0.f;

    // ═══════════════════════════════════════════════════
    // CLIMB UP — W (Forward) while hanging → stand on top
    // ═══════════════════════════════════════════════════
    /** Call from Blueprint when Forward input (W) is pressed while bIsHanging=true */
    UFUNCTION(BlueprintCallable, Category = "Ledge|Actions")
    bool TryClimbUp();

    // ═══════════════════════════════════════════════════
    // SHIMMY — A/D while hanging → slide left/right
    // ═══════════════════════════════════════════════════
    /** Call with -1.0 for Left (A), +1.0 for Right (D) */
    UFUNCTION(BlueprintCallable, Category = "Ledge|Actions")
    bool TryShimmy(float Direction);

    /** How far character moves per shimmy step (in cm) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge|Shimmy")
    float ShimmyStepDistance = 60.f;

    /** True when a shimmy/climb action montage is playing (blocks new inputs) */
    UPROPERTY(BlueprintReadOnly, Category = "Ledge|State")
    bool bIsPerformingAction = false;

    // ═══════════════════════════════════════════════════
    // CORNER WRAP — auto-detected during shimmy
    // ═══════════════════════════════════════════════════
    /** Call to attempt wrapping around a wall corner while hanging */
    UFUNCTION(BlueprintCallable, Category = "Ledge|Actions")
    bool TryCornerWrap(float Direction);

private:
    /** Time elapsed since hang began (for MaxHangTime check) */
    float HangElapsedTime = 0.f;

    /** Cooldown timer — prevents re-grab spam after release */
    float LastGrabTime = -10.f;

    /** True when ClimbUp montage is playing — OnMontageEnded will release to standing */
    bool bIsClimbingUp = false;

    /** Rising-edge input tracking — prevents key-held spam */
    bool bWasWDown = false;
    bool bWasADown = false;
    bool bWasDDown = false;
    bool bWasSDown = false;
    bool bWasCDown = false;
    bool bWasSpaceDown = false;

    /** Tracks if W was already being held at the moment of grab */
    bool bWasInJumpW = false;

    /** Internal release — stops montage, restores gravity & movement */
    void InternalReleaseHang();

    /** Montage end delegate — handles loop transition, climb-up finish, etc. */
    UFUNCTION()
    void OnHangMontageEnded(UAnimMontage* Montage, bool bInterrupted);

    /** Montage end for action montages (shimmy/corner) — returns to loop */
    UFUNCTION()
    void OnActionMontageEnded(UAnimMontage* Montage, bool bInterrupted);

public:

    /** Global DataAsset containing all 80+ AAA animations. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge|Data")
    TObjectPtr<ULedgeMontageSet> TraversalDataAsset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Overlay")
    class UHeroOverlayData* CurrentOverlay;

    UFUNCTION(BlueprintCallable, Category = "Hero|Overlay")
    void SetOverlayState(EHeroOverlayState NewState);

    UFUNCTION(BlueprintCallable, Category = "Ledge")
    bool TryDetectLedge(const FLedgeTraceInputs& Inputs, FLedgeCheckResult& OutResult);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge|Detection")
    float MinGrabHeight = 50.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge|Detection")
    float MaxGrabHeight = 250.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge|Detection")
    float VaultHeightThreshold = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge|Detection")
    TEnumAsByte<ECollisionChannel> TraceChannel = ECC_WorldStatic;

    // ── STAMINA SYSTEM (Dying Light 2 Style) ────────────────
    UFUNCTION(BlueprintCallable, Category = "Traversal|Stamina")
    void UpdateStamina(float DeltaTime);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal|Stamina")
    float CurrentStamina = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal|Stamina")
    float MaxStamina = 5000.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal|Stamina")
    float StaminaDrainRate = 10.f;  // per second — 5000/10 = 500 sec hang time

    /** Enable/Disable stamina drain while hanging */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal|Stamina")
    bool bStaminaDrainEnabled = true;

    // ── LANDING SYSTEM ─────────────────────────────
    UFUNCTION(BlueprintCallable, Category = "Traversal|Landing")
    bool CheckFallImpact(float FallVelocityZ, ULedgeMontageSet* TraversalSet);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal|Landing")
    float RollVelocityThreshold = -1000.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal|Landing")
    float StaggerVelocityThreshold = -1500.f;

    // ── REACH & PREDICTION ─────────────────────────
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge|Reach")
    float ReachDistance = 150.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge|Prediction")
    float PredictionInterval = 0.2f;

    UPROPERTY(BlueprintReadOnly, Category = "Ledge|Reach")
    bool bIsReaching = false;

    UPROPERTY(BlueprintReadOnly, Category = "Ledge|Prediction")
    bool bCanJumpBack = false;

    UPROPERTY(BlueprintReadOnly, Category = "Ledge|Prediction")
    bool bCanJumpLeft = false;

    UPROPERTY(BlueprintReadOnly, Category = "Ledge|Prediction")
    bool bCanJumpRight = false;

    // ── DEBUG ──────────────────────────────────────
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge|Debug")
    bool bShowDebugTraces = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ledge|Debug")
    float DebugDrawDuration = 2.f;

    // ── BALANCE SYSTEM ─────────────────────────────
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal|Balance")
    float CurrentStability = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal|Balance")
    float StabilityDrainRate = 15.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal|Balance")
    float StabilitySkillMultiplier = 1.0f;

    UFUNCTION(BlueprintCallable, Category = "Traversal|Balance")
    void UpdateBalance(float DeltaTime, float MoveInputX);

    UFUNCTION(BlueprintCallable, Category = "Traversal|Balance")
    bool CheckGroundStability(FName BeamTag = FName("ThinBeam"));

    // ── PARKOUR SYSTEM ─────────────────────────────
    UFUNCTION(BlueprintCallable, Category = "Traversal|Parkour")
    bool TryStartSlide();

    UFUNCTION(BlueprintCallable, Category = "Traversal|Parkour")
    void UpdateSliding(float DeltaTime);

    UFUNCTION(BlueprintCallable, Category = "Traversal|Parkour")
    bool TryMantle(ULedgeMontageSet* TraversalSet);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal|Parkour")
    float SlideEndSpeedThreshold = 200.f;

    UPROPERTY(BlueprintReadOnly, Category = "Traversal|Parkour")
    bool bIsSliding = false;

    // ── SQUEEZE SYSTEM ─────────────────────────────
    UPROPERTY(BlueprintReadWrite, Category = "Traversal|Squeeze")
    TObjectPtr<USplineComponent> ActiveSqueezeSpline = nullptr;

    UFUNCTION(BlueprintCallable, Category = "Traversal|Squeeze")
    void StartSqueeze(USplineComponent* TargetSpline);

    UFUNCTION(BlueprintCallable, Category = "Traversal|Squeeze")
    void UpdateSqueeze(float DeltaTime, float MoveInput);

    UPROPERTY(BlueprintReadOnly, Category = "Traversal|Squeeze")
    bool bIsSqueezing = false;

    // ── LADDER SYSTEM ──────────────────────────────
    UPROPERTY(BlueprintReadWrite, Category = "Traversal|Ladder")
    FVector ActiveLadderExitPoint = FVector::ZeroVector;

    // ── POLISH ─────────────────────────────────────
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal|Polish")
    TSubclassOf<class UCameraShakeBase> BalanceCameraShake;

private:
    bool TraceForwardWall(const FVector& Origin, const FVector& Forward, float Distance, float Radius, FHitResult& OutHit) const;
    bool TraceWallTop(const FVector& WallHitLocation, const FVector& Forward, float MaxHeight, FHitResult& OutLedgeHit) const;
    void HandleMidAirReach(float DeltaTime);
    void UpdatePredictiveCheck(float DeltaTime);
    
    bool CheckForCorner(bool bCheckLeft, FVector& OutCornerLocation, FRotator& OutCornerRotation);
    bool TraceBackClearance(const FVector& LedgeTopLocation, const FVector& Forward, float CapsuleHalfHeight, float Radius, FVector& OutBackFloor, FVector& OutBackLedge) const;

    float PredictionTimer = 0.f;
};
