#include "UI/EditorPlusToolbarExtender.h"

#include "AssetToolsModule.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Framework/MultiBox/MultiBoxExtender.h"
#include "Framework/SlateDelegates.h"
#include "LevelEditor.h"

#include "EditorPlusStyle.h"
#include "EditorPlusCommands.h"

#define LOCTEXT_NAMESPACE "EditorPlus"

void FEditorPlusToolbarExtender::Extend()
{
	struct FExtenderLocals
	{
		static TSharedRef<SWidget> HandleShowToolbarMenu()
		{
			FMenuBuilder MenuBuilder(true, FEditorPlusCommands::Get().LevelMenuActionList);

			MenuBuilder.AddMenuEntry(FEditorPlusCommands::Get().FreeMemoryCommand);

			return MenuBuilder.MakeWidget();
		}

		static void ExtendLevelToolbar(FToolBarBuilder& ToolbarBuilder) {
			ToolbarBuilder.AddComboButton(FUIAction(),
				FOnGetContent::CreateStatic(&FExtenderLocals::HandleShowToolbarMenu),
				LOCTEXT("EditorPlus.Toolbar.ButtonText", "Editor Plus"),
				LOCTEXT("EditorPlus.Toolbar.ButtonTooltip", "Show EditorPlus actions"),
				FSlateIcon(FEditorPlusStyle::GetStyleSetName(), TEXT("EditorPlus.ToolbarIcon")));
		}
	};

	auto& LevelEditorModule = FModuleManager::Get().LoadModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));

	LevelToolbarExtender = MakeShareable(new FExtender);
	LevelToolbarExtender->AddToolBarExtension(
		TEXT("Settings"),
		EExtensionHook::After,
		FEditorPlusCommands::Get().LevelMenuActionList,
		FToolBarExtensionDelegate::CreateStatic(&FExtenderLocals::ExtendLevelToolbar)
	);

	LevelEditorModule.GetToolBarExtensibilityManager().Get()->AddExtender(LevelToolbarExtender);
}

void FEditorPlusToolbarExtender::Release()
{
	auto LevelEditorModule = FModuleManager::Get().GetModulePtr<FLevelEditorModule>(TEXT("LevelEditor"));
	if (LevelEditorModule)
	{
		if (LevelEditorModule->GetToolBarExtensibilityManager().IsValid())
			LevelEditorModule->GetToolBarExtensibilityManager().Get()->RemoveExtender(LevelToolbarExtender);
	}
}

#undef LOCTEXT_NAMESPACE