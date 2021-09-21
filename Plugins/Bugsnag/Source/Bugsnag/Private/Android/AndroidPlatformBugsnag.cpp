#include "AndroidPlatformBugsnag.h"

#include <jni.h>

#include "Android/AndroidJavaEnv.h"
#include "AndroidPlatformConfiguration.h"
#include "JNIUtilities.h"

DEFINE_PLATFORM_BUGSNAG(FAndroidPlatformBugsnag);

static JNIReferenceCache JNICache;

void FAndroidPlatformBugsnag::Start(const TSharedPtr<FBugsnagConfiguration>& Config)
{
	if (JNICache.loaded) // only attempt initialization once
	{
		return;
	}
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);
	if ((JNICache.loaded = FAndroidPlatformJNI::LoadReferenceCache(Env, &JNICache)))
	{
		jobject jActivity = AndroidJavaEnv::GetGameActivityThis();
		jobject jConfig = FAndroidPlatformConfiguration::Parse(Env, &JNICache, Config);
		jobject jClient = (*Env).CallStaticObjectMethod(JNICache.BugsnagClass, JNICache.BugsnagStartMethod, jActivity, jConfig);
		JNICache.initialized = !FAndroidPlatformJNI::CheckAndClearException(Env) && jClient != NULL;
	}
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
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);
	if (!Env || !JNICache.initialized)
	{
		return;
	}
	jstring jContext = FAndroidPlatformJNI::ParseFString(Env, Context);
	if (jContext)
	{
		(*Env).CallStaticVoidMethod(JNICache.BugsnagClass, JNICache.BugsnagSetContext, jContext);
	}
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
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);
	if (Env && JNICache.initialized)
	{
		jstring jMessage = FAndroidPlatformJNI::ParseFString(Env, Message);
		jobject jType = FAndroidPlatformJNI::ParseBreadcrumbType(Env, &JNICache, Type);

		if (!jMessage || !jType)
		{
			return;
		}

		jobject jMetadata = Metadata
								? FAndroidPlatformJNI::ParseJsonObject(Env, &JNICache, Metadata)
								: (*Env).NewObject(JNICache.HashMapClass, JNICache.HashMapConstructor);
		if (FAndroidPlatformJNI::CheckAndClearException(Env) || !jMetadata)
		{
			return;
		}
		(*Env).CallStaticVoidMethod(JNICache.BugsnagClass, JNICache.BugsnagLeaveBreadcrumb, jMessage, jMetadata, jType);
		FAndroidPlatformJNI::CheckAndClearException(Env);
	}
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
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);
	if (Env && JNICache.initialized)
	{
		(*Env).CallStaticVoidMethod(JNICache.BugsnagClass, JNICache.BugsnagStartSession);
		FAndroidPlatformJNI::CheckAndClearException(Env);
	}
}

void FAndroidPlatformBugsnag::PauseSession()
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);
	if (Env && JNICache.initialized)
	{
		(*Env).CallStaticVoidMethod(JNICache.BugsnagClass, JNICache.BugsnagPauseSession);
		FAndroidPlatformJNI::CheckAndClearException(Env);
	}
}

bool FAndroidPlatformBugsnag::ResumeSession()
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);
	if (Env && JNICache.initialized)
	{
		jboolean result = (*Env).CallStaticBooleanMethod(JNICache.BugsnagClass, JNICache.BugsnagResumeSession);
		FAndroidPlatformJNI::CheckAndClearException(Env);
		return result == JNI_TRUE;
	}
	return false;
}

void FAndroidPlatformBugsnag::AddOnBreadcrumb(const FBugsnagOnBreadcrumbCallback& Callback)
{
}

void FAndroidPlatformBugsnag::AddOnSendError(const FBugsnagOnErrorCallback& Callback)
{
}

void FAndroidPlatformBugsnag::AddOnSession(const FBugsnagOnSessionCallback& Callback)
{
}
