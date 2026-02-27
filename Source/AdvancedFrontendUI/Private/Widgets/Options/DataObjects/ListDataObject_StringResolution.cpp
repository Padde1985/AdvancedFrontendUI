#include "Widgets/Options/DataObjects/ListDataObject_StringResolution.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"

void UListDataObject_StringResolution::InitResolutionValues()
{
	TArray<FIntPoint> AvailableResolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(AvailableResolutions);
	AvailableResolutions.Sort([](const FIntPoint& A, const FIntPoint& B)
	{
		return A.SizeSquared() < B.SizeSquared();
	});
	
	for (const FIntPoint& Resolution : AvailableResolutions)
	{
		AddDynamicOption(this->ResToValueString(Resolution), this->ResToDisplayText(Resolution));
	}
	
	this->MaximumAllowedResolution = this->ResToValueString(AvailableResolutions.Last());
	
	SetDefaultValueFromString(this->MaximumAllowedResolution);
}

FString UListDataObject_StringResolution::GetMaximumAllowedResolution() const
{
	return this->MaximumAllowedResolution;
}

void UListDataObject_StringResolution::OnDataObjectInitialized()
{
	Super::OnDataObjectInitialized();
	
	if (!TrySetDisplayTextFromStringValue(this->CurrentStringValue))
	{
		this->CurrentDisplayText = this->ResToDisplayText(UFrontendGameUserSettings::Get()->GetScreenResolution());
	}
}

FString UListDataObject_StringResolution::ResToValueString(const FIntPoint& InResolution) const
{
	return FString::Printf(TEXT("(X=%i,Y=%i)"), InResolution.X, InResolution.Y);
}

FText UListDataObject_StringResolution::ResToDisplayText(const FIntPoint& InResolution) const
{	
	return FText::FromString(FString::Printf(TEXT("%i x %i"), InResolution.X, InResolution.Y));
}
