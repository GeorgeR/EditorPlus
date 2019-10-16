#pragma once

class UWorld;

class FEditorPlusInternalUtilities
{
public:
	static UWorld* GetEditorWorld();
	static UWorld* GetEditorWorldChecked();
};