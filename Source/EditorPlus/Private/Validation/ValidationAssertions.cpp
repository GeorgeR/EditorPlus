#include "ValidationAssertions.h"
#include <UObjectGlobals.h>
#include <NoExportTypes.h>

using namespace Validation;

#define UE_LOG_ERROR_CATEGORY(Category, Message) \
{ \
	UE_LOG_EXPAND_IS_FATAL(Error, PREPROCESSOR_NOTHING, if (!Category.IsSuppressed(ELogVerbosity::Error))) \
	{ \
		FMsg::Logf_Internal(__FILE__, __LINE__, Category.GetCategoryName(), ELogVerbosity::Error, Message); \
		UE_LOG_EXPAND_IS_FATAL(Error, \
			{ \
				_DebugBreakAndPromptForRemote(); \
				FDebug::AssertFailed("", __FILE__, __LINE__, Message); \
				CA_ASSUME(false); \
			}, \
			PREPROCESSOR_NOTHING \
		) \
	} \
}

bool FAssert::That(bool Condition, TArray<FText>& ValidationErrors)
{
    FText Message = FText::GetEmpty();
    return That(Condition, Message, ValidationErrors);
}

bool FAssert::That(bool Condition, FText& Message, TArray<FText>& ValidationErrors)
{
    if (Condition) return true;

    if (Message.IsEmpty())
        Message = FText::FromString(TEXT("The input condition failed."));

    ValidationErrors.Add(Message);

    return false;
}

bool Validation::FAssert::IsDataValid(UObject* Object, TArray<FText>& ValidationErrors)
{
    bool bIsValid = IsNotNull(Object, TEXT("Unspecified"), ValidationErrors);
    if (!bIsValid)
        return false;

    return Object->IsDataValid(ValidationErrors) != EDataValidationResult::Invalid;
}

bool Validation::FUtilities::PrintErrors(const struct FLogCategoryBase& LogCategory, TArray<FText>& ValidationErrors)
{
    if (ValidationErrors.Num() == 0)
        return false;

    for (auto i = 0; i < ValidationErrors.Num(); i++)
    {
        /*UE_LOG_ERROR_CATEGORY(LogCategory, *ValidationErrors[i].ToString());*/
    }
    
    return true;
}
