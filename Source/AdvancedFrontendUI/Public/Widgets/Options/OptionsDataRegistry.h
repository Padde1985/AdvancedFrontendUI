#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "OptionsDataRegistry.generated.h"

class UListDataObject_Base;
class UListDataObject_Collection;

UCLASS()
class ADVANCEDFRONTENDUI_API UOptionsDataRegistry : public UObject
{
	GENERATED_BODY()
	
public:
	void InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer);
	const TArray<UListDataObject_Collection*>&  GetRegisteredOptionsTabCollection() const;
	TArray<UListDataObject_Base*> GetListSourceItemsBySelectedTabID(const FName& InSelectedTabID) const;
	
private:
	UPROPERTY(Transient) TArray<UListDataObject_Collection*> RegisteredOptionsTabCollection;
	
	void InitGameplayCollectionTab();
	void InitAudioCollectionTab();
	void InitVideoCollectionTab();
	void InitControllsCollectionTab(ULocalPlayer* InOwningLocalPlayer);
	void FindChildListDataRecursively(const UListDataObject_Base* InParentData, TArray<UListDataObject_Base*>& OutFoundChildListData) const;
};
