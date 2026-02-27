#pragma once

#include "CoreMinimal.h"
#include "ListDataObject_String.h"
#include "ListDataObject_StringResolution.generated.h"

UCLASS()
class ADVANCEDFRONTENDUI_API UListDataObject_StringResolution : public UListDataObject_String
{
	GENERATED_BODY()
	
public:
	void InitResolutionValues();
	FString GetMaximumAllowedResolution() const;
	
protected:
	virtual void OnDataObjectInitialized() override;
	
private:
	FString MaximumAllowedResolution;
	
	FString ResToValueString(const FIntPoint& InResolution) const;
	FText ResToDisplayText(const FIntPoint& InResolution) const;
};
