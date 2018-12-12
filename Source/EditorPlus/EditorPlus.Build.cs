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
			    "LandscapeEditor",
                "TargetPlatform",
			});
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"AssetTools",
				"BlueprintGraph",
				"CollectionManager",
				"ContentBrowser",
				"EditorStyle",
                "Foliage",
				"GraphEditor",
				"InputCore",
				"Json",
                "JsonUtilities",
				"KismetCompiler",
                "MainFrame",
                "MaterialEditor",
                "MaterialUtilities",
                "MeshDescription",
                "MeshDescriptionOperations",
				"PropertyEditor",
                "RawMesh",
				"SandboxFile",
				"Slate",
				"SlateCore",				
				"UnrealEd",
				"WorkspaceMenuStructure",
			});
	}
}
