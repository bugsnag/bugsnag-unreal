// Copyright 2021 Bugsnag. All Rights Reserved.

#include "AndroidPlatformBugsnag.h"

#include <dlfcn.h>
#include <jni.h>
#include <cstdio>

#include "Android/AndroidJavaEnv.h"

#include "AndroidBreadcrumb.h"
#include "AndroidEvent.h"
#include "AndroidPlatformConfiguration.h"
#include "AndroidSession.h"
#include "BugsnagConstants.h"
#include "BugsnagUtils.h"
#include "JNIUtilities.h"
#include "LogBugsnag.h"
#include "Shorthand.h"

DEFINE_PLATFORM_BUGSNAG(FAndroidPlatformBugsnag);

static JNIReferenceCache JNICache;

void FAndroidPlatformBugsnag::Start(const TSharedRef<FBugsnagConfiguration>& Config)
{
	UE_LOG(LogBugsnag, Error, TEXT("RICHLOG CALLING START"));
	if (JNICache.loaded) // only attempt initialization once
	{
		return;
	}
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);

	UE_LOG(LogBugsnag, Error, TEXT("RICHLOG about to load JNICache for the first time"));
	JNICache.loaded = FAndroidPlatformJNI::LoadReferenceCache(Env, &JNICache);
	UE_LOG(LogBugsnag, Error, TEXT("RICHLOG JNICache WTF"));

	if(JNICache.loaded)
	{
		UE_LOG(LogBugsnag, Error, TEXT("RICHLOG JNICache loaded"));
	}
	if (JNICache.loaded)
	{
		OnBreadcrumbCallbacks += Config->GetOnBreadcrumbCallbacks();
					UE_LOG(LogBugsnag, Error, TEXT("RICHLOG JNICache loaded 1"));

		OnSessionCallbacks += Config->GetOnSessionCallbacks();
			UE_LOG(LogBugsnag, Error, TEXT("RICHLOG JNICache loaded 2"));

		OnSendErrorCallbacks += Config->GetOnSendErrorCallbacks();
			UE_LOG(LogBugsnag, Error, TEXT("RICHLOG JNICache loaded 3"));

		jobject jActivity = AndroidJavaEnv::GetGameActivityThis();
			UE_LOG(LogBugsnag, Error, TEXT("RICHLOG JNICache loaded 4"));

		jobject jApp = (*Env).CallObjectMethod(jActivity, JNICache.ContextGetApplication);
			UE_LOG(LogBugsnag, Error, TEXT("RICHLOG JNICache loaded 5"));

		ReturnVoidOnException(Env);
			UE_LOG(LogBugsnag, Error, TEXT("RICHLOG JNICache loaded 6"));

		jobject jConfig = FAndroidPlatformConfiguration::Parse(Env, &JNICache, Config);
			UE_LOG(LogBugsnag, Error, TEXT("RICHLOG JNICache loaded 7"));

		jobject jClient = (*Env).CallStaticObjectMethod(JNICache.BugsnagClass, JNICache.BugsnagStartMethod, jApp, jConfig);
			UE_LOG(LogBugsnag, Error, TEXT("RICHLOG JNICache loaded 8"));

		JNICache.initialized = !FAndroidPlatformJNI::CheckAndClearException(Env) && jClient != NULL;
			UE_LOG(LogBugsnag, Error, TEXT("RICHLOG JNICache loaded 9"));

		if (JNICache.initialized)
		{
			jstring jKey = FAndroidPlatformJNI::ParseFString(Env, BugsnagConstants::UnrealEngine);
			jstring jValue = FAndroidPlatformJNI::ParseFString(Env, BugsnagUtils::GetUnrealEngineVersion());
			(*Env).CallVoidMethod(jClient, JNICache.ClientAddRuntimeVersionInfo, jKey, jValue);
			FAndroidPlatformJNI::CheckAndClearException(Env);
			UE_LOG(LogBugsnag, Error, TEXT("RICHLOG JNICache initialized"));

		}
	}else{
		UE_LOG(LogBugsnag, Error, TEXT("RICHLOG JNICache NOT loaded"));
	}
			UE_LOG(LogBugsnag, Error, TEXT("RICHLOG Start finished"));

}

void FAndroidPlatformBugsnag::Notify(const FString& ErrorClass, const FString& Message, const TArray<uint64>& StackTrace,
	const FBugsnagOnErrorCallback& Callback)
{
	UE_LOG(LogBugsnag, Error, TEXT("RICHLOG NOTIFY: 1"));

	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);
	ReturnVoidIf(!Env || !JNICache.initialized);
		UE_LOG(LogBugsnag, Error, TEXT("RICHLOG NOTIFY: 2"));

	jstring jErrorClass = FAndroidPlatformJNI::ParseFString(Env, ErrorClass);
	jstring jMessage = FAndroidPlatformJNI::ParseFString(Env, Message);
	ReturnVoidIf(!jErrorClass || !jMessage);
		UE_LOG(LogBugsnag, Error, TEXT("RICHLOG NOTIFY: 3"));

	jobjectArray jFrames = (*Env).NewObjectArray(StackTrace.Num(), JNICache.TraceClass, NULL);
	ReturnVoidIf(!jFrames);
		UE_LOG(LogBugsnag, Error, TEXT("RICHLOG NOTIFY: 4"));

	// Java class name for each stack frame, intentionally empty
	jobject jClassName = (*Env).NewStringUTF("");
	ReturnVoidIf(!jClassName);
		UE_LOG(LogBugsnag, Error, TEXT("RICHLOG NOTIFY: 5"));

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
	jobject jCallbackBuffer = nullptr;
	if (Callback)
	{
		jCallbackBuffer = (*Env).NewDirectByteBuffer((void*)&Callback, sizeof(FBugsnagOnErrorCallback));
		// in the event the buffer could not be created, the callback is set to null
		FAndroidPlatformJNI::CheckAndClearException(Env);
	}
	(*Env).CallStaticVoidMethod(JNICache.BugsnagUnrealPluginClass,
		JNICache.BugsnagUnrealPluginNotify, jErrorClass, jMessage, jFrames, jCallbackBuffer);
	FAndroidPlatformJNI::CheckAndClearException(Env);
			UE_LOG(LogBugsnag, Error, TEXT("RICHLOG NOTIFY: 6"));

}

const TOptional<FString> FAndroidPlatformBugsnag::GetContext()
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);
	ReturnValueOnFail(Env && JNICache.initialized, TOptional<FString>());
	jobject jContext = (*Env).CallStaticObjectMethod(JNICache.BugsnagClass, JNICache.BugsnagGetContext);
	ReturnValueOnException(Env, TOptional<FString>());
	return FAndroidPlatformJNI::ParseJavaString(Env, &JNICache, jContext);
}

void FAndroidPlatformBugsnag::SetContext(const TOptional<FString>& Context)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);
	ReturnVoidIf(!Env || !JNICache.initialized);
	jstring jContext = FAndroidPlatformJNI::ParseFStringOptional(Env, Context);
	(*Env).CallStaticVoidMethod(JNICache.BugsnagClass, JNICache.BugsnagSetContext, jContext);
	ReturnVoidOnException(Env);
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

void FAndroidPlatformBugsnag::AddFeatureFlag(const FString& Name, const TOptional<FString>& Variant)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);
	jstring jName = FAndroidPlatformJNI::ParseFString(Env, Name);
	ReturnVoidIf(!jName);
	jstring jVariant = FAndroidPlatformJNI::ParseFStringOptional(Env, Variant);
	if (!jVariant && Variant.IsSet())
	{
		UE_LOG(LogBugsnag, Error, TEXT("Failed to create Java string with \"%s\""), *Variant.GetValue());
		return;
	}
	(*Env).CallStaticVoidMethod(JNICache.BugsnagClass, JNICache.BugsnagAddFeatureFlag, jName, jVariant);
	FAndroidPlatformJNI::CheckAndClearException(Env);
}

void FAndroidPlatformBugsnag::AddFeatureFlags(const TArray<FBugsnagFeatureFlag>& FeatureFlags)
{
	for (const FBugsnagFeatureFlag& Flag : FeatureFlags)
	{
		FAndroidPlatformBugsnag::AddFeatureFlag(Flag.GetName(), Flag.GetVariant());
	}
}

void FAndroidPlatformBugsnag::ClearFeatureFlag(const FString& Name)
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);
	jstring jName = FAndroidPlatformJNI::ParseFString(Env, Name);
	ReturnVoidIf(!jName);
	(*Env).CallStaticVoidMethod(JNICache.BugsnagClass, JNICache.BugsnagClearFeatureFlag, jName);
	FAndroidPlatformJNI::CheckAndClearException(Env);
}

void FAndroidPlatformBugsnag::ClearFeatureFlags()
{
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);
	(*Env).CallStaticVoidMethod(JNICache.BugsnagClass, JNICache.BugsnagClearFeatureFlags);
	FAndroidPlatformJNI::CheckAndClearException(Env);
}

void FAndroidPlatformBugsnag::AddMetadata(const FString& Section, const TSharedRef<FJsonObject>& Metadata)
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
	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv(true);
	ReturnValueOnFail(Env && JNICache.initialized, {});
	jobject jBreadcrumbs = (*Env).CallStaticObjectMethod(JNICache.BugsnagClass, JNICache.BugsnagGetBreadcrumbs);
	ReturnValueOnException(Env, {});
	ReturnValueOnFail(jBreadcrumbs, {});
	jint Len = (*Env).CallIntMethod(jBreadcrumbs, JNICache.ListSize);
	ReturnValueOnException(Env, {});
	TArray<TSharedRef<const class IBugsnagBreadcrumb>> Items;
	for (jint Index = 0; Index < Len; Index++)
	{
		jobject jItem = (*Env).CallObjectMethod(jBreadcrumbs, JNICache.ListGet, Index);
		ContinueOnException(Env);
		Items.Add(MakeShared<FAndroidBreadcrumb>(Env, &JNICache, jItem));
	}
	return Items;
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
	ReturnNullOnFail(Env && JNICache.loaded);
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

#ifdef __cplusplus
extern "C"
{
#endif

	JNIEXPORT jboolean JNICALL Java_com_bugsnag_android_unreal_UnrealPlugin_runNotifyCallback(
		JNIEnv* Env, jobject _this, jobject jEvent, jobject jCallbackPtr)
	{
		if (JNICache.initialized && jCallbackPtr)
		{
			auto Event = MakeShared<FAndroidEvent>(Env, &JNICache, jEvent);
			auto Callback = (FBugsnagOnErrorCallback*)(*Env).GetDirectBufferAddress(jCallbackPtr);
			return (*Callback)(Event) ? JNI_TRUE : JNI_FALSE;
		}
		return JNI_TRUE;
	}

	JNIEXPORT jboolean JNICALL Java_com_bugsnag_android_unreal_UnrealPlugin_runBreadcrumbCallbacks(
		JNIEnv* Env, jobject _this, jobject jCrumb)
	{
		if (JNICache.loaded)
		{
			auto Crumb = MakeShared<FAndroidBreadcrumb>(Env, &JNICache, jCrumb);
			return GPlatformBugsnag.RunOnBreadcrumbCallbacks(Crumb) ? JNI_TRUE : JNI_FALSE;
		}
		return JNI_TRUE;
	}

	JNIEXPORT jboolean JNICALL Java_com_bugsnag_android_unreal_UnrealPlugin_runSessionCallbacks(
		JNIEnv* Env, jobject _this, jobject jSession)
	{
		if (JNICache.loaded)
		{
			auto Session = MakeShared<FAndroidSession>(Env, &JNICache, jSession);
			return GPlatformBugsnag.RunOnSessionCallbacks(Session) ? JNI_TRUE : JNI_FALSE;
		}
		return JNI_TRUE;
	}

	JNIEXPORT jboolean JNICALL Java_com_bugsnag_android_unreal_UnrealPlugin_runEventCallbacks(
		JNIEnv* Env, jobject _this, jobject jEvent)
	{
		if (JNICache.loaded)
		{
			auto Event = MakeShared<FAndroidEvent>(Env, &JNICache, jEvent);
			return GPlatformBugsnag.RunOnSendCallbacks(Event) ? JNI_TRUE : JNI_FALSE;
		}
		return JNI_TRUE;
	}

	JNIEXPORT void JNICALL Java_com_bugsnag_android_unreal_UnrealPlugin_setSeverityReason(
		JNIEnv* Env, jobject _this, jobject jEvent, jobject jReasonType)
	{
		if (JNICache.loaded)
		{
			(*Env).CallVoidMethod(jEvent, JNICache.EventUpdateSeverityReason, jReasonType);
			ReturnVoidOnException(Env);
		}
	}

#ifdef __cplusplus
}
#endif
