#include "EditorPlusTask.h"

#include "FeedbackContext.h"

FText UEditorPlusTask::DestroyedTag = FText::FromString(TEXT("@Destroyed"));

UEditorPlusTask::~UEditorPlusTask()
{
    if (InnerTask.IsValid())
        Destroy();
}

void UEditorPlusTask::BeginDestroy()
{
    if (InnerTask.IsValid())
        Destroy();

    Super::BeginDestroy();
}

UEditorPlusTask* UEditorPlusTask::BeginTask(
    UObject* WorldContextObject,
    const float AmountOfWork, 
    const FText DefaultMessage,
    const bool bMakeDialog)
{
    ensure(WorldContextObject);

    auto Task = NewObject<UEditorPlusTask>(WorldContextObject);
    //Task->InnerTask = MakeShared<FScopedSlowTask>(MoveTemp(FScopedSlowTask(AmountOfWork, DefaultMessage)));
    Task->InnerTask = MakeShareable(new FScopedSlowTask(AmountOfWork, DefaultMessage));

    if (bMakeDialog)
        Task->MakeDialogDelayed(true, false);

    return Task;
}

void UEditorPlusTask::MakeDialogDelayed(const float Threshold, const bool bShowCancelButton, const bool bAllowInPIE) const
{
    if (!InnerTask)
        return;

    InnerTask->MakeDialogDelayed(Threshold, bShowCancelButton, bAllowInPIE);
}

void UEditorPlusTask::MakeDialog(const bool bShowCancelButton, const bool bAllowInPIE) const
{
    if (!InnerTask)
        return;

    InnerTask->MakeDialog(bShowCancelButton, bAllowInPIE);
}

void UEditorPlusTask::EnterProgressFrame(const float ExpectedWorkThisFrame, const FText Text) const
{
    if (!InnerTask)
        return;

    InnerTask->EnterProgressFrame(ExpectedWorkThisFrame, Text);

    if (InnerTask->CompletedWork + ExpectedWorkThisFrame >= InnerTask->TotalAmountOfWork)
        InnerTask->Destroy();
}

FText UEditorPlusTask::GetCurrentMessage() const
{
    if (!InnerTask)
        return FText();

    return InnerTask->GetCurrentMessage();
}

bool UEditorPlusTask::ShouldCancel() const
{
    if (!InnerTask)
        return false;

    return InnerTask->ShouldCancel();
}

bool UEditorPlusTask::Destroy()
{
    if (!InnerTask)
        return false;

    if (InnerTask->GetCurrentMessage().EqualTo(DestroyedTag))
        return false;

    InnerTask->Destroy();
    InnerTask->DefaultMessage = DestroyedTag;

    return true;
}
