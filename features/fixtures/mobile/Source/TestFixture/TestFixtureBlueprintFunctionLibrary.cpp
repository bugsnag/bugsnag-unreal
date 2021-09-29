#include "TestFixtureBlueprintFunctionLibrary.h"

#include "ScenarioNames.h"
#include "Scenarios/Scenario.h"

#if PLATFORM_ANDROID
#include "Android/AndroidJavaEnv.h"
#endif

static void ClearPersistentData()
{
	UE_LOG(LogTemp, Display, TEXT("Clearing persistent data"));
#if PLATFORM_ANDROID
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);
	jclass ActivityClass = AndroidJavaEnv::FindJavaClass("com/epicgames/ue4/GameActivity");
	jmethodID ClearBugsnagCache = (*Env).GetMethodID(ActivityClass, "clearBugsnagCache", "()V");
	jobject Activity = AndroidJavaEnv::GetGameActivityThis();
	(*Env).CallVoidMethod(Activity, ClearBugsnagCache);
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

static FString DisplayText;

static FString GetScenarioName()
{
	int32 ScenarioIndex = -1;
	LexFromString(ScenarioIndex, *DisplayText);
	UE_LOG(LogTemp, Display, TEXT("ScenarioIndex = %d"), ScenarioIndex);
	return ScenarioNames[ScenarioIndex];
}

void UTestFixtureBlueprintFunctionLibrary::AppendText(const FString& Text)
{
	DisplayText += Text;
}

FString UTestFixtureBlueprintFunctionLibrary::GetDisplayText()
{
	return DisplayText;
}

void UTestFixtureBlueprintFunctionLibrary::Run()
{
	FString ScenarioName = GetScenarioName();
	ClearPersistentData();
	Scenario::Run(ScenarioName);
	DisplayText.Reset();
}

void UTestFixtureBlueprintFunctionLibrary::Start()
{
	FString ScenarioName = GetScenarioName();
	Scenario::Start(ScenarioName);
	DisplayText.Reset();
}
