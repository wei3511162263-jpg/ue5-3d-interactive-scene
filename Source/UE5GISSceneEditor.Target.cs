using UnrealBuildTool;
using System.Collections.Generic;

public class UE5GISSceneEditorTarget : TargetRules
{
	public UE5GISSceneEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		ExtraModuleNames.Add("UE5GISScene");
	}
}
