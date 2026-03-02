#include "Widgets/Options/FKeyRemapScreenInputPreprocessor.h"

#include "CommonInputSubsystem.h"
#include "CommonUITypes.h"
#include "ICommonInputModule.h"

FKeyRemapScreenInputPreprocessor::FKeyRemapScreenInputPreprocessor(ECommonInputType InInputTypeToListenTo, ULocalPlayer* InOwningLocalPlayer) 
	: CachedInputTypeToListenTo(InInputTypeToListenTo), CachedOwningLocalPlayer(InOwningLocalPlayer)
{
}

void FKeyRemapScreenInputPreprocessor::Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor)
{
}

bool FKeyRemapScreenInputPreprocessor::HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent)
{
	this->ProcessPressedKey(InKeyEvent.GetKey());
	
	return true;
}

bool FKeyRemapScreenInputPreprocessor::HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	this->ProcessPressedKey(MouseEvent.GetEffectingButton());
	
	return true;
}

void FKeyRemapScreenInputPreprocessor::ProcessPressedKey(const FKey& InPressedKey)
{
	if (InPressedKey == EKeys::Escape)
	{
		this->OnInputPreProcessorKeySelectCanceled.ExecuteIfBound(TEXT("Key Remap has been canceled"));
		
		return;
	}
	
	UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(this->CachedOwningLocalPlayer.Get());
	
	check(CommonInputSubsystem);
	
	ECommonInputType CurrentInputType = CommonInputSubsystem->GetCurrentInputType();

	switch (this->CachedInputTypeToListenTo)
	{
	case ECommonInputType::MouseAndKeyboard:
		if (InPressedKey.IsGamepadKey() || CurrentInputType == ECommonInputType::Gamepad)
		{
			this->OnInputPreProcessorKeySelectCanceled.ExecuteIfBound(TEXT("Detected Gamepad key pressed for keyboard inputs. Key Remap has been canceled"));
			
			return;
		}
		break;
	case ECommonInputType::Gamepad:
		if (CurrentInputType == ECommonInputType::Gamepad && InPressedKey == EKeys::LeftMouseButton)
		{
			FCommonInputActionDataBase* InputActionData = ICommonInputModule::GetSettings().GetDefaultClickAction().GetRow<FCommonInputActionDataBase>(TEXT(""));
			check(InputActionData);
			
			this->OnInputPreProcessorKeyPressedD.ExecuteIfBound(InputActionData->GetDefaultGamepadInputTypeInfo().GetKey());
			
			return;
		}
		
		if (!InPressedKey.IsGamepadKey())
		{
			this->OnInputPreProcessorKeySelectCanceled.ExecuteIfBound(TEXT("Detected non Gamepad key pressed for Gamepad inputs. Key Remap has been canceled"));
			
			return;
		}
		break;
	default:
		break;
	}
	
	this->OnInputPreProcessorKeyPressedD.ExecuteIfBound(InPressedKey);
}
