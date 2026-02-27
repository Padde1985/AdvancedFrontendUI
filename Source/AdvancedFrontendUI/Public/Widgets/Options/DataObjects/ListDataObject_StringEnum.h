#pragma once

#include "CoreMinimal.h"
#include "ListDataObject_String.h"
#include "ListDataObject_StringEnum.generated.h"

UCLASS()
class ADVANCEDFRONTENDUI_API UListDataObject_StringEnum : public UListDataObject_String
{
	GENERATED_BODY()
	
public:
	template<typename EnumType> void AddEnumOptions(EnumType InEnumOption, const FText& InDisplayText)
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();
		const FString ConvertedEnumString = StaticEnumOption->GetNameStringByValue(InEnumOption);
		
		AddDynamicOption(ConvertedEnumString, InDisplayText);
	}
	
	template<typename EnumType> EnumType GetCurrentValueAsEnum() const
	{		
		return (EnumType)StaticEnum<EnumType>()->GetValueByNameString(CurrentStringValue);
	}
	
	template<typename EnumType> void SetDefaultValueFromEnumOption(EnumType InEnumOption)
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();
		const FString ConvertedEnumString = StaticEnumOption->GetNameStringByValue(InEnumOption);
		
		SetDefaultValueFromString(ConvertedEnumString);
	}
};
