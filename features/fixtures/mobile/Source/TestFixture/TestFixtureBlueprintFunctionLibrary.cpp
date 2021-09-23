#include "TestFixtureBlueprintFunctionLibrary.h"

#include "Scenarios/Scenario.h"

static void ClearPersistentData()
{
	UE_LOG(LogTemp, Display, TEXT("Clearing persistent data"));
#if PLATFORM_ANDROID
	// TODO
#elif PLATFORM_APPLE
	[NSUserDefaults.standardUserDefaults removePersistentDomainForName:NSBundle.mainBundle.bundleIdentifier];
	NSString* AppSupportDir = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES).firstObject;
	NSString* RootDir = [AppSupportDir stringByAppendingPathComponent:@"com.bugsnag.Bugsnag"];
	NSError* Error = nil;
	if (![NSFileManager.defaultManager removeItemAtPath:RootDir error:&Error])
	{
		if (![Error.domain isEqual:NSCocoaErrorDomain] && Error.code != NSFileNoSuchFileError)
		{
			UE_LOG(LogTemp, Error, TEXT("%@"), UTF8_TO_TCHAR(Error.description.UTF8String));
		}
	}
#endif
}

static void Evaluate(const FString& Command)
{
	UE_LOG(LogTemp, Display, TEXT("Evaluating \"%s\""), *Command);

	if (Command.StartsWith(TEXT("Run ")))
	{
		ClearPersistentData();
		Scenario::Run(Command.RightChop(4));
	}

	if (Command.StartsWith(TEXT("Start ")))
	{
		Scenario::Start(Command.RightChop(6));
	}
}

void UTestFixtureBlueprintFunctionLibrary::OnButtonClicked(const FString& String)
{
	UE_LOG(LogTemp, Display, TEXT("UTestFixtureBlueprintFunctionLibrary::OnButtonClicked() %s"), *String);

	Evaluate(String);
}
