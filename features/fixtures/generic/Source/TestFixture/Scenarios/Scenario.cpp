#include "Scenario.h"

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

#if PLATFORM_ANDROID
#include "Android/AndroidJavaEnv.h"
#include "Misc/EngineVersion.h"
#endif

Scenario* Scenario::CurrentScenario = nullptr;

static void PingGoogle()
{
    FHttpModule* Http = &FHttpModule::Get();
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();

    Request->OnProcessRequestComplete().BindLambda([](FHttpRequestPtr Req, FHttpResponsePtr Resp, bool bSucceeded)
    {
        if (bSucceeded && Resp.IsValid())
        {
            UE_LOG(LogTestFixture, Log, TEXT("Ping success! Code: %d"), Resp->GetResponseCode());
        }
        else
        {
            UE_LOG(LogTestFixture, Error, TEXT("Ping failed."));
        }
    });

    Request->SetURL("https://www.google.com");
    Request->SetVerb("GET");
    Request->ProcessRequest();
}

void Scenario::ClearPersistentData()
{
	PingGoogle();

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
			FString ErrorDescription = UTF8_TO_TCHAR([Error.description UTF8String]);
			UE_LOG(LogTestFixture, Error, TEXT("%s"), *ErrorDescription);
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
