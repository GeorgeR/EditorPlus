#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Misc/ScopedSlowTask.h"

#include "EditorPlusTask.generated.h"

UCLASS(BlueprintType, Transient)
class EDITORPLUS_API UEditorPlusTask
    : public UObject
{
	GENERATED_BODY()

protected:
    TSharedPtr<FScopedSlowTask> InnerTask;

public:
    virtual ~UEditorPlusTask();
    void BeginDestroy() override;

public:
    UFUNCTION(BlueprintCallable, Category = "Editor Plus | Task", meta = (WorldContext = "WorldContextObject"))
    static UEditorPlusTask* BeginTask(UObject* WorldContextObject, float AmountOfWork, const FText DefaultMessage = FText(), bool bMakeDialog = true);

    /**
     * Creates a new dialog for this slow task after the given time threshold. If the task completes before this time, no dialog will be shown.
     * @param		Threshold				Time in seconds before dialog will be shown.
     * @param		bShowCancelButton		Whether to show a cancel button on the dialog or not
     * @param		bAllowInPIE				Whether to allow this dialog in PIE. If false, this dialog will not appear during PIE sessions.
     */
    UFUNCTION(BlueprintCallable, Category = "Editor Plus | Task")
    void MakeDialogDelayed(float Threshold, bool bShowCancelButton = false, bool bAllowInPIE = false) const;

    /**
     * Creates a new dialog for this slow task, if there is currently not one open
     * @param		bShowCancelButton		Whether to show a cancel button on the dialog or not
     * @param		bAllowInPIE				Whether to allow this dialog in PIE. If false, this dialog will not appear during PIE sessions.
     */
    UFUNCTION(BlueprintCallable, Category = "Editor Plus | Task")
    void MakeDialog(bool bShowCancelButton = false, bool bAllowInPIE = false) const;

    /**
     * Indicate that we are to enter a frame that will take up the specified amount of work. Completes any previous frames (potentially contributing to parent scopes' progress).
     * @param		ExpectedWorkThisFrame	The amount of work that will happen between now and the next frame, as a numerator of TotalAmountOfWork.
     * @param		Text					Optional text to describe this frame's purpose.
     */
    UFUNCTION(BlueprintCallable, Category = "Editor Plus | Task")
    void EnterProgressFrame(float ExpectedWorkThisFrame = 1.0f, FText Text = FText()) const;

    /**
     * Get the frame message or default message if empty
     */
    UFUNCTION(BlueprintPure, Category = "Editor Plus | Task")
    FText GetCurrentMessage() const;

    /**
     * True if the user has requested that the slow task be canceled
     */
    UFUNCTION(BlueprintPure, Category = "Editor Plus | Task")
    bool ShouldCancel() const;

protected:
    static FText DestroyedTag;
    bool Destroy();
};
