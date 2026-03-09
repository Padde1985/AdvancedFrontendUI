#pragma once

#include "CoreMinimal.h"
#include "CommonListView.h"
#include "FrontendCommonListView.generated.h"

class UDataAsset_DataListEntryMapping;

// List view class used for the Main menu and/or the different option screens
UCLASS(PrioritizeCategories=("Frontend List View Settings"))
class ADVANCEDFRONTENDUI_API UFrontendCommonListView : public UCommonListView
{
	GENERATED_BODY()
	
protected:
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;
	virtual bool OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem) override;
	
private:
	UPROPERTY(EditAnywhere, Category="Frontend List View Settings") UDataAsset_DataListEntryMapping* DataListEntryMapping;
	
#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const override;
#endif
};
