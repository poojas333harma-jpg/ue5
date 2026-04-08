using UnrealBuildTool;

public class hero : ModuleRules
{
	public hero(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] { 
			"hero/Public",
			"hero/Public/Ledge",
			"hero/Public/AI",
			"hero/Public/MassAI"
		});

		// Runtime — har build mein
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"Mover",
			"MotionWarping",
			"GameplayTags",
			"AnimationLocomotionLibraryRuntime",
			"AnimationWarpingRuntime",
			"PoseSearch",
			"MotionTrajectory",
			"AnimGraphRuntime",
			"Niagara",
			"CableComponent",
			"UMG",
			"Slate",
			"SlateCore",
			"ChaosVehicles",
			"PhysicsCore",
			"GeometryCollectionEngine",
			"Chaos",
			"ChaosSolverEngine",
			"ProceduralMeshComponent",
			"MediaAssets",
			"MediaPlate",
			"AIModule",
			"GameplayTasks",
			"NavigationSystem",
			"MassEntity",
			"MassActors",
			"MassSpawner",
			"MassSignals",
			"MassNavigation",
			"MassRepresentation",
			"MassCommon",
			"MassMovement"
		});

		// Editor-only — sirf Editor build mein
		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(new string[]
			{
				"UnrealEd",
				"AssetRegistry",
				"AssetTools",
				"EditorScriptingUtilities",
				"Blutility",
				"PoseSearchEditor",
				"AnimationLocomotionLibraryEditor"
			});
		}
	}
}