using UnrealBuildTool;
using System.Collections.Generic;

public class UE5GISSceneTarget : TargetRules
{
	public UE5GISSceneTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		ExtraModuleNames.Add("UE5GISScene");
	}
}
