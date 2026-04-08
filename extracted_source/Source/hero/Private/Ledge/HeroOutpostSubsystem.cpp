// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroOutpostSubsystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Ledge/HeroNPCBase.h"

FVector UHeroOutpostSubsystem::SnapLocationToGrid(FVector Location, float GridSize)
{
    float SnappedX = FMath::GridSnap(Location.X, GridSize);
    float SnappedY = FMath::GridSnap(Location.Y, GridSize);
    float SnappedZ = Location.Z; // Usually keep Z for terrain height unless inside a structure

    return FVector(SnappedX, SnappedY, SnappedZ);
}

void UHeroOutpostSubsystem::SetOutpostState(FName OutpostID, EHeroOutpostState NewState)
{
    if (!OutpostStates.Contains(OutpostID) || OutpostStates[OutpostID] != NewState)
    {
        OutpostStates.Add(OutpostID, NewState);
        OnOutpostStateChanged.Broadcast(OutpostID, NewState);
        
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Outpost %s state changed to: %d"), *OutpostID.ToString(), (int32)NewState));
    }
}

EHeroOutpostState UHeroOutpostSubsystem::GetOutpostState(FName OutpostID) const
{
    if (OutpostStates.Contains(OutpostID))
    {
        return OutpostStates[OutpostID];
    }
    return EHeroOutpostState::Occupied;
}

void UHeroOutpostSubsystem::RegisterFurniture(FName OutpostID, AActor* Furniture)
{
    if (!Furniture) return;

    FVector GridSnappedLoc = SnapLocationToGrid(Furniture->GetActorLocation(), 50.f);
    Furniture->SetActorLocation(GridSnappedLoc);

    // Save Logic: Store in a persistent map that syncs with HeroSaveGame
    FHeroOutpostFurniture Data;
    Data.FurnitureClass = Furniture->GetClass();
    Data.Transform = Furniture->GetTransform();
    Data.Transform.SetLocation(GridSnappedLoc);
    
    FurnitureData.FindOrAdd(OutpostID).Items.Add(Data);
    
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("OUTPOST: Furniture Snapped to 50cm Grid and Saved."));
}

void UHeroOutpostSubsystem::AssignGuard(FName OutpostID, class AHeroNPCBase* GuardNPC, FVector PostLocation)
{
    if (!GuardNPC) return;

    FVector SnappedPost = SnapLocationToGrid(PostLocation, 50.f);
    
    // Logic: Tell NPC to move to and stay at this post
    // GuardNPC->SetGuardPost(SnappedPost);

    FHeroGuardAssignment Assignment;
    Assignment.NPCID = GuardNPC->NPCProfile;
    Assignment.PostLocation = SnappedPost;
    
    GuardAssignments.FindOrAdd(OutpostID).Assignments.Add(Assignment);
}
