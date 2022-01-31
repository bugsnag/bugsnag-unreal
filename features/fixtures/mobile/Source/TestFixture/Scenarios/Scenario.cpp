#include "Scenario.h"

#if PLATFORM_ANDROID
#include "Android/AndroidJavaEnv.h"
#include "Misc/EngineVersion.h"
#endif

Scenario* Scenario::CurrentScenario = nullptr;

void Scenario::ClearPersistentData()
{
	UE_LOG(LogTestFixture, Display, TEXT("Clearing persistent data"));
#if PLATFORM_ANDROID
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);
	jclass ActivityClass = AndroidJavaEnv::FindJavaClass(
		FEngineVersion::Current().GetMajor() < 5
			? "com/epicgames/ue4/GameActivity"
			: "com/epicgames/unreal/GameActivity");
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
			UE_LOG(LogTestFixture, Error, TEXT("%@"), UTF8_TO_TCHAR(Error.description.UTF8String));
		}
	}
#endif
}

UWorld* Scenario::GetCurrentPlayWorld()
{
	if (GEngine)
	{
		for (const FWorldContext& Context : GEngine->GetWorldContexts())
		{
			if (Context.WorldType == EWorldType::Game)
			{
				return Context.World();
			}
		}
	}
	return nullptr;
}
