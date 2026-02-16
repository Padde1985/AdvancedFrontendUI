#pragma once

#include "CoreMinimal.h"
#include "CommonListView.h"
#include "FrontendCommonListView.generated.h"

class UDataAsset_DataListEntryMapping;

UCLASS(PrioritizeCategories=("Frontend List View Settings"))
class ADVANCEDFRONTENDUI_API UFrontendCommonListView : public UCommonListView
{
	GENERATED_BODY()
	
protected:
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;
	
private:
	UPROPERTY(EditAnywhere, Category="Frontend List View Settings") UDataAsset_DataListEntryMapping* DataListEntryMapping;
	
#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const override;
#endif
};
