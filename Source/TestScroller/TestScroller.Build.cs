// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TestScroller : ModuleRules
{
	public TestScroller(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "MobileFileOpener" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
		
		/*
		if (Target.Platform == UnrealTargetPlatform.Android || Target.Platform == UnrealTargetPlatform.Win64 ||
		    Target.Platform == UnrealTargetPlatform.Win32)
		{
			PublicAdditionalLibraries.AddRange(new string[] {System.IO.Path.Combine(ModuleDirectory, "ThirdPartyLibraries","PugiXMLLib", "src", "pugixml.lib")});
		}

		if (Target.Platform == UnrealTargetPlatform.IOS)
		{
			PublicAdditionalLibraries.AddRange(new string[] {System.IO.Path.Combine(ModuleDirectory, "ThirdPartyLibraries", "PugiXMLLib", "src", "pugixml.a")});
		}
		
		
		if (Target.Platform == UnrealTargetPlatform.Android || Target.Platform == UnrealTargetPlatform.Win64 ||
		    Target.Platform == UnrealTargetPlatform.Win32)
		{
			PublicAdditionalLibraries.AddRange(new string[] {System.IO.Path.Combine(ModuleDirectory, "ThirdPartyLibraries", "json11", "json11.lib")});
		}
		*/
	}
}
