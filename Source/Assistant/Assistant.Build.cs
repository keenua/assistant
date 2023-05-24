using System;
using System.Collections.Generic;
using System.IO;

using UnrealBuildTool;

public class Assistant : ModuleRules
{
	public Assistant(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        bEnableUndefinedIdentifierWarnings = false;

		PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "Json",
            "JsonUtilities",
            "Projects"
        });

		PrivateDependencyModuleNames.AddRange(new string[] {
            "CoreUObject",
            "Engine",
            "Slate",
            "SlateCore",
            "Json",
            "JsonUtilities"
        });

        if (Target.Platform == UnrealTargetPlatform.Win64) {
            PublicDefinitions.Add("USE_WINDOWS_DLL_SEMANTICS");
        }
 	}
}
