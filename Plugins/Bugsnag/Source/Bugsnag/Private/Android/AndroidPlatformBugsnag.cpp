#include "AndroidPlatformBugsnag.h"

#include <jni.h>

#include "Android/AndroidJavaEnv.h"
#include "AndroidPlatformConfiguration.h"
#include "JNIUtilities.h"

DEFINE_PLATFORM_BUGSNAG(FAndroidPlatformBugsnag);

static JNIReferenceCache JNICache;

void FAndroidPlatformBugsnag::Start(const TSharedPtr<FBugsnagConfiguration>& Config)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);
	if ((JNICache.loaded = FAndroidPlatformJNI::LoadReferenceCache(Env, &JNICache)))
	{
		jobject jActivity = AndroidJavaEnv::GetGameActivityThis();
		jobject jConfig = FAndroidPlatformConfiguration::Parse(Env, &JNICache, Config);
		jobject jClient = (*Env).CallStaticObjectMethod(JNICache.BugsnagClass, JNICache.BugsnagStartMethod, jActivity, jConfig);
		JNICache.initialized = !FAndroidPlatformJNI::CheckAndClearException(Env) && jClient != NULL;
	}
}

void FAndroidPlatformBugsnag::Notify(const FString& ErrorClass, const FString& Message)
{
}

void FAndroidPlatformBugsnag::Notify(const FString& ErrorClass, const FString& Message, const FBugsnagOnErrorCallback& Callback)
{
}

void FAndroidPlatformBugsnag::Notify(const FString& ErrorClass, const FString& Message, const TArray<uint64>& StackTrace)
{
}

void FAndroidPlatformBugsnag::Notify(const FString& ErrorClass, const FString& Message, const TArray<uint64>& StackTrace,
	const FBugsnagOnErrorCallback& Callback)
{
}

const FString FAndroidPlatformBugsnag::GetContext()
{
	return TEXT("");
}

void FAndroidPlatformBugsnag::SetContext(const FString& Context)
{
}

const TSharedPtr<FBugsnagUser> FAndroidPlatformBugsnag::GetUser()
{
	return nullptr;
}

void FAndroidPlatformBugsnag::SetUser(const FString& Id, const FString& Email, const FString& Name)
{
}

void FAndroidPlatformBugsnag::AddMetadata(const FString& Section, const TSharedPtr<FJsonObject>& Metadata)
{
}

void FAndroidPlatformBugsnag::AddMetadata(const FString& Section, const FString& Key, const TSharedPtr<FJsonValue>& Value)
{
}

TSharedPtr<FJsonObject> FAndroidPlatformBugsnag::GetMetadata(const FString& Section)
{
	return nullptr;
}

TSharedPtr<FJsonValue> FAndroidPlatformBugsnag::GetMetadata(const FString& Section, const FString& Key)
{
	return nullptr;
}

void FAndroidPlatformBugsnag::ClearMetadata(const FString& Section)
{
}

void FAndroidPlatformBugsnag::ClearMetadata(const FString& Section, const FString& Key)
{
}

void FAndroidPlatformBugsnag::LeaveBreadcrumb(const FString& Message, const TSharedPtr<FJsonObject>& Metadata, EBugsnagBreadcrumbType Type)
{
}

TArray<TSharedRef<const class IBugsnagBreadcrumb>> FAndroidPlatformBugsnag::GetBreadcrumbs()
{
	return {};
}

void FAndroidPlatformBugsnag::MarkLaunchCompleted()
{
}

TSharedPtr<FBugsnagLastRunInfo> FAndroidPlatformBugsnag::GetLastRunInfo()
{
	return nullptr;
}

void FAndroidPlatformBugsnag::StartSession()
{
}

void FAndroidPlatformBugsnag::PauseSession()
{
}

bool FAndroidPlatformBugsnag::ResumeSession()
{
	return false;
}

void FAndroidPlatformBugsnag::AddOnBreadcrumb(const FBugsnagOnBreadcrumbCallback& Callback)
{
}

void FAndroidPlatformBugsnag::AddOnError(const FBugsnagOnErrorCallback& Callback)
{
}

void FAndroidPlatformBugsnag::AddOnSession(const FBugsnagOnSessionCallback& Callback)
{
}
