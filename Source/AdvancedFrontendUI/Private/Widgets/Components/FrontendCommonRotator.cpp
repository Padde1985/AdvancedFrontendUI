#include "Widgets/Components/FrontendCommonRotator.h"
#include "CommonTextBlock.h"

// find the item index by the selected text since we are not displaying the internal ID
void UFrontendCommonRotator::SetSelectedOptionByText(const FText& InTextOption)
{
	const int32 FoundIndex = TextLabels.IndexOfByPredicate([InTextOption](const FText& TextItem)
	{
		return TextItem.EqualTo(InTextOption);
	});
	
	if (FoundIndex != INDEX_NONE)
	{
		SetSelectedItem(FoundIndex);
	}
	else
	{
		MyText->SetText(InTextOption);
	}
}
