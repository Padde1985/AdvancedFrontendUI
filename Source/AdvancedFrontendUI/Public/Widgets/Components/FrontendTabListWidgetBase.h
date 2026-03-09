#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "FrontendTabListWidgetBase.generated.h"

class UFrontendCommonButtonBase;

// tab class for the options menu headers
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick), PrioritizeCategories=("Frontend Tab List Settings"))
class ADVANCEDFRONTENDUI_API UFrontendTabListWidgetBase : public UCommonTabListWidgetBase
{
	GENERATED_BODY()
	
public:
	void RequestRegisterTab(const FName& InTabID, const FText& InTabDisplayName);
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Tab List Settings", meta=(AllowPrivateAccess = "true", ClampMin = "1", ClampMax = "10")) int32 DebugEditorPreviewTabCount = 3;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Tab List Settings", meta=(AllowPrivateAccess = "true")) TSubclassOf<UFrontendCommonButtonBase> TabButtonEntryWidgetClass;
	
#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
};
