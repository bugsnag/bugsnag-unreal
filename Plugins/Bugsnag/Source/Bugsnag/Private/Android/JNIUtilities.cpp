#include "JNIUtilities.h"

#include "Version.h"

#include <string>

#include "Android/AndroidJavaEnv.h"
#include "CoreMinimal.h"
#include "Shorthand.h"

#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/LargeMemoryReader.h"
#include "Serialization/LargeMemoryWriter.h"

/**
 * Load a Java class as a global reference
 *
 * @param env             The JNI environment to use
 * @param qualified_name  The name of the class, formatted for the load method
 * @param use_helpers     true to use the Unreal Engine JNI utilities
 *
 * @return a jclass reference or null if not found. The result must always be checked.
 */
static jclass LoadJavaClass(JNIEnv* env, const char* qualified_name, bool use_helpers)
{
	if (use_helpers)
	{
		return AndroidJavaEnv::FindJavaClassGlobalRef(qualified_name);
	}
	return (jclass)(*env).NewGlobalRef((*env).FindClass(qualified_name));
}

bool FAndroidPlatformJNI::LoadReferenceCache(JNIEnv* env, JNIReferenceCache* cache)
{
	if (!env)
	{
		return false;
	}

	// Third-party classes are loaded via a class loader accessible to the Unreal JNI helpers
	cache->BugsnagClass = LoadJavaClass(env, "com.bugsnag.android.Bugsnag", true);
	ReturnFalseIfNullAndClearExceptions(env, cache->BugsnagClass);
	cache->ConfigClass = LoadJavaClass(env, "com.bugsnag.android.Configuration", true);
	ReturnFalseIfNullAndClearExceptions(env, cache->ConfigClass);
	cache->InterfaceClass = LoadJavaClass(env, "com.bugsnag.android.NativeInterface", true);
	ReturnFalseIfNullAndClearExceptions(env, cache->InterfaceClass);
	cache->NotifierClass = LoadJavaClass(env, "com.bugsnag.android.Notifier", true);
	ReturnFalseIfNullAndClearExceptions(env, cache->NotifierClass);
	cache->BreadcrumbTypeClass = LoadJavaClass(env, "com.bugsnag.android.BreadcrumbType", true);
	ReturnFalseIfNullAndClearExceptions(env, cache->BreadcrumbTypeClass);
	cache->SeverityClass = LoadJavaClass(env, "com.bugsnag.android.Severity", true);
	ReturnFalseIfNullAndClearExceptions(env, cache->SeverityClass);
	cache->EndpointConfigurationClass = LoadJavaClass(env, "com.bugsnag.android.EndpointConfiguration", true);
	ReturnFalseIfNullAndClearExceptions(env, cache->EndpointConfigurationClass);
	cache->ErrorTypesClass = LoadJavaClass(env, "com.bugsnag.android.ErrorTypes", true);
	ReturnFalseIfNullAndClearExceptions(env, cache->ErrorTypesClass);
	cache->ThreadSendPolicyClass = LoadJavaClass(env, "com.bugsnag.android.ThreadSendPolicy", true);
	ReturnFalseIfNullAndClearExceptions(env, cache->ThreadSendPolicyClass);
	cache->MetadataParserClass = LoadJavaClass(env, "com.bugsnag.android.unreal.MetadataParser", true);
	ReturnFalseIfNullAndClearExceptions(env, cache->MetadataParserClass);

	// Core classes are available through standard JNI functions only
	cache->HashMapClass = LoadJavaClass(env, "java/util/HashMap", false);
	ReturnFalseIfNullAndClearExceptions(env, cache->HashMapClass);
	cache->ArrayListClass = LoadJavaClass(env, "java/util/ArrayList", false);
	ReturnFalseIfNullAndClearExceptions(env, cache->ArrayListClass);
	cache->HashSetClass = LoadJavaClass(env, "java/util/HashSet", false);
	ReturnFalseIfNullAndClearExceptions(env, cache->HashSetClass);
	cache->IntegerClass = LoadJavaClass(env, "java/lang/Integer", false);
	ReturnFalseIfNullAndClearExceptions(env, cache->IntegerClass);
	cache->TraceClass = LoadJavaClass(env, "java/lang/StackTraceElement", false);
	ReturnFalseIfNullAndClearExceptions(env, cache->TraceClass);

	cache->BugsnagStartMethod = (*env).GetStaticMethodID(cache->BugsnagClass, "start",
		"(Landroid/content/Context;Lcom/bugsnag/android/Configuration;)Lcom/bugsnag/android/Client;");
	ReturnFalseIfNullAndClearExceptions(env, cache->BugsnagStartMethod);
	cache->BugsnagSetContext = (*env).GetStaticMethodID(cache->BugsnagClass, "setContext",
		"(Ljava/lang/String;)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->BugsnagSetContext);
	cache->BugsnagNotifyMethod = (*env).GetStaticMethodID(cache->InterfaceClass, "notify",
		"(Ljava/lang/String;Ljava/lang/String;Lcom/bugsnag/android/Severity;[Ljava/lang/StackTraceElement;)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->BugsnagNotifyMethod);
	cache->BugsnagLeaveBreadcrumb = (*env).GetStaticMethodID(cache->BugsnagClass, "leaveBreadcrumb",
		"(Ljava/lang/String;Ljava/util/Map;Lcom/bugsnag/android/BreadcrumbType;)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->BugsnagLeaveBreadcrumb);
	cache->BugsnagStartSession = (*env).GetStaticMethodID(cache->BugsnagClass, "startSession", "()V");
	ReturnFalseIfNullAndClearExceptions(env, cache->BugsnagStartSession);
	cache->BugsnagResumeSession = (*env).GetStaticMethodID(cache->BugsnagClass, "resumeSession", "()Z");
	ReturnFalseIfNullAndClearExceptions(env, cache->BugsnagResumeSession);
	cache->BugsnagPauseSession = (*env).GetStaticMethodID(cache->BugsnagClass, "pauseSession", "()V");
	ReturnFalseIfNullAndClearExceptions(env, cache->BugsnagPauseSession);

	cache->MetadataParserParse = (*env).GetStaticMethodID(cache->MetadataParserClass, "parse", "([B)Ljava/util/Map;");
	ReturnFalseIfNullAndClearExceptions(env, cache->MetadataParserParse);

	cache->ConfigConstructor = (*env).GetMethodID(cache->ConfigClass, "<init>",
		"(Ljava/lang/String;)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->ConfigConstructor);
	cache->ConfigAddMetadata = (*env).GetMethodID(cache->ConfigClass, "addMetadata", "(Ljava/lang/String;Ljava/util/Map;)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->ConfigAddMetadata);
	cache->ConfigGetNotifier = (*env).GetMethodID(cache->ConfigClass, "getNotifier", "()Lcom/bugsnag/android/Notifier;");
	ReturnFalseIfNullAndClearExceptions(env, cache->ConfigGetNotifier);
	cache->ConfigSetAppType = (*env).GetMethodID(cache->ConfigClass, "setAppType", "(Ljava/lang/String;)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->ConfigSetAppType);
	cache->ConfigSetAppVersion = (*env).GetMethodID(cache->ConfigClass, "setAppVersion", "(Ljava/lang/String;)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->ConfigSetAppVersion);
	cache->ConfigSetAutoDetectErrors = (*env).GetMethodID(cache->ConfigClass, "setAutoDetectErrors", "(Z)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->ConfigSetAutoDetectErrors);
	cache->ConfigSetAutoTrackSessions = (*env).GetMethodID(cache->ConfigClass, "setAutoTrackSessions", "(Z)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->ConfigSetAutoTrackSessions);
	cache->ConfigSetContext = (*env).GetMethodID(cache->ConfigClass, "setContext", "(Ljava/lang/String;)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->ConfigSetContext);
	cache->ConfigSetDiscardClasses = (*env).GetMethodID(cache->ConfigClass, "setDiscardClasses", "(Ljava/util/Set;)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->ConfigSetDiscardClasses);
	cache->ConfigSetEnabledBreadcrumbTypes = (*env).GetMethodID(cache->ConfigClass, "setEnabledBreadcrumbTypes", "(Ljava/util/Set;)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->ConfigSetEnabledBreadcrumbTypes);
	cache->ConfigSetEnabledErrorTypes = (*env).GetMethodID(cache->ConfigClass, "setEnabledErrorTypes", "(Lcom/bugsnag/android/ErrorTypes;)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->ConfigSetEnabledErrorTypes);
	cache->ConfigSetEnabledReleaseStages = (*env).GetMethodID(cache->ConfigClass, "setEnabledReleaseStages", "(Ljava/util/Set;)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->ConfigSetEnabledReleaseStages);
	cache->ConfigSetEndpoints = (*env).GetMethodID(cache->ConfigClass, "setEndpoints", "(Lcom/bugsnag/android/EndpointConfiguration;)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->ConfigSetEndpoints);
	cache->ConfigSetLaunchDurationMillis = (*env).GetMethodID(cache->ConfigClass, "setLaunchDurationMillis", "(J)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->ConfigSetLaunchDurationMillis);
	cache->ConfigSetMaxBreadcrumbs = (*env).GetMethodID(cache->ConfigClass, "setMaxBreadcrumbs", "(I)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->ConfigSetMaxBreadcrumbs);
	cache->ConfigSetMaxPersistedEvents = (*env).GetMethodID(cache->ConfigClass, "setMaxPersistedEvents", "(I)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->ConfigSetMaxPersistedEvents);
	cache->ConfigSetPersistUser = (*env).GetMethodID(cache->ConfigClass, "setPersistUser", "(Z)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->ConfigSetPersistUser);
	cache->ConfigSetRedactedKeys = (*env).GetMethodID(cache->ConfigClass, "setRedactedKeys", "(Ljava/util/Set;)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->ConfigSetRedactedKeys);
	cache->ConfigSetReleaseStage = (*env).GetMethodID(cache->ConfigClass, "setReleaseStage", "(Ljava/lang/String;)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->ConfigSetReleaseStage);
	cache->ConfigSetSendLaunchCrashesSynchronously = (*env).GetMethodID(cache->ConfigClass, "setSendLaunchCrashesSynchronously", "(Z)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->ConfigSetSendLaunchCrashesSynchronously);
	cache->ConfigSetSendThreads = (*env).GetMethodID(cache->ConfigClass, "setSendThreads", "(Lcom/bugsnag/android/ThreadSendPolicy;)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->ConfigSetSendThreads);
	cache->ConfigSetUser = (*env).GetMethodID(cache->ConfigClass, "setUser", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->ConfigSetUser);
	cache->ConfigSetVersionCode = (*env).GetMethodID(cache->ConfigClass, "setVersionCode", "(Ljava/lang/Integer;)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->ConfigSetVersionCode);

	cache->EndpointConfigurationConstructor = (*env).GetMethodID(cache->EndpointConfigurationClass, "<init>", "(Ljava/lang/String;Ljava/lang/String;)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->EndpointConfigurationConstructor);

	cache->ErrorTypesConstructor = (*env).GetMethodID(cache->ErrorTypesClass, "<init>", "(ZZZZ)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->ErrorTypesConstructor);

	cache->HashMapConstructor = (*env).GetMethodID(cache->HashMapClass, "<init>", "()V");
	ReturnFalseIfNullAndClearExceptions(env, cache->HashMapConstructor);

	cache->NotifierConstructor = (*env).GetMethodID(cache->NotifierClass, "<init>", "()V");
	ReturnFalseIfNullAndClearExceptions(env, cache->NotifierConstructor);
	cache->NotifierSetDependencies = (*env).GetMethodID(cache->NotifierClass, "setDependencies", "(Ljava/util/List;)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->NotifierSetDependencies);
	cache->NotifierSetName = (*env).GetMethodID(cache->NotifierClass, "setName", "(Ljava/lang/String;)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->NotifierSetName);
	cache->NotifierSetUrl = (*env).GetMethodID(cache->NotifierClass, "setUrl", "(Ljava/lang/String;)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->NotifierSetUrl);
	cache->NotifierSetVersion = (*env).GetMethodID(cache->NotifierClass, "setVersion", "(Ljava/lang/String;)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->NotifierSetVersion);

	cache->ArrayListConstructor = (*env).GetMethodID(cache->ArrayListClass, "<init>", "()V");
	ReturnFalseIfNullAndClearExceptions(env, cache->ArrayListConstructor);
	cache->ArrayListAdd = (*env).GetMethodID(cache->ArrayListClass, "add", "(Ljava/lang/Object;)Z");
	ReturnFalseIfNullAndClearExceptions(env, cache->ArrayListAdd);

	cache->IntegerConstructor = (*env).GetMethodID(cache->IntegerClass, "<init>", "(I)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->IntegerConstructor);

	cache->HashSetConstructor = (*env).GetMethodID(cache->HashSetClass, "<init>", "()V");
	ReturnFalseIfNullAndClearExceptions(env, cache->HashSetConstructor);
	cache->HashSetAdd = (*env).GetMethodID(cache->HashSetClass, "add", "(Ljava/lang/Object;)Z");
	ReturnFalseIfNullAndClearExceptions(env, cache->HashSetAdd);

	cache->TraceConstructor = (*env).GetMethodID(cache->TraceClass, "<init>",
		"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V");
	ReturnFalseIfNullAndClearExceptions(env, cache->TraceConstructor);
	cache->SeverityFieldInfo = (*env).GetStaticFieldID(cache->SeverityClass, "INFO", "Lcom/bugsnag/android/Severity;");
	ReturnFalseIfNullAndClearExceptions(env, cache->SeverityFieldInfo);
	cache->SeverityFieldWarning = (*env).GetStaticFieldID(cache->SeverityClass, "WARNING", "Lcom/bugsnag/android/Severity;");
	ReturnFalseIfNullAndClearExceptions(env, cache->SeverityFieldWarning);
	cache->SeverityFieldError = (*env).GetStaticFieldID(cache->SeverityClass, "ERROR", "Lcom/bugsnag/android/Severity;");
	ReturnFalseIfNullAndClearExceptions(env, cache->SeverityFieldError);
	cache->BreadcrumbTypeError = (*env).GetStaticFieldID(cache->BreadcrumbTypeClass, "ERROR", "Lcom/bugsnag/android/BreadcrumbType;");
	ReturnFalseIfNullAndClearExceptions(env, cache->BreadcrumbTypeError);
	cache->BreadcrumbTypeLog = (*env).GetStaticFieldID(cache->BreadcrumbTypeClass, "LOG", "Lcom/bugsnag/android/BreadcrumbType;");
	ReturnFalseIfNullAndClearExceptions(env, cache->BreadcrumbTypeLog);
	cache->BreadcrumbTypeManual = (*env).GetStaticFieldID(cache->BreadcrumbTypeClass, "MANUAL", "Lcom/bugsnag/android/BreadcrumbType;");
	ReturnFalseIfNullAndClearExceptions(env, cache->BreadcrumbTypeManual);
	cache->BreadcrumbTypeNavigation = (*env).GetStaticFieldID(cache->BreadcrumbTypeClass, "NAVIGATION", "Lcom/bugsnag/android/BreadcrumbType;");
	ReturnFalseIfNullAndClearExceptions(env, cache->BreadcrumbTypeNavigation);
	cache->BreadcrumbTypeProcess = (*env).GetStaticFieldID(cache->BreadcrumbTypeClass, "PROCESS", "Lcom/bugsnag/android/BreadcrumbType;");
	ReturnFalseIfNullAndClearExceptions(env, cache->BreadcrumbTypeProcess);
	cache->BreadcrumbTypeRequest = (*env).GetStaticFieldID(cache->BreadcrumbTypeClass, "REQUEST", "Lcom/bugsnag/android/BreadcrumbType;");
	ReturnFalseIfNullAndClearExceptions(env, cache->BreadcrumbTypeRequest);
	cache->BreadcrumbTypeState = (*env).GetStaticFieldID(cache->BreadcrumbTypeClass, "STATE", "Lcom/bugsnag/android/BreadcrumbType;");
	ReturnFalseIfNullAndClearExceptions(env, cache->BreadcrumbTypeState);
	cache->BreadcrumbTypeUser = (*env).GetStaticFieldID(cache->BreadcrumbTypeClass, "USER", "Lcom/bugsnag/android/BreadcrumbType;");
	ReturnFalseIfNullAndClearExceptions(env, cache->BreadcrumbTypeUser);

	cache->ThreadSendPolicyAlways = (*env).GetStaticFieldID(cache->ThreadSendPolicyClass, "ALWAYS", "Lcom/bugsnag/android/ThreadSendPolicy;");
	ReturnFalseIfNullAndClearExceptions(env, cache->ThreadSendPolicyAlways);
	cache->ThreadSendPolicyUnhandledOnly = (*env).GetStaticFieldID(cache->ThreadSendPolicyClass, "UNHANDLED_ONLY", "Lcom/bugsnag/android/ThreadSendPolicy;");
	ReturnFalseIfNullAndClearExceptions(env, cache->ThreadSendPolicyUnhandledOnly);
	cache->ThreadSendPolicyNever = (*env).GetStaticFieldID(cache->ThreadSendPolicyClass, "NEVER", "Lcom/bugsnag/android/ThreadSendPolicy;");
	ReturnFalseIfNullAndClearExceptions(env, cache->ThreadSendPolicyNever);

	return true;
}

jstring FAndroidPlatformJNI::ParseFString(JNIEnv* Env, const FString& Text)
{
	const char* rawText = TCHAR_TO_UTF8(*Text);
	if (rawText)
	{
		jstring jText = (*Env).NewStringUTF(rawText);
		if (FAndroidPlatformJNI::CheckAndClearException(Env))
		{
			return NULL;
		}
		return jText;
	}
	return NULL;
}

jstring FAndroidPlatformJNI::ParseFStringPtr(JNIEnv* Env, const TSharedPtr<FString>& Text)
{
	if (Text.IsValid())
	{
		return ParseFString(Env, *Text);
	}
	return NULL;
}

bool FAndroidPlatformJNI::CheckAndClearException(JNIEnv* Env)
{
	if ((*Env).ExceptionCheck())
	{
		(*Env).ExceptionClear();
		return true;
	}
	return false;
}

jboolean FAndroidPlatformJNI::ParseBoolean(bool value)
{
	return value ? JNI_TRUE : JNI_FALSE;
}

jobject FAndroidPlatformJNI::ParseStringSet(JNIEnv* Env, const JNIReferenceCache* Cache, const TArray<FString>& Values)
{
	jobject jSet = (*Env).NewObject(Cache->HashSetClass, Cache->HashSetConstructor);
	if (FAndroidPlatformJNI::CheckAndClearException(Env))
	{
		return nullptr;
	}
	for (const FString& Item : Values)
	{
		jstring jStr = FAndroidPlatformJNI::ParseFString(Env, Item);
		if (jStr)
		{
			(*Env).CallBooleanMethod(jSet, Cache->HashSetAdd, jStr);
			if (FAndroidPlatformJNI::CheckAndClearException(Env))
			{
				return nullptr;
			}
		}
	}
	return jSet;
}

jobject FAndroidPlatformJNI::ParseJsonObject(JNIEnv* Env, const JNIReferenceCache* Cache, const TSharedPtr<FJsonObject>& Object)
{
	if (!Object.IsValid())
	{
		return nullptr;
	}
	FLargeMemoryWriter Archive;
	TSharedRef<TJsonWriter<char>> JsonWriter = TJsonWriterFactory<char>::Create(&Archive);
	if (!FJsonSerializer::Serialize(Object.ToSharedRef(), JsonWriter))
	{
		return nullptr;
	}
	JsonWriter->Close();
	jbyteArray jBytes = (*Env).NewByteArray(Archive.TotalSize());
	ReturnNullOnException(Env);
	(*Env).SetByteArrayRegion(jBytes, 0, Archive.TotalSize(), (const jbyte*)Archive.GetData());
	ReturnNullOnException(Env);
	jobject jObject = (*Env).CallStaticObjectMethod(Cache->MetadataParserClass, Cache->MetadataParserParse, jBytes);
	ReturnNullOnException(Env);
	(*Env).DeleteLocalRef(jBytes);
	return jObject;
}

/**
 * Adds a value to a set if enabled
 *
 * @param Env       A JNI environment for the current thread
 * @param ShouldAdd true if the value should be added
 * @param TypeClass the enum class
 * @param FieldName the name of the field to add to the set
 *
 * @return true if operation completed without error
 */
static bool addTypeToSet(JNIEnv* Env, jobject jSet, bool ShouldAdd, const JNIReferenceCache* Cache, jfieldID FieldName)
{
	if (ShouldAdd)
	{
		jobject jType = (*Env).GetStaticObjectField(Cache->BreadcrumbTypeClass, FieldName);
		if (FAndroidPlatformJNI::CheckAndClearException(Env))
		{
			return false;
		}
		(*Env).CallBooleanMethod(jSet, Cache->HashSetAdd, jType);
		if (FAndroidPlatformJNI::CheckAndClearException(Env))
		{
			return false;
		}
	}
	return true;
}

static jfieldID breadcrumbFieldForType(const JNIReferenceCache* Cache, EBugsnagBreadcrumbType Type)
{
	switch (Type)
	{
	case EBugsnagBreadcrumbType::Manual:
		return Cache->BreadcrumbTypeManual;
	case EBugsnagBreadcrumbType::Error:
		return Cache->BreadcrumbTypeError;
	case EBugsnagBreadcrumbType::Log:
		return Cache->BreadcrumbTypeLog;
	case EBugsnagBreadcrumbType::Navigation:
		return Cache->BreadcrumbTypeNavigation;
	case EBugsnagBreadcrumbType::Process:
		return Cache->BreadcrumbTypeProcess;
	case EBugsnagBreadcrumbType::Request:
		return Cache->BreadcrumbTypeRequest;
	case EBugsnagBreadcrumbType::State:
		return Cache->BreadcrumbTypeState;
	case EBugsnagBreadcrumbType::User:
		return Cache->BreadcrumbTypeUser;
	}
}

jobject FAndroidPlatformJNI::ParseBreadcrumbType(JNIEnv* Env, const JNIReferenceCache* Cache, EBugsnagBreadcrumbType Type)
{
	jfieldID jType = breadcrumbFieldForType(Cache, Type);
	jobject result = (*Env).GetStaticObjectField(Cache->BreadcrumbTypeClass, jType);
	ReturnNullOnException(Env);
	return result;
}

jobject FAndroidPlatformJNI::ParseBreadcrumbTypeSet(JNIEnv* Env, const JNIReferenceCache* Cache, const FBugsnagEnabledBreadcrumbTypes Value)
{
	jobject jSet = (*Env).NewObject(Cache->HashSetClass, Cache->HashSetConstructor);
	if (FAndroidPlatformJNI::CheckAndClearException(Env))
	{
		return nullptr;
	}
	if (addTypeToSet(Env, jSet, Value.bError, Cache, Cache->BreadcrumbTypeError) &&
		addTypeToSet(Env, jSet, Value.bLog, Cache, Cache->BreadcrumbTypeLog) &&
		addTypeToSet(Env, jSet, Value.bManual, Cache, Cache->BreadcrumbTypeManual) &&
		addTypeToSet(Env, jSet, Value.bNavigation, Cache, Cache->BreadcrumbTypeNavigation) &&
		addTypeToSet(Env, jSet, Value.bProcess, Cache, Cache->BreadcrumbTypeProcess) &&
		addTypeToSet(Env, jSet, Value.bRequest, Cache, Cache->BreadcrumbTypeRequest) &&
		addTypeToSet(Env, jSet, Value.bState, Cache, Cache->BreadcrumbTypeState) &&
		addTypeToSet(Env, jSet, Value.bUser, Cache, Cache->BreadcrumbTypeUser))
	{
		return jSet;
	}
	return nullptr;
}

jobject FAndroidPlatformJNI::ParseInteger(JNIEnv* Env, const JNIReferenceCache* Cache, int Value)
{
	jobject jValue = (*Env).NewObject(Cache->IntegerClass, Cache->IntegerConstructor, Value);
	if (FAndroidPlatformJNI::CheckAndClearException(Env))
	{
		return nullptr;
	}
	return jValue;
}

jobject FAndroidPlatformJNI::ParseSeverity(JNIEnv* Env, const JNIReferenceCache* Cache, const EBugsnagSeverity Value)
{
	jfieldID FieldName;
	switch (Value)
	{
	case EBugsnagSeverity::Info:
		FieldName = Cache->SeverityFieldInfo;
		break;
	case EBugsnagSeverity::Warning:
		FieldName = Cache->SeverityFieldWarning;
		break;
	case EBugsnagSeverity::Error:
		FieldName = Cache->SeverityFieldError;
		break;
	default:
		return nullptr;
	}
	jobject jSeverity = (*Env).GetStaticObjectField(Cache->SeverityClass, FieldName);
	if (FAndroidPlatformJNI::CheckAndClearException(Env))
	{
		return nullptr;
	}
	return jSeverity;
}

jobject FAndroidPlatformJNI::ParseThreadSendPolicy(JNIEnv* Env, const JNIReferenceCache* Cache, const EBugsnagSendThreadsPolicy Policy)
{
	jfieldID FieldName;
	switch (Policy)
	{
	case EBugsnagSendThreadsPolicy::All:
		FieldName = Cache->ThreadSendPolicyAlways;
		break;
	case EBugsnagSendThreadsPolicy::Never:
		FieldName = Cache->ThreadSendPolicyNever;
		break;
	case EBugsnagSendThreadsPolicy::UnhandledOnly:
		FieldName = Cache->ThreadSendPolicyUnhandledOnly;
		break;
	default:
		return nullptr;
	}
	jobject jPolicy = (*Env).GetStaticObjectField(Cache->ThreadSendPolicyClass, FieldName);
	if (FAndroidPlatformJNI::CheckAndClearException(Env))
	{
		return nullptr;
	}
	return jPolicy;
}

bool FAndroidPlatformJNI::SetNotifierInfo(JNIEnv* Env, const JNIReferenceCache* Cache, jobject jConfig)
{
	jobject jNotifier = (*Env).CallObjectMethod(jConfig, Cache->ConfigGetNotifier);
	ReturnFalseOnException(Env);
	// default notifier object has the name, url, and version of the underlying library
	jobject jDependency = (*Env).NewObject(Cache->NotifierClass, Cache->NotifierConstructor);
	ReturnFalseIfNullAndClearExceptions(Env, jDependency);
	jobject jDependencies = (*Env).NewObject(Cache->ArrayListClass, Cache->ArrayListConstructor);
	ReturnFalseIfNullAndClearExceptions(Env, jDependencies);
	(*Env).CallBooleanMethod(jDependencies, Cache->ArrayListAdd, jDependency);
	ReturnFalseOnException(Env);
	jstring jName = (*Env).NewStringUTF(BUGSNAG_UNREAL_NAME);
	ReturnFalseIfNullAndClearExceptions(Env, jName);
	jstring jVersion = (*Env).NewStringUTF(BUGSNAG_UNREAL_VERSION_STRING);
	ReturnFalseIfNullAndClearExceptions(Env, jVersion);
	jstring jUrl = (*Env).NewStringUTF(BUGSNAG_UNREAL_URL);
	ReturnFalseIfNullAndClearExceptions(Env, jUrl);
	(*Env).CallVoidMethod(jNotifier, Cache->NotifierSetName, jName);
	ReturnFalseOnException(Env);
	(*Env).CallVoidMethod(jNotifier, Cache->NotifierSetVersion, jVersion);
	ReturnFalseOnException(Env);
	(*Env).CallVoidMethod(jNotifier, Cache->NotifierSetUrl, jUrl);
	ReturnFalseOnException(Env);
	(*Env).CallVoidMethod(jNotifier, Cache->NotifierSetDependencies, jDependencies);
	ReturnFalseOnException(Env);

	return true;
}
