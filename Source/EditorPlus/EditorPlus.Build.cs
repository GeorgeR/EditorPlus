using UnrealBuildTool;

public class EditorPlus : ModuleRules
{
	public EditorPlus(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

	    if (Target.Version.MinorVersion <= 19)
	    {
            PublicIncludePaths.AddRange(
			    new string[] {
				    "EditorPlus/Public",
				    "EditorPlus/Public/Validation",
			    });

		    PrivateIncludePaths.AddRange(
			    new string[] {
				    "EditorPlus/Private",
				    "EditorPlus/Private/Validation",
			    });
		}

        PublicDependencyModuleNames.AddRange(
			new string[]
			{
			    "AssetRegistry",
                "Core",				
				"CoreUObject",
				"Engine",
                "Landscape",
			    "LandscapeEditor"
			});
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"EditorStyle",
                "Foliage",
				"GraphEditor",
				"InputCore",
				"Json",
                "JsonUtilities",
                "MaterialEditor",
                "MaterialUtilities",
                "MeshDescription",
				"PropertyEditor",
                "RawMesh",
				"Slate",
				"SlateCore",				
				"UnrealEd"
			});
	}
}
