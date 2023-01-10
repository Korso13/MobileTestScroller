// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MobileFileOpener : ModuleRules
{
	public MobileFileOpener(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

		if (Target.Platform == UnrealTargetPlatform.Android)
		{
			PrivateDependencyModuleNames.AddRange(new string[] { "Launch", "ApplicationCore" });
			
			PrivateIncludePaths.AddRange(new string[] { "D:/UE_4.27/Engine/Source/Runtime/Launch/Private"});

			string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
			
			AdditionalPropertiesForReceipt.Add("FileOpener", System.IO.Path.Combine(PluginPath, "MobileFileOpener_APL.xml"));
			//AdditionalPropertiesForReceipt.Add(new ReceiptProperty("MobileFileOpener", System.IO.Path.Combine(pluginPath, "MobileFileOpener_APL.xml")));
		}
	}
}
