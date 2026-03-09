#pragma once

#include "CoreMinimal.h"
#include "ListDataObject_String.h"
#include "ListDataObject_StringEnum.generated.h"

// special class for String representation when the given values are enum variables
UCLASS()
class ADVANCEDFRONTENDUI_API UListDataObject_StringEnum : public UListDataObject_String
{
	GENERATED_BODY()
	
public:
	// add the enum variable as option
	template<typename EnumType> void AddEnumOptions(EnumType InEnumOption, const FText& InDisplayText)
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();
		const FString ConvertedEnumString = StaticEnumOption->GetNameStringByValue(InEnumOption);
		
		AddDynamicOption(ConvertedEnumString, InDisplayText);
	}
	
	// creates an enum from the currently displayed text
	template<typename EnumType> EnumType GetCurrentValueAsEnum() const
	{		
		return (EnumType)StaticEnum<EnumType>()->GetValueByNameString(CurrentStringValue);
	}
	
	// set the default value from a given enum variable
	template<typename EnumType> void SetDefaultValueFromEnumOption(EnumType InEnumOption)
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();
		const FString ConvertedEnumString = StaticEnumOption->GetNameStringByValue(InEnumOption);
		
		SetDefaultValueFromString(ConvertedEnumString);
	}
};
