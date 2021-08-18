#include "TestFixtureBlueprintFunctionLibrary.h"

static void Evaluate(const FString& String)
{
	if (String == "Crash")
	{
		volatile int* ptr = nullptr;
		*ptr = 42;
	}
}

// We rely on OnTextChanged because OnTextCommitted is not reliably emitted on
// all platforms.

void UTestFixtureBlueprintFunctionLibrary::OnTextChanged(const FText& Text)
{
	FString String = Text.ToString();

	UE_LOG(LogTemp, Display,
		TEXT("UTestFixtureBlueprintFunctionLibrary::OnTextChanged() %s"),
		*String);

	const FString EndOfInput(TEXT("$"));

	if (String.EndsWith(EndOfInput))
	{
		String.RemoveFromEnd(EndOfInput);
		Evaluate(String);
	}
}
