using System.IO;
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

        var EnginePath = Path.GetFullPath(Target.RelativeEnginePath);
        var EditorPath = Path.Combine(EnginePath, "Source", "Editor");

        PrivateIncludePaths.AddRange(
            new string[] {
                Path.Combine(EditorPath, "WorldBrowser", "Private")
            });

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
                "AssetTools",
                "Core",
                "CoreUObject",
				"EditorStyle",
                "EditorSubsystem",
                "Engine",
                "Foliage",
				"GraphEditor",
				"InputCore",
				"Json",
                "JsonUtilities",
                "LevelEditor",
                "MaterialEditor",
                "MaterialUtilities",
                "MeshDescription",
                "Projects",
				"PropertyEditor",
                "RawMesh",
				"Slate",
				"SlateCore",
				"UnrealEd",
                "WorldBrowser"
			});
	}
}
