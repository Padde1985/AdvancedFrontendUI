#pragma once

// Confirmation screen types
UENUM(BlueprintType)
enum class EConfirmScreenType : uint8
{
	OK, // a simple screen with just one button that closes the window
	YesNo, // a screen showing a Yes and a No button for user confirmation or warning confirmations
	OkCancel, // OK and cancel button, mainly used for warnings
	Unknown UMETA(Hidden) // Hidden UMETA only usable in C++ but hidden in Blueprint
};

// Button types for confirmation screens
UENUM(BlueprintType)
enum class EConfirmScreenButtonType : uint8
{
	Confirmed, // confirm
	Cancelled, // cancel
	Closed, // close
	Unknown UMETA(Hidden) // not used
};

// Modification reasons for List dependencies
UENUM(BlueprintType)
enum class EOptionsListDataModifyReason : uint8
{
	DirectlyModified, // directly modified
	DependencyModified, // changed due to depdencies , e.g. Screen resolution is changed when window type is changed
	ResetToDefault // Reset to the default value defined in OptionsDataRegistry
};