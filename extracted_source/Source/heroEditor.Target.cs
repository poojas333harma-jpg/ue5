using UnrealBuildTool;
using System.Collections.Generic;

public class heroEditorTarget : TargetRules
{
	public heroEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("hero");
	}
}