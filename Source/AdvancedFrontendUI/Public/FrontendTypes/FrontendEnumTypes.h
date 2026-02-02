#pragma once

UENUM(BlueprintType)
enum class EConfirmScreenType : uint8
{
	OK,
	YesNo,
	OkCancel,
	Unknown UMETA(Hidden) // Hidden emans only usable in C++ but hidden in Blueprint
};

UENUM(BlueprintType)
enum class EConfirmScreenButtonType : uint8
{
	Confirmed,
	Cancelled,
	Closed,
	Unknown UMETA(Hidden)
};
