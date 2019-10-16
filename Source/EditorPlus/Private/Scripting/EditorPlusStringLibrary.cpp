#include "Scripting/EditorPlusStringLibrary.h"

FString UEditorPlusStringLibrary::StripPrefix(const FString& String, const FString Prefix, const FString Separator)
{
	auto FirstSeparatorIndex = -1;
	if(!Separator.IsEmpty())
		FirstSeparatorIndex = String.Find(Separator);

	if(Prefix.IsEmpty() && FirstSeparatorIndex >= 0)
		return String.RightChop(FirstSeparatorIndex + 1);

	if(!Prefix.IsEmpty())
	{
		if(FirstSeparatorIndex >= 0 && String.Left(FirstSeparatorIndex).Equals(Prefix, ESearchCase::IgnoreCase))
			return String.RightChop(FirstSeparatorIndex + 1);
		
		if(String.StartsWith(Prefix))
			return String.RightChop(Prefix.Len());
	}
	
	return String;
}

FString UEditorPlusStringLibrary::StripSuffix(const FString& String, const FString Suffix, const FString Separator)
{
	auto LastSeparatorIndex = -1;
	if(!Separator.IsEmpty())
		LastSeparatorIndex = String.Find(Separator, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

	if(Suffix.IsEmpty() && LastSeparatorIndex >= 0)
		return String.LeftChop(LastSeparatorIndex + 1);

	if(!Suffix.IsEmpty())
	{
		if(LastSeparatorIndex >= 0 && String.Left(LastSeparatorIndex).Equals(Suffix, ESearchCase::IgnoreCase))
			return String.LeftChop(LastSeparatorIndex + 1);
		
		if(String.EndsWith(Suffix))
			return String.LeftChop(Suffix.Len());
	}
	
	return String;
}

FString UEditorPlusStringLibrary::AddPrefix(const FString& String, const FString Prefix, const FString Separator)
{
	auto CleanString = String;
	if(CleanString.StartsWith(Separator))
		CleanString = String.RightChop(Separator.Len());
	
	return Prefix + Separator + CleanString;
}

FString UEditorPlusStringLibrary::AddSuffix(const FString& String, const FString Suffix, const FString Separator)
{
	auto CleanString = String;
	if(CleanString.EndsWith(Separator))
		CleanString = String.LeftChop(Separator.Len());
	
	return CleanString + Separator + Suffix;
}