#include "AndroidPlatformBugsnag.h"

#include <dlfcn.h>
#include <jni.h>
#include <cstdio>

#include "Android/AndroidJavaEnv.h"

#include "AndroidBreadcrumb.h"
#include "AndroidPlatformConfiguration.h"
#include "AndroidSession.h"
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
		OnSessionCallbacks += Config->GetOnSessionCallbacks();
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

const TSharedPtr<FString> FAndroidPlatformBugsnag::GetContext()
{
	return nullptr;
}

void FAndroidPlatformBugsnag::SetContext(const TSharedPtr<FString>& Context)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);
	ReturnVoidIf(!Env || !JNICache.initialized);
	jstring jContext = FAndroidPlatformJNI::ParseFStringPtr(Env, Context);
	(*Env).CallStaticVoidMethod(JNICache.BugsnagClass, JNICache.BugsnagSetContext, jContext);
}

const FBugsnagUser FAndroidPlatformBugsnag::GetUser()
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);
	if (!Env || !JNICache.initialized)
	{
		return FBugsnagUser();
	}
	jobject jUser = (*Env).CallStaticObjectMethod(JNICache.BugsnagClass, JNICache.BugsnagGetUser);
	return FAndroidPlatformJNI::CheckAndClearException(Env)
			   ? FBugsnagUser()
			   : FBugsnagUser(FAndroidPlatformJNI::ParseJavaUser(Env, &JNICache, jUser));
}

void FAndroidPlatformBugsnag::SetUser(const FString& Id, const FString& Email, const FString& Name)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);
	ReturnVoidIf(!Env || !JNICache.initialized);
	jstring jId = FAndroidPlatformJNI::ParseFString(Env, Id);
	jstring jName = FAndroidPlatformJNI::ParseFString(Env, Name);
	jstring jEmail = FAndroidPlatformJNI::ParseFString(Env, Email);
	(*Env).CallStaticVoidMethod(JNICache.BugsnagClass, JNICache.BugsnagSetUser, jId, jEmail, jName);
	FAndroidPlatformJNI::CheckAndClearException(Env);
}

void FAndroidPlatformBugsnag::AddMetadata(const FString& Section, const TSharedPtr<FJsonObject>& Metadata)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);
	ReturnVoidIf(!Env || !JNICache.initialized);
	jstring jSection = FAndroidPlatformJNI::ParseFString(Env, Section);
	ReturnVoidIf(!jSection);
	jobject jMetadata = FAndroidPlatformJNI::ParseJsonObject(Env, &JNICache, Metadata);
	ReturnVoidOnException(Env);
	ReturnVoidIf(!jMetadata);
	(*Env).CallStaticVoidMethod(JNICache.BugsnagClass, JNICache.BugsnagAddMetadataToSection, jSection, jMetadata);
	FAndroidPlatformJNI::CheckAndClearException(Env);
}

void FAndroidPlatformBugsnag::AddMetadata(const FString& Section, const FString& Key, const TSharedPtr<FJsonValue>& Value)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);
	ReturnVoidIf(!Env || !JNICache.initialized);
	jstring jSection = FAndroidPlatformJNI::ParseFString(Env, Section);
	ReturnVoidIf(!jSection);
	jstring jKey = FAndroidPlatformJNI::ParseFString(Env, Key);
	ReturnVoidIf(!jKey);
	TSharedPtr<FJsonObject> Container = MakeShareable(new FJsonObject);
	Container->SetField("Value", Value);
	jobject jContainer = FAndroidPlatformJNI::ParseJsonObject(Env, &JNICache, Container);
	ReturnVoidOnException(Env);
	jstring jValueKey = (*Env).NewStringUTF("Value");
	ReturnVoidOnException(Env);
	jobject jMetadata = (*Env).CallObjectMethod(jContainer, JNICache.HashMapGet, jValueKey);
	ReturnVoidOnException(Env);
	(*Env).CallStaticVoidMethod(JNICache.BugsnagClass, JNICache.BugsnagAddMetadataValue, jSection, jKey, jMetadata);
	FAndroidPlatformJNI::CheckAndClearException(Env);
}

TSharedPtr<FJsonObject> FAndroidPlatformBugsnag::GetMetadata(const FString& Section)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);
	ReturnNullOnFail(Env && JNICache.initialized);
	jstring jSection = FAndroidPlatformJNI::ParseFString(Env, Section);
	ReturnNullOnFail(jSection);
	jbyteArray jMetadata = (jbyteArray)(*Env).CallStaticObjectMethod(JNICache.BugsnagUnrealPluginClass, JNICache.BugsnagUnrealPluginGetMetadataSection, jSection);
	ReturnNullOnException(Env);
	ReturnNullOnFail(jMetadata);
	return FAndroidPlatformJNI::ConvertJavaBytesToJson(Env, &JNICache, jMetadata);
}

TSharedPtr<FJsonValue> FAndroidPlatformBugsnag::GetMetadata(const FString& Section, const FString& Key)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);
	ReturnNullOnFail(Env && JNICache.initialized);
	jstring jSection = FAndroidPlatformJNI::ParseFString(Env, Section);
	ReturnNullOnFail(jSection);
	jstring jKey = FAndroidPlatformJNI::ParseFString(Env, Key);
	ReturnNullOnFail(jKey);
	jbyteArray jMetadata = (jbyteArray)(*Env).CallStaticObjectMethod(JNICache.BugsnagUnrealPluginClass, JNICache.BugsnagUnrealPluginGetMetadataValue, jSection, jKey);
	ReturnNullOnException(Env);
	auto MetadataContainer = FAndroidPlatformJNI::ConvertJavaBytesToJson(Env, &JNICache, jMetadata);

	return MetadataContainer.IsValid()
			   ? MetadataContainer->TryGetField("Value")
			   : nullptr;
}

void FAndroidPlatformBugsnag::ClearMetadata(const FString& Section)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);
	ReturnVoidIf(!Env || !JNICache.initialized);
	jstring jSection = FAndroidPlatformJNI::ParseFString(Env, Section);
	ReturnVoidIf(!jSection);
	(*Env).CallStaticVoidMethod(JNICache.BugsnagClass, JNICache.BugsnagClearMetadataSection, jSection);
	FAndroidPlatformJNI::CheckAndClearException(Env);
}

void FAndroidPlatformBugsnag::ClearMetadata(const FString& Section, const FString& Key)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);
	ReturnVoidIf(!Env || !JNICache.initialized);
	jstring jSection = FAndroidPlatformJNI::ParseFString(Env, Section);
	ReturnVoidIf(!jSection);
	jstring jKey = FAndroidPlatformJNI::ParseFString(Env, Key);
	ReturnVoidIf(!jKey);
	(*Env).CallStaticVoidMethod(JNICache.BugsnagClass, JNICache.BugsnagClearMetadataValue, jSection, jKey);
	FAndroidPlatformJNI::CheckAndClearException(Env);
}

void FAndroidPlatformBugsnag::LeaveBreadcrumb(const FString& Message, const TSharedPtr<FJsonObject>& Metadata, EBugsnagBreadcrumbType Type)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);
	ReturnVoidIf(!Env || !JNICache.initialized);
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

TArray<TSharedRef<const class IBugsnagBreadcrumb>> FAndroidPlatformBugsnag::GetBreadcrumbs()
{
	return {};
}

void FAndroidPlatformBugsnag::MarkLaunchCompleted()
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);
	ReturnVoidIf(!Env || !JNICache.initialized);
	(*Env).CallStaticVoidMethod(JNICache.BugsnagClass, JNICache.BugsnagMarkLaunchCompleted);
	FAndroidPlatformJNI::CheckAndClearException(Env);
}

TSharedPtr<FBugsnagLastRunInfo> FAndroidPlatformBugsnag::GetLastRunInfo()
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);
	ReturnNullOnFail(Env && JNICache.initialized);
	jobject jLastRunInfo = (*Env).CallStaticObjectMethod(JNICache.BugsnagClass, JNICache.BugsnagGetLastRunInfo);
	ReturnNullOnException(Env);
	ReturnNullOnFail(jLastRunInfo);
	jboolean jCrashed = (*Env).CallBooleanMethod(jLastRunInfo, JNICache.LastRunInfoGetCrashed);
	ReturnValueOnException(Env, nullptr);
	jboolean jCrashedDuringLaunch = (*Env).CallBooleanMethod(jLastRunInfo, JNICache.LastRunInfoGetCrashedDuringLaunch);
	ReturnValueOnException(Env, nullptr);
	jint jConsecutiveCrashes = (*Env).CallIntMethod(jLastRunInfo, JNICache.LastRunInfoGetConsecutiveLaunchCrashes);
	ReturnValueOnException(Env, nullptr);
	return MakeShareable(new FBugsnagLastRunInfo(
		jConsecutiveCrashes,
		jCrashed == JNI_TRUE,
		jCrashedDuringLaunch == JNI_TRUE));
}

void FAndroidPlatformBugsnag::StartSession()
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);
	ReturnVoidIf(!Env || !JNICache.initialized);
	(*Env).CallStaticVoidMethod(JNICache.BugsnagClass, JNICache.BugsnagStartSession);
	FAndroidPlatformJNI::CheckAndClearException(Env);
}

void FAndroidPlatformBugsnag::PauseSession()
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);
	ReturnVoidIf(!Env || !JNICache.initialized);
	(*Env).CallStaticVoidMethod(JNICache.BugsnagClass, JNICache.BugsnagPauseSession);
	FAndroidPlatformJNI::CheckAndClearException(Env);
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

	JNIEXPORT jboolean JNICALL Java_com_bugsnag_android_unreal_UnrealPlugin_runSessionCallbacks(
		JNIEnv* Env, jobject _this, jobject jSession)
	{
		if (JNICache.loaded)
		{
			auto Session = FAndroidSession::From(Env, &JNICache, jSession);
			return GPlatformBugsnag.RunOnSessionCallbacks(Session) ? JNI_TRUE : JNI_FALSE;
		}
		return JNI_TRUE;
	}

#ifdef __cplusplus
}
#endif
