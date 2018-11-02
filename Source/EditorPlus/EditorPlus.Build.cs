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
				"Core",				
				"CoreUObject",
				"Engine",
				"TargetPlatform",
			});
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"AssetRegistry",
				"AssetTools",
				"BlueprintGraph",
				"CollectionManager",
				"ContentBrowser",
				"EditorStyle",
				"GraphEditor",
				"InputCore",
				"Json",
				"KismetCompiler",
				"PropertyEditor",
				"SandboxFile",
				"Slate",
				"SlateCore",				
				"UnrealEd",
				"WorkspaceMenuStructure",
			});
	}
}
