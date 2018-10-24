using UnrealBuildTool;

public class EditorPlus : ModuleRules
{
	public EditorPlus(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

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
