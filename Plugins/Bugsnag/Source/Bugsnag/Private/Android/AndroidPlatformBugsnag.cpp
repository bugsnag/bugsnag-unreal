#include "AndroidPlatformBugsnag.h"

#include <dlfcn.h>
#include <jni.h>
#include <cstdio>

#include "Android/AndroidJavaEnv.h"

#include "AndroidBreadcrumb.h"
#include "AndroidPlatformConfiguration.h"
#include "JNIUtilities.h"
#include "Shorthand.h"

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
		OnBreadcrumbCallbacks += Config->GetOnBreadcrumbCallbacks();
		jobject jActivity = AndroidJavaEnv::GetGameActivityThis();
		jobject jConfig = FAndroidPlatformConfiguration::Parse(Env, &JNICache, Config);
		jobject jClient = (*Env).CallStaticObjectMethod(JNICache.BugsnagClass, JNICache.BugsnagStartMethod, jActivity, jConfig);
		JNICache.initialized = !FAndroidPlatformJNI::CheckAndClearException(Env) && jClient != NULL;
	}
}

void FAndroidPlatformBugsnag::Notify(const FString& ErrorClass, const FString& Message, const TArray<uint64>& StackTrace,
	const FBugsnagOnErrorCallback& Callback)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);
	ReturnVoidIf(!Env || !JNICache.initialized);

	jstring jErrorClass = FAndroidPlatformJNI::ParseFString(Env, ErrorClass);
	jstring jMessage = FAndroidPlatformJNI::ParseFString(Env, Message);
	ReturnVoidIf(!jErrorClass || !jMessage);

	jobject jSeverity = FAndroidPlatformJNI::ParseSeverity(Env, &JNICache, EBugsnagSeverity::Warning);
	ReturnVoidIf(!jSeverity);

	jobjectArray jFrames = (*Env).NewObjectArray(StackTrace.Num(), JNICache.TraceClass, NULL);
	ReturnVoidIf(!jFrames);

	// Java class name for each stack frame, intentionally empty
	jobject jClassName = (*Env).NewStringUTF("");
	ReturnVoidIf(!jClassName);

	Dl_info Info;
	jstring jFilename, jMethod;
	jobject jFrame;
	uint32 jFramesIndex = 0;
	const uint32 MethodNameLength = 256;
	char MethodNameBuffer[MethodNameLength] = {0};
	for (uint64 Address : StackTrace)
	{
		if (dladdr((void*)Address, &Info) != 0)
		{
			jFilename = (*Env).NewStringUTF(Info.dli_fname ?: "");
			if (FAndroidPlatformJNI::CheckAndClearException(Env) || !jFilename)
			{
				continue;
			}
			if (Info.dli_sname)
			{
				jMethod = (*Env).NewStringUTF(Info.dli_sname);
			}
			else
			{
				// print hex formatted address "0x{frame address}" when symbol name is null
				snprintf(MethodNameBuffer, MethodNameLength, "0x%016llx", Address);
				jMethod = (*Env).NewStringUTF(MethodNameBuffer);
			}
			if (FAndroidPlatformJNI::CheckAndClearException(Env) || !jMethod)
			{
				continue;
			}

			jFrame = (*Env).NewObject(JNICache.TraceClass,
				JNICache.TraceConstructor, jClassName, jMethod, jFilename,
				// Address offset is encoded into the line number field
				Address - (uintptr_t)Info.dli_fbase);
			if (jFrame && !FAndroidPlatformJNI::CheckAndClearException(Env))
			{
				(*Env).SetObjectArrayElement(jFrames, jFramesIndex++, jFrame);
				FAndroidPlatformJNI::CheckAndClearException(Env);
			}
		}
	}
	(*Env).CallStaticVoidMethod(JNICache.InterfaceClass,
		JNICache.BugsnagNotifyMethod, jErrorClass, jMessage, jSeverity, jFrames);
	FAndroidPlatformJNI::CheckAndClearException(Env);
	// TODO: handle callback
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

void FAndroidPlatformBugsnag::AddOnSendError(FBugsnagOnErrorCallback Callback)
{
}

void FAndroidPlatformBugsnag::AddOnSession(FBugsnagOnSessionCallback Callback)
{
}

bool FAndroidPlatformBugsnag::RunOnBreadcrumbCallbacks(TSharedRef<IBugsnagBreadcrumb> Crumb)
{
	for (auto& Callback : OnBreadcrumbCallbacks)
	{
		if (!Callback(Crumb))
		{
			return false;
		}
	}
	return true;
}

#ifdef __cplusplus
extern "C"
{
#endif

	JNIEXPORT jboolean JNICALL Java_com_bugsnag_android_unreal_UnrealPlugin_runBreadcrumbCallbacks(
		JNIEnv* Env, jobject _this, jobject jCrumb)
	{
		if (JNICache.loaded)
		{
			auto Crumb = FAndroidBreadcrumb::From(Env, &JNICache, jCrumb);
			return GPlatformBugsnag.RunOnBreadcrumbCallbacks(Crumb) ? JNI_TRUE : JNI_FALSE;
		}
		return JNI_TRUE;
	}

#ifdef __cplusplus
}
#endif
