#include "Widgets/Widget_PrimaryLayout.h"
#include "FrontendDebugHelper.h"

UCommonActivatableWidgetContainerBase* UWidget_PrimaryLayout::FindWidgetStackByTag(const FGameplayTag& InTag) const
{
	checkf(this->RegisteredWidgetStackMap.Contains(InTag), TEXT("Cannot find the widget stack by the tag %s"), *InTag.ToString());
	
	return this->RegisteredWidgetStackMap.FindRef(InTag);
}

void UWidget_PrimaryLayout::RegisterWidgetStack(FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* InStack)
{
	// check if we are running in run time and not just in the unreal editor moving stuff around
	if (!IsDesignTime())
	{
		if (!this->RegisteredWidgetStackMap.Contains(InStackTag))
		{
			this->RegisteredWidgetStackMap.Add(InStackTag, InStack);
		}
	}
}
