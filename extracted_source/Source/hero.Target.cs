using UnrealBuildTool;
using System.Collections.Generic;

public class heroTarget : TargetRules
{
	public heroTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("hero");
	}
}