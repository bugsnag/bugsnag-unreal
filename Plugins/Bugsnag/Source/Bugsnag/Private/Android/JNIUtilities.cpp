// Copyright 2021 Bugsnag. All Rights Reserved.

#include "JNIUtilities.h"

#include "BugsnagUser.h"
#include "Version.h"

#include <string>

#include "Android/AndroidJavaEnv.h"
#include "CoreMinimal.h"
#include "Shorthand.h"

#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/LargeMemoryReader.h"
#include "Serialization/LargeMemoryWriter.h"

#if 0
#include <android/log.h>

#define DebugLogNull(destination, name) \
	if (destination == NULL)            \
		__android_log_print(ANDROID_LOG_WARN, "BugsnagUE4", "failed to load %s", name);
#else
#define DebugLogNull(destination, name)
#endif

#define CacheInstanceJavaMethod(env, destination, clazz, name, signature) \
	destination = (*env).GetMethodID(clazz, name, signature);             \
	DebugLogNull(destination, name);                                      \
	ReturnFalseIfNullAndClearExceptions(env, destination);

#define CacheStaticJavaMethod(env, destination, clazz, name, signature) \
	destination = (*env).GetStaticMethodID(clazz, name, signature);     \
	DebugLogNull(destination, name);                                    \
	ReturnFalseIfNullAndClearExceptions(env, destination);

#define CacheStaticJavaField(env, destination, clazz, name, signature) \
	destination = (*env).GetStaticFieldID(clazz, name, signature);     \
	DebugLogNull(destination, name);                                   \
	ReturnFalseIfNullAndClearExceptions(env, destination);

// Qualified names for system classes are formatted as 'com/example/ClassName'
#define CacheSystemJavaClass(env, destination, qualified_name)                   \
	destination = (jclass)(*env).NewGlobalRef((*env).FindClass(qualified_name)); \
	DebugLogNull(destination, qualified_name);                                   \
	ReturnFalseIfNullAndClearExceptions(env, destination);

// Qualified names for external classes are formatted as 'com.example.ClassName'
// Third-party classes are loaded via a class loader accessible to the Unreal JNI helpers
#define CacheExternalJavaClass(env, destination, qualified_name)          \
	destination = AndroidJavaEnv::FindJavaClassGlobalRef(qualified_name); \
	DebugLogNull(destination, qualified_name);                            \
	ReturnFalseIfNullAndClearExceptions(env, destination);

bool FAndroidPlatformJNI::LoadReferenceCache(JNIEnv* env, JNIReferenceCache* cache)
{
	if (!env)
	{
		return false;
	}

	CacheExternalJavaClass(env, cache->AppClass, "com.bugsnag.android.App");
	CacheExternalJavaClass(env, cache->AppWithStateClass, "com.bugsnag.android.AppWithState");
	CacheExternalJavaClass(env, cache->BreadcrumbClass, "com.bugsnag.android.Breadcrumb");
	CacheExternalJavaClass(env, cache->BugsnagClass, "com.bugsnag.android.Bugsnag");
	CacheExternalJavaClass(env, cache->BugsnagUnrealPluginClass, "com.bugsnag.android.unreal.UnrealPlugin");
	CacheExternalJavaClass(env, cache->ClientClass, "com.bugsnag.android.Client");
	CacheExternalJavaClass(env, cache->ConfigClass, "com.bugsnag.android.Configuration");
	CacheExternalJavaClass(env, cache->DeviceClass, "com.bugsnag.android.Device");
	CacheExternalJavaClass(env, cache->DeviceWithStateClass, "com.bugsnag.android.DeviceWithState");
	CacheExternalJavaClass(env, cache->ErrorClass, "com.bugsnag.android.Error");
	CacheExternalJavaClass(env, cache->ErrorTypeClass, "com.bugsnag.android.ErrorType");
	CacheExternalJavaClass(env, cache->ErrorTypesClass, "com.bugsnag.android.ErrorTypes");
	CacheExternalJavaClass(env, cache->EventClass, "com.bugsnag.android.Event");
	CacheExternalJavaClass(env, cache->LastRunInfoClass, "com.bugsnag.android.LastRunInfo");
	CacheExternalJavaClass(env, cache->NotifierClass, "com.bugsnag.android.Notifier");
	CacheExternalJavaClass(env, cache->BreadcrumbTypeClass, "com.bugsnag.android.BreadcrumbType");
	CacheExternalJavaClass(env, cache->SessionClass, "com.bugsnag.android.Session");
	CacheExternalJavaClass(env, cache->SeverityClass, "com.bugsnag.android.Severity");
	CacheExternalJavaClass(env, cache->StackframeClass, "com.bugsnag.android.Stackframe");
	CacheExternalJavaClass(env, cache->ThreadClass, "com.bugsnag.android.Thread");
	CacheExternalJavaClass(env, cache->UserClass, "com.bugsnag.android.User");
	CacheExternalJavaClass(env, cache->EndpointConfigurationClass, "com.bugsnag.android.EndpointConfiguration");
	CacheExternalJavaClass(env, cache->ThreadSendPolicyClass, "com.bugsnag.android.ThreadSendPolicy");
	CacheExternalJavaClass(env, cache->ThreadTypeClass, "com.bugsnag.android.ThreadType");
	CacheExternalJavaClass(env, cache->MetadataParserClass, "com.bugsnag.android.unreal.MetadataParser");
	CacheExternalJavaClass(env, cache->MetadataSerializerClass, "com.bugsnag.android.unreal.MetadataSerializer");

	CacheSystemJavaClass(env, cache->ContextClass, "android/content/Context");
	CacheSystemJavaClass(env, cache->DateClass, "java/util/Date");
	CacheSystemJavaClass(env, cache->EnumClass, "java/lang/Enum");
	CacheSystemJavaClass(env, cache->HashMapClass, "java/util/HashMap");
	CacheSystemJavaClass(env, cache->ArrayListClass, "java/util/ArrayList");
	CacheSystemJavaClass(env, cache->BooleanClass, "java/lang/Boolean");
	CacheSystemJavaClass(env, cache->FileClass, "java/io/File");
	CacheSystemJavaClass(env, cache->HashSetClass, "java/util/HashSet");
	CacheSystemJavaClass(env, cache->IntegerClass, "java/lang/Integer");
	CacheSystemJavaClass(env, cache->ListClass, "java/util/List");
	CacheSystemJavaClass(env, cache->LongClass, "java/lang/Long");
	CacheSystemJavaClass(env, cache->MapClass, "java/util/Map");
	CacheSystemJavaClass(env, cache->NumberClass, "java/lang/Number");
	CacheSystemJavaClass(env, cache->StringClass, "java/lang/String");
	CacheSystemJavaClass(env, cache->TraceClass, "java/lang/StackTraceElement");

	CacheInstanceJavaMethod(env, cache->AppGetBinaryArch, cache->AppClass, "getBinaryArch", "()Ljava/lang/String;");
	CacheInstanceJavaMethod(env, cache->AppGetBuildUuid, cache->AppClass, "getBuildUuid", "()Ljava/lang/String;");
	CacheInstanceJavaMethod(env, cache->AppGetId, cache->AppClass, "getId", "()Ljava/lang/String;");
	CacheInstanceJavaMethod(env, cache->AppGetReleaseStage, cache->AppClass, "getReleaseStage", "()Ljava/lang/String;");
	CacheInstanceJavaMethod(env, cache->AppGetType, cache->AppClass, "getType", "()Ljava/lang/String;");
	CacheInstanceJavaMethod(env, cache->AppGetVersion, cache->AppClass, "getVersion", "()Ljava/lang/String;");
	CacheInstanceJavaMethod(env, cache->AppGetVersionCode, cache->AppClass, "getVersionCode", "()Ljava/lang/Number;");
	CacheInstanceJavaMethod(env, cache->AppSetBinaryArch, cache->AppClass, "setBinaryArch", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->AppSetBuildUuid, cache->AppClass, "setBuildUuid", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->AppSetId, cache->AppClass, "setId", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->AppSetReleaseStage, cache->AppClass, "setReleaseStage", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->AppSetType, cache->AppClass, "setType", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->AppSetVersion, cache->AppClass, "setVersion", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->AppSetVersionCode, cache->AppClass, "setVersionCode", "(Ljava/lang/Number;)V");

	CacheInstanceJavaMethod(env, cache->AppWithStateGetDuration, cache->AppWithStateClass, "getDuration", "()Ljava/lang/Number;");
	CacheInstanceJavaMethod(env, cache->AppWithStateGetDurationInForeground, cache->AppWithStateClass, "getDurationInForeground", "()Ljava/lang/Number;");
	CacheInstanceJavaMethod(env, cache->AppWithStateGetInForeground, cache->AppWithStateClass, "getInForeground", "()Ljava/lang/Boolean;");
	CacheInstanceJavaMethod(env, cache->AppWithStateGetIsLaunching, cache->AppWithStateClass, "isLaunching", "()Ljava/lang/Boolean;");
	CacheInstanceJavaMethod(env, cache->AppWithStateSetDuration, cache->AppWithStateClass, "setDuration", "(Ljava/lang/Number;)V");
	CacheInstanceJavaMethod(env, cache->AppWithStateSetDurationInForeground, cache->AppWithStateClass, "setDurationInForeground", "(Ljava/lang/Number;)V");
	CacheInstanceJavaMethod(env, cache->AppWithStateSetInForeground, cache->AppWithStateClass, "setInForeground", "(Ljava/lang/Boolean;)V");
	CacheInstanceJavaMethod(env, cache->AppWithStateSetIsLaunching, cache->AppWithStateClass, "setLaunching", "(Ljava/lang/Boolean;)V");

	CacheInstanceJavaMethod(env, cache->BreadcrumbGetMessage, cache->BreadcrumbClass, "getMessage", "()Ljava/lang/String;");
	CacheInstanceJavaMethod(env, cache->BreadcrumbGetMetadata, cache->BreadcrumbClass, "getMetadata", "()Ljava/util/Map;");
	CacheInstanceJavaMethod(env, cache->BreadcrumbGetTimestamp, cache->BreadcrumbClass, "getTimestamp", "()Ljava/util/Date;");
	CacheInstanceJavaMethod(env, cache->BreadcrumbGetType, cache->BreadcrumbClass, "getType", "()Lcom/bugsnag/android/BreadcrumbType;");
	CacheInstanceJavaMethod(env, cache->BreadcrumbSetMessage, cache->BreadcrumbClass, "setMessage", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->BreadcrumbSetMetadata, cache->BreadcrumbClass, "setMetadata", "(Ljava/util/Map;)V");
	CacheInstanceJavaMethod(env, cache->BreadcrumbSetType, cache->BreadcrumbClass, "setType", "(Lcom/bugsnag/android/BreadcrumbType;)V");

	CacheStaticJavaMethod(env, cache->BugsnagClearMetadataSection, cache->BugsnagClass, "clearMetadata", "(Ljava/lang/String;)V");
	CacheStaticJavaMethod(env, cache->BugsnagClearMetadataValue, cache->BugsnagClass, "clearMetadata", "(Ljava/lang/String;Ljava/lang/String;)V");
	CacheStaticJavaMethod(env, cache->BugsnagAddFeatureFlag, cache->BugsnagClass, "addFeatureFlag", "(Ljava/lang/String;Ljava/lang/String;)V");
	CacheStaticJavaMethod(env, cache->BugsnagAddMetadataToSection, cache->BugsnagClass, "addMetadata", "(Ljava/lang/String;Ljava/util/Map;)V");
	CacheStaticJavaMethod(env, cache->BugsnagAddMetadataValue, cache->BugsnagClass, "addMetadata", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/Object;)V");
	CacheStaticJavaMethod(env, cache->BugsnagClearFeatureFlag, cache->BugsnagClass, "clearFeatureFlag", "(Ljava/lang/String;)V");
	CacheStaticJavaMethod(env, cache->BugsnagClearFeatureFlags, cache->BugsnagClass, "clearFeatureFlags", "()V");
	CacheStaticJavaMethod(env, cache->BugsnagGetBreadcrumbs, cache->BugsnagClass, "getBreadcrumbs", "()Ljava/util/List;");
	CacheStaticJavaMethod(env, cache->BugsnagGetContext, cache->BugsnagClass, "getContext", "()Ljava/lang/String;");
	CacheStaticJavaMethod(env, cache->BugsnagGetLastRunInfo, cache->BugsnagClass, "getLastRunInfo", "()Lcom/bugsnag/android/LastRunInfo;");
	CacheStaticJavaMethod(env, cache->BugsnagGetUser, cache->BugsnagClass, "getUser", "()Lcom/bugsnag/android/User;");
	CacheStaticJavaMethod(env, cache->BugsnagStartMethod, cache->BugsnagClass, "start", "(Landroid/content/Context;Lcom/bugsnag/android/Configuration;)Lcom/bugsnag/android/Client;");
	CacheStaticJavaMethod(env, cache->BugsnagSetContext, cache->BugsnagClass, "setContext", "(Ljava/lang/String;)V");
	CacheStaticJavaMethod(env, cache->BugsnagLeaveBreadcrumb, cache->BugsnagClass, "leaveBreadcrumb", "(Ljava/lang/String;Ljava/util/Map;Lcom/bugsnag/android/BreadcrumbType;)V");
	CacheStaticJavaMethod(env, cache->BugsnagMarkLaunchCompleted, cache->BugsnagClass, "markLaunchCompleted", "()V");
	CacheStaticJavaMethod(env, cache->BugsnagStartSession, cache->BugsnagClass, "startSession", "()V");
	CacheStaticJavaMethod(env, cache->BugsnagResumeSession, cache->BugsnagClass, "resumeSession", "()Z");
	CacheStaticJavaMethod(env, cache->BugsnagSetUser, cache->BugsnagClass, "setUser", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
	CacheStaticJavaMethod(env, cache->BugsnagPauseSession, cache->BugsnagClass, "pauseSession", "()V");

	CacheStaticJavaMethod(env, cache->BugsnagUnrealPluginGetEventMetadataSection, cache->BugsnagUnrealPluginClass, "getMetadata", "(Lcom/bugsnag/android/Event;Ljava/lang/String;)[B");
	CacheStaticJavaMethod(env, cache->BugsnagUnrealPluginGetEventMetadataValue, cache->BugsnagUnrealPluginClass, "getMetadata", "(Lcom/bugsnag/android/Event;Ljava/lang/String;Ljava/lang/String;)[B");
	CacheStaticJavaMethod(env, cache->BugsnagUnrealPluginGetMetadataSection, cache->BugsnagUnrealPluginClass, "getMetadata", "(Ljava/lang/String;)[B");
	CacheStaticJavaMethod(env, cache->BugsnagUnrealPluginGetMetadataValue, cache->BugsnagUnrealPluginClass, "getMetadata", "(Ljava/lang/String;Ljava/lang/String;)[B");
	CacheStaticJavaMethod(env, cache->BugsnagUnrealPluginNotify, cache->BugsnagUnrealPluginClass, "notify", "(Ljava/lang/String;Ljava/lang/String;[Ljava/lang/StackTraceElement;Ljava/nio/ByteBuffer;)V");
	CacheInstanceJavaMethod(env, cache->BugsnagUnrealPluginConstructor, cache->BugsnagUnrealPluginClass, "<init>", "(Lcom/bugsnag/android/Configuration;)V");

	CacheStaticJavaMethod(env, cache->MetadataParserParse, cache->MetadataParserClass, "parse", "([B)Ljava/util/Map;");

	CacheStaticJavaMethod(env, cache->MetadataSerializerSerialize, cache->MetadataSerializerClass, "serialize", "(Ljava/util/Map;)[B");

	CacheInstanceJavaMethod(env, cache->ClientAddRuntimeVersionInfo, cache->ClientClass, "addRuntimeVersionInfo", "(Ljava/lang/String;Ljava/lang/String;)V");

	CacheInstanceJavaMethod(env, cache->ConfigConstructor, cache->ConfigClass, "<init>", "(Ljava/lang/String;)V");

	CacheInstanceJavaMethod(env, cache->ConfigAddFeatureFlag, cache->ConfigClass, "addFeatureFlag", "(Ljava/lang/String;Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->ConfigAddMetadata, cache->ConfigClass, "addMetadata", "(Ljava/lang/String;Ljava/util/Map;)V");
	CacheInstanceJavaMethod(env, cache->ConfigGetNotifier, cache->ConfigClass, "getNotifier", "()Lcom/bugsnag/android/Notifier;");
	CacheInstanceJavaMethod(env, cache->ConfigAddPlugin, cache->ConfigClass, "addPlugin", "(Lcom/bugsnag/android/Plugin;)V");
	CacheInstanceJavaMethod(env, cache->ConfigClearFeatureFlag, cache->ConfigClass, "clearFeatureFlag", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->ConfigClearFeatureFlags, cache->ConfigClass, "clearFeatureFlags", "()V");
	CacheInstanceJavaMethod(env, cache->ConfigSetAppType, cache->ConfigClass, "setAppType", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->ConfigSetAppVersion, cache->ConfigClass, "setAppVersion", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->ConfigSetAutoDetectErrors, cache->ConfigClass, "setAutoDetectErrors", "(Z)V");
	CacheInstanceJavaMethod(env, cache->ConfigSetAutoTrackSessions, cache->ConfigClass, "setAutoTrackSessions", "(Z)V");
	CacheInstanceJavaMethod(env, cache->ConfigSetContext, cache->ConfigClass, "setContext", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->ConfigSetDiscardClasses, cache->ConfigClass, "setDiscardClasses", "(Ljava/util/Set;)V");
	CacheInstanceJavaMethod(env, cache->ConfigSetEnabledBreadcrumbTypes, cache->ConfigClass, "setEnabledBreadcrumbTypes", "(Ljava/util/Set;)V");
	CacheInstanceJavaMethod(env, cache->ConfigSetEnabledErrorTypes, cache->ConfigClass, "setEnabledErrorTypes", "(Lcom/bugsnag/android/ErrorTypes;)V");
	CacheInstanceJavaMethod(env, cache->ConfigSetEnabledReleaseStages, cache->ConfigClass, "setEnabledReleaseStages", "(Ljava/util/Set;)V");
	CacheInstanceJavaMethod(env, cache->ConfigSetEndpoints, cache->ConfigClass, "setEndpoints", "(Lcom/bugsnag/android/EndpointConfiguration;)V");
	CacheInstanceJavaMethod(env, cache->ConfigSetLaunchDurationMillis, cache->ConfigClass, "setLaunchDurationMillis", "(J)V");
	CacheInstanceJavaMethod(env, cache->ConfigSetMaxBreadcrumbs, cache->ConfigClass, "setMaxBreadcrumbs", "(I)V");
	CacheInstanceJavaMethod(env, cache->ConfigSetMaxPersistedEvents, cache->ConfigClass, "setMaxPersistedEvents", "(I)V");
	CacheInstanceJavaMethod(env, cache->ConfigSetMaxPersistedSessions, cache->ConfigClass, "setMaxPersistedSessions", "(I)V");
	CacheInstanceJavaMethod(env, cache->ConfigSetPersistenceDirectory, cache->ConfigClass, "setPersistenceDirectory", "(Ljava/io/File;)V");
	CacheInstanceJavaMethod(env, cache->ConfigSetPersistUser, cache->ConfigClass, "setPersistUser", "(Z)V");
	CacheInstanceJavaMethod(env, cache->ConfigSetProjectPackages, cache->ConfigClass, "setProjectPackages", "(Ljava/util/Set;)V");
	CacheInstanceJavaMethod(env, cache->ConfigSetRedactedKeys, cache->ConfigClass, "setRedactedKeys", "(Ljava/util/Set;)V");
	CacheInstanceJavaMethod(env, cache->ConfigSetReleaseStage, cache->ConfigClass, "setReleaseStage", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->ConfigSetSendLaunchCrashesSynchronously, cache->ConfigClass, "setSendLaunchCrashesSynchronously", "(Z)V");
	CacheInstanceJavaMethod(env, cache->ConfigSetSendThreads, cache->ConfigClass, "setSendThreads", "(Lcom/bugsnag/android/ThreadSendPolicy;)V");
	CacheInstanceJavaMethod(env, cache->ConfigSetUser, cache->ConfigClass, "setUser", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->ConfigSetVersionCode, cache->ConfigClass, "setVersionCode", "(Ljava/lang/Integer;)V");

	CacheInstanceJavaMethod(env, cache->ContextGetApplication, cache->ContextClass, "getApplicationContext", "()Landroid/content/Context;");

	CacheInstanceJavaMethod(env, cache->DateConstructor, cache->DateClass, "<init>", "(J)V");
	CacheInstanceJavaMethod(env, cache->DateGetTime, cache->DateClass, "getTime", "()J");

	CacheInstanceJavaMethod(env, cache->DeviceGetCpuAbi, cache->DeviceClass, "getCpuAbi", "()[Ljava/lang/String;");
	CacheInstanceJavaMethod(env, cache->DeviceGetId, cache->DeviceClass, "getId", "()Ljava/lang/String;");
	CacheInstanceJavaMethod(env, cache->DeviceGetJailbroken, cache->DeviceClass, "getJailbroken", "()Ljava/lang/Boolean;");
	CacheInstanceJavaMethod(env, cache->DeviceGetLocale, cache->DeviceClass, "getLocale", "()Ljava/lang/String;");
	CacheInstanceJavaMethod(env, cache->DeviceGetManufacturer, cache->DeviceClass, "getManufacturer", "()Ljava/lang/String;");
	CacheInstanceJavaMethod(env, cache->DeviceGetModel, cache->DeviceClass, "getModel", "()Ljava/lang/String;");
	CacheInstanceJavaMethod(env, cache->DeviceGetOsName, cache->DeviceClass, "getOsName", "()Ljava/lang/String;");
	CacheInstanceJavaMethod(env, cache->DeviceGetOsVersion, cache->DeviceClass, "getOsVersion", "()Ljava/lang/String;");
	CacheInstanceJavaMethod(env, cache->DeviceGetRuntimeVersions, cache->DeviceClass, "getRuntimeVersions", "()Ljava/util/Map;");
	CacheInstanceJavaMethod(env, cache->DeviceGetTotalMemory, cache->DeviceClass, "getTotalMemory", "()Ljava/lang/Long;");
	CacheInstanceJavaMethod(env, cache->DeviceSetCpuAbi, cache->DeviceClass, "setCpuAbi", "([Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->DeviceSetId, cache->DeviceClass, "setId", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->DeviceSetJailbroken, cache->DeviceClass, "setJailbroken", "(Ljava/lang/Boolean;)V");
	CacheInstanceJavaMethod(env, cache->DeviceSetLocale, cache->DeviceClass, "setLocale", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->DeviceSetManufacturer, cache->DeviceClass, "setManufacturer", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->DeviceSetModel, cache->DeviceClass, "setModel", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->DeviceSetOsName, cache->DeviceClass, "setOsName", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->DeviceSetOsVersion, cache->DeviceClass, "setOsVersion", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->DeviceSetRuntimeVersions, cache->DeviceClass, "setRuntimeVersions", "(Ljava/util/Map;)V");
	CacheInstanceJavaMethod(env, cache->DeviceSetTotalMemory, cache->DeviceClass, "setTotalMemory", "(Ljava/lang/Long;)V");

	CacheInstanceJavaMethod(env, cache->DeviceWithStateGetFreeDisk, cache->DeviceWithStateClass, "getFreeDisk", "()Ljava/lang/Long;");
	CacheInstanceJavaMethod(env, cache->DeviceWithStateGetFreeMemory, cache->DeviceWithStateClass, "getFreeMemory", "()Ljava/lang/Long;");
	CacheInstanceJavaMethod(env, cache->DeviceWithStateGetOrientation, cache->DeviceWithStateClass, "getOrientation", "()Ljava/lang/String;");
	CacheInstanceJavaMethod(env, cache->DeviceWithStateGetTime, cache->DeviceWithStateClass, "getTime", "()Ljava/util/Date;");
	CacheInstanceJavaMethod(env, cache->DeviceWithStateSetFreeDisk, cache->DeviceWithStateClass, "setFreeDisk", "(Ljava/lang/Long;)V");
	CacheInstanceJavaMethod(env, cache->DeviceWithStateSetFreeMemory, cache->DeviceWithStateClass, "setFreeMemory", "(Ljava/lang/Long;)V");
	CacheInstanceJavaMethod(env, cache->DeviceWithStateSetOrientation, cache->DeviceWithStateClass, "setOrientation", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->DeviceWithStateSetTime, cache->DeviceWithStateClass, "setTime", "(Ljava/util/Date;)V");

	CacheInstanceJavaMethod(env, cache->EnumGetName, cache->EnumClass, "name", "()Ljava/lang/String;");

	CacheInstanceJavaMethod(env, cache->EndpointConfigurationConstructor, cache->EndpointConfigurationClass, "<init>", "(Ljava/lang/String;Ljava/lang/String;)V");

	CacheInstanceJavaMethod(env, cache->ErrorGetErrorClass, cache->ErrorClass, "getErrorClass", "()Ljava/lang/String;");
	CacheInstanceJavaMethod(env, cache->ErrorGetErrorMessage, cache->ErrorClass, "getErrorMessage", "()Ljava/lang/String;");
	CacheInstanceJavaMethod(env, cache->ErrorGetStacktrace, cache->ErrorClass, "getStacktrace", "()Ljava/util/List;");
	CacheInstanceJavaMethod(env, cache->ErrorGetType, cache->ErrorClass, "getType", "()Lcom/bugsnag/android/ErrorType;");
	CacheInstanceJavaMethod(env, cache->ErrorSetErrorClass, cache->ErrorClass, "setErrorClass", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->ErrorSetErrorMessage, cache->ErrorClass, "setErrorMessage", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->ErrorSetType, cache->ErrorClass, "setType", "(Lcom/bugsnag/android/ErrorType;)V");

	CacheInstanceJavaMethod(env, cache->ErrorTypesConstructor, cache->ErrorTypesClass, "<init>", "(ZZZZ)V");

	CacheInstanceJavaMethod(env, cache->EventAddFeatureFlag, cache->EventClass, "addFeatureFlag", "(Ljava/lang/String;Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->EventAddMetadataToSection, cache->EventClass, "addMetadata", "(Ljava/lang/String;Ljava/util/Map;)V");
	CacheInstanceJavaMethod(env, cache->EventAddMetadataValue, cache->EventClass, "addMetadata", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/Object;)V");
	CacheInstanceJavaMethod(env, cache->EventClearFeatureFlag, cache->EventClass, "clearFeatureFlag", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->EventClearFeatureFlags, cache->EventClass, "clearFeatureFlags", "()V");
	CacheInstanceJavaMethod(env, cache->EventClearMetadataSection, cache->EventClass, "clearMetadata", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->EventClearMetadataValue, cache->EventClass, "clearMetadata", "(Ljava/lang/String;Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->EventGetApiKey, cache->EventClass, "getApiKey", "()Ljava/lang/String;");
	CacheInstanceJavaMethod(env, cache->EventGetApp, cache->EventClass, "getApp", "()Lcom/bugsnag/android/AppWithState;");
	CacheInstanceJavaMethod(env, cache->EventGetBreadcrumbs, cache->EventClass, "getBreadcrumbs", "()Ljava/util/List;");
	CacheInstanceJavaMethod(env, cache->EventGetContext, cache->EventClass, "getContext", "()Ljava/lang/String;");
	CacheInstanceJavaMethod(env, cache->EventGetDevice, cache->EventClass, "getDevice", "()Lcom/bugsnag/android/DeviceWithState;");
	CacheInstanceJavaMethod(env, cache->EventGetErrors, cache->EventClass, "getErrors", "()Ljava/util/List;");
	CacheInstanceJavaMethod(env, cache->EventGetGroupingHash, cache->EventClass, "getGroupingHash", "()Ljava/lang/String;");
	CacheInstanceJavaMethod(env, cache->EventGetSeverity, cache->EventClass, "getSeverity", "()Lcom/bugsnag/android/Severity;");
	CacheInstanceJavaMethod(env, cache->EventGetThreads, cache->EventClass, "getThreads", "()Ljava/util/List;");
	CacheInstanceJavaMethod(env, cache->EventGetUnhandled, cache->EventClass, "isUnhandled", "()Z");
	CacheInstanceJavaMethod(env, cache->EventGetUser, cache->EventClass, "getUser", "()Lcom/bugsnag/android/User;");
	CacheInstanceJavaMethod(env, cache->EventSetApiKey, cache->EventClass, "setApiKey", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->EventSetContext, cache->EventClass, "setContext", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->EventSetGroupingHash, cache->EventClass, "setGroupingHash", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->EventSetSeverity, cache->EventClass, "setSeverity", "(Lcom/bugsnag/android/Severity;)V");
	CacheInstanceJavaMethod(env, cache->EventSetUnhandled, cache->EventClass, "setUnhandled", "(Z)V");
	CacheInstanceJavaMethod(env, cache->EventSetUser, cache->EventClass, "setUser", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->EventUpdateSeverityReason, cache->EventClass, "updateSeverityReason", "(Ljava/lang/String;)V");

	CacheInstanceJavaMethod(env, cache->FileConstructor, cache->FileClass, "<init>", "(Ljava/lang/String;)V");

	CacheInstanceJavaMethod(env, cache->HashMapConstructor, cache->HashMapClass, "<init>", "()V");
	CacheInstanceJavaMethod(env, cache->HashMapGet, cache->HashMapClass, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");

	CacheInstanceJavaMethod(env, cache->LastRunInfoGetCrashed, cache->LastRunInfoClass, "getCrashed", "()Z");
	CacheInstanceJavaMethod(env, cache->LastRunInfoGetCrashedDuringLaunch, cache->LastRunInfoClass, "getCrashedDuringLaunch", "()Z");
	CacheInstanceJavaMethod(env, cache->LastRunInfoGetConsecutiveLaunchCrashes, cache->LastRunInfoClass, "getConsecutiveLaunchCrashes", "()I");

	CacheInstanceJavaMethod(env, cache->ListAdd, cache->ListClass, "add", "(Ljava/lang/Object;)Z");
	CacheInstanceJavaMethod(env, cache->ListClear, cache->ListClass, "clear", "()V");
	CacheInstanceJavaMethod(env, cache->ListGet, cache->ListClass, "get", "(I)Ljava/lang/Object;");
	CacheInstanceJavaMethod(env, cache->ListSize, cache->ListClass, "size", "()I");

	CacheInstanceJavaMethod(env, cache->LongConstructor, cache->LongClass, "<init>", "(J)V");

	CacheInstanceJavaMethod(env, cache->NotifierConstructor, cache->NotifierClass, "<init>", "()V");
	CacheInstanceJavaMethod(env, cache->NotifierSetDependencies, cache->NotifierClass, "setDependencies", "(Ljava/util/List;)V");
	CacheInstanceJavaMethod(env, cache->NotifierSetName, cache->NotifierClass, "setName", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->NotifierSetUrl, cache->NotifierClass, "setUrl", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->NotifierSetVersion, cache->NotifierClass, "setVersion", "(Ljava/lang/String;)V");

	CacheInstanceJavaMethod(env, cache->SessionGetApp, cache->SessionClass, "getApp", "()Lcom/bugsnag/android/App;");
	CacheInstanceJavaMethod(env, cache->SessionGetDevice, cache->SessionClass, "getDevice", "()Lcom/bugsnag/android/Device;");
	CacheInstanceJavaMethod(env, cache->SessionGetId, cache->SessionClass, "getId", "()Ljava/lang/String;");
	CacheInstanceJavaMethod(env, cache->SessionGetStartedAt, cache->SessionClass, "getStartedAt", "()Ljava/util/Date;");
	CacheInstanceJavaMethod(env, cache->SessionGetUser, cache->SessionClass, "getUser", "()Lcom/bugsnag/android/User;");
	CacheInstanceJavaMethod(env, cache->SessionSetId, cache->SessionClass, "setId", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->SessionSetStartedAt, cache->SessionClass, "setStartedAt", "(Ljava/util/Date;)V");
	CacheInstanceJavaMethod(env, cache->SessionSetUser, cache->SessionClass, "setUser", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");

	CacheInstanceJavaMethod(env, cache->StackframeGetFile, cache->StackframeClass, "getFile", "()Ljava/lang/String;");
	CacheInstanceJavaMethod(env, cache->StackframeGetInProject, cache->StackframeClass, "getInProject", "()Ljava/lang/Boolean;");
	CacheInstanceJavaMethod(env, cache->StackframeGetLineNumber, cache->StackframeClass, "getLineNumber", "()Ljava/lang/Number;");
	CacheInstanceJavaMethod(env, cache->StackframeGetMethod, cache->StackframeClass, "getMethod", "()Ljava/lang/String;");
	CacheInstanceJavaMethod(env, cache->StackframeGetType, cache->StackframeClass, "getType", "()Lcom/bugsnag/android/ErrorType;");
	CacheInstanceJavaMethod(env, cache->StackframeSetFile, cache->StackframeClass, "setFile", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->StackframeSetInProject, cache->StackframeClass, "setInProject", "(Ljava/lang/Boolean;)V");
	CacheInstanceJavaMethod(env, cache->StackframeSetLineNumber, cache->StackframeClass, "setLineNumber", "(Ljava/lang/Number;)V");
	CacheInstanceJavaMethod(env, cache->StackframeSetMethod, cache->StackframeClass, "setMethod", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->StackframeSetType, cache->StackframeClass, "setType", "(Lcom/bugsnag/android/ErrorType;)V");

	CacheInstanceJavaMethod(env, cache->ThreadGetErrorReportingThread, cache->ThreadClass, "getErrorReportingThread", "()Z");
	CacheInstanceJavaMethod(env, cache->ThreadGetId, cache->ThreadClass, "getId", "()J");
	CacheInstanceJavaMethod(env, cache->ThreadGetName, cache->ThreadClass, "getName", "()Ljava/lang/String;");
	CacheInstanceJavaMethod(env, cache->ThreadGetType, cache->ThreadClass, "getType", "()Lcom/bugsnag/android/ThreadType;");
	CacheInstanceJavaMethod(env, cache->ThreadGetStacktrace, cache->ThreadClass, "getStacktrace", "()Ljava/util/List;");
	CacheInstanceJavaMethod(env, cache->ThreadSetId, cache->ThreadClass, "setId", "(J)V");
	CacheInstanceJavaMethod(env, cache->ThreadSetName, cache->ThreadClass, "setName", "(Ljava/lang/String;)V");
	CacheInstanceJavaMethod(env, cache->ThreadSetStacktrace, cache->ThreadClass, "setStacktrace", "(Ljava/util/List;)V");

	CacheInstanceJavaMethod(env, cache->UserGetEmail, cache->UserClass, "getEmail", "()Ljava/lang/String;");
	CacheInstanceJavaMethod(env, cache->UserGetId, cache->UserClass, "getId", "()Ljava/lang/String;");
	CacheInstanceJavaMethod(env, cache->UserGetName, cache->UserClass, "getName", "()Ljava/lang/String;");

	CacheInstanceJavaMethod(env, cache->ArrayListConstructor, cache->ArrayListClass, "<init>", "()V");
	CacheInstanceJavaMethod(env, cache->ArrayListCollectionConstructor, cache->ArrayListClass, "<init>", "(Ljava/util/Collection;)V");

	CacheInstanceJavaMethod(env, cache->BooleanConstructor, cache->BooleanClass, "<init>", "(Z)V");
	CacheInstanceJavaMethod(env, cache->BooleanBooleanValue, cache->BooleanClass, "booleanValue", "()Z");

	CacheInstanceJavaMethod(env, cache->IntegerConstructor, cache->IntegerClass, "<init>", "(I)V");

	CacheInstanceJavaMethod(env, cache->HashSetConstructor, cache->HashSetClass, "<init>", "()V");
	CacheInstanceJavaMethod(env, cache->HashSetAdd, cache->HashSetClass, "add", "(Ljava/lang/Object;)Z");

	CacheInstanceJavaMethod(env, cache->MapKeySet, cache->MapClass, "keySet", "()Ljava/util/Set;");
	CacheInstanceJavaMethod(env, cache->MapSize, cache->MapClass, "size", "()I");

	CacheInstanceJavaMethod(env, cache->NumberIntValue, cache->NumberClass, "intValue", "()I");
	CacheInstanceJavaMethod(env, cache->NumberLongValue, cache->NumberClass, "longValue", "()J");

	CacheInstanceJavaMethod(env, cache->TraceConstructor, cache->TraceClass, "<init>", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V");

	CacheStaticJavaField(env, cache->SeverityFieldInfo, cache->SeverityClass, "INFO", "Lcom/bugsnag/android/Severity;");
	CacheStaticJavaField(env, cache->SeverityFieldWarning, cache->SeverityClass, "WARNING", "Lcom/bugsnag/android/Severity;");
	CacheStaticJavaField(env, cache->SeverityFieldError, cache->SeverityClass, "ERROR", "Lcom/bugsnag/android/Severity;");
	CacheStaticJavaField(env, cache->BreadcrumbTypeError, cache->BreadcrumbTypeClass, "ERROR", "Lcom/bugsnag/android/BreadcrumbType;");
	CacheStaticJavaField(env, cache->BreadcrumbTypeLog, cache->BreadcrumbTypeClass, "LOG", "Lcom/bugsnag/android/BreadcrumbType;");
	CacheStaticJavaField(env, cache->BreadcrumbTypeManual, cache->BreadcrumbTypeClass, "MANUAL", "Lcom/bugsnag/android/BreadcrumbType;");
	CacheStaticJavaField(env, cache->BreadcrumbTypeNavigation, cache->BreadcrumbTypeClass, "NAVIGATION", "Lcom/bugsnag/android/BreadcrumbType;");
	CacheStaticJavaField(env, cache->BreadcrumbTypeProcess, cache->BreadcrumbTypeClass, "PROCESS", "Lcom/bugsnag/android/BreadcrumbType;");
	CacheStaticJavaField(env, cache->BreadcrumbTypeRequest, cache->BreadcrumbTypeClass, "REQUEST", "Lcom/bugsnag/android/BreadcrumbType;");
	CacheStaticJavaField(env, cache->BreadcrumbTypeState, cache->BreadcrumbTypeClass, "STATE", "Lcom/bugsnag/android/BreadcrumbType;");
	CacheStaticJavaField(env, cache->BreadcrumbTypeUser, cache->BreadcrumbTypeClass, "USER", "Lcom/bugsnag/android/BreadcrumbType;");
	CacheStaticJavaField(env, cache->ErrorTypeAndroid, cache->ErrorTypeClass, "ANDROID", "Lcom/bugsnag/android/ErrorType;");
	CacheStaticJavaField(env, cache->ErrorTypeC, cache->ErrorTypeClass, "C", "Lcom/bugsnag/android/ErrorType;");
	CacheStaticJavaField(env, cache->ThreadSendPolicyAlways, cache->ThreadSendPolicyClass, "ALWAYS", "Lcom/bugsnag/android/ThreadSendPolicy;");
	CacheStaticJavaField(env, cache->ThreadSendPolicyUnhandledOnly, cache->ThreadSendPolicyClass, "UNHANDLED_ONLY", "Lcom/bugsnag/android/ThreadSendPolicy;");
	CacheStaticJavaField(env, cache->ThreadSendPolicyNever, cache->ThreadSendPolicyClass, "NEVER", "Lcom/bugsnag/android/ThreadSendPolicy;");
	CacheStaticJavaField(env, cache->ThreadTypeAndroid, cache->ThreadTypeClass, "ANDROID", "Lcom/bugsnag/android/ThreadType;");
	CacheStaticJavaField(env, cache->ThreadTypeC, cache->ThreadTypeClass, "C", "Lcom/bugsnag/android/ThreadType;");

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

FString FAndroidPlatformJNI::ParseJavaString(JNIEnv* Env, const JNIReferenceCache* Cache, jobject Value)
{
	if (!Value || !(Env)->IsInstanceOf(Value, Cache->StringClass))
	{
		return TEXT("");
	}
	const char* Text = (*Env).GetStringUTFChars((jstring)Value, nullptr);
	if (FAndroidPlatformJNI::CheckAndClearException(Env))
	{
		return TEXT("");
	}
	return UTF8_TO_TCHAR(Text);
}

FBugsnagUser FAndroidPlatformJNI::ParseJavaUser(JNIEnv* Env, const JNIReferenceCache* Cache, jobject Value)
{
	if (FAndroidPlatformJNI::CheckAndClearException(Env) || !Env->IsInstanceOf(Value, Cache->UserClass))
	{
		return FBugsnagUser();
	}
	jobject jId = (*Env).CallObjectMethod(Value, Cache->UserGetId);
	FAndroidPlatformJNI::CheckAndClearException(Env);
	FString Id = FAndroidPlatformJNI::ParseJavaString(Env, Cache, jId);
	jobject jName = (*Env).CallObjectMethod(Value, Cache->UserGetName);
	FAndroidPlatformJNI::CheckAndClearException(Env);
	FString Name = FAndroidPlatformJNI::ParseJavaString(Env, Cache, jName);
	jobject jEmail = (*Env).CallObjectMethod(Value, Cache->UserGetEmail);
	FAndroidPlatformJNI::CheckAndClearException(Env);
	FString Email = FAndroidPlatformJNI::ParseJavaString(Env, Cache, jEmail);
	return FBugsnagUser(Id, Email, Name);
}

jstring FAndroidPlatformJNI::ParseFStringOptional(JNIEnv* Env, const TOptional<FString>& Text)
{
	if (Text.IsSet())
	{
		return ParseFString(Env, Text.GetValue());
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

jobject FAndroidPlatformJNI::ParseBreadcrumbTypeSet(JNIEnv* Env, const JNIReferenceCache* Cache, const EBugsnagEnabledBreadcrumbTypes Value)
{
	jobject jSet = (*Env).NewObject(Cache->HashSetClass, Cache->HashSetConstructor);
	if (FAndroidPlatformJNI::CheckAndClearException(Env))
	{
		return nullptr;
	}
	if (addTypeToSet(Env, jSet, EnumHasAllFlags(Value, EBugsnagEnabledBreadcrumbTypes::Error), Cache, Cache->BreadcrumbTypeError) &&
		addTypeToSet(Env, jSet, EnumHasAllFlags(Value, EBugsnagEnabledBreadcrumbTypes::Log), Cache, Cache->BreadcrumbTypeLog) &&
		addTypeToSet(Env, jSet, EnumHasAllFlags(Value, EBugsnagEnabledBreadcrumbTypes::Navigation), Cache, Cache->BreadcrumbTypeNavigation) &&
		addTypeToSet(Env, jSet, EnumHasAllFlags(Value, EBugsnagEnabledBreadcrumbTypes::Process), Cache, Cache->BreadcrumbTypeProcess) &&
		addTypeToSet(Env, jSet, EnumHasAllFlags(Value, EBugsnagEnabledBreadcrumbTypes::Request), Cache, Cache->BreadcrumbTypeRequest) &&
		addTypeToSet(Env, jSet, EnumHasAllFlags(Value, EBugsnagEnabledBreadcrumbTypes::State), Cache, Cache->BreadcrumbTypeState) &&
		addTypeToSet(Env, jSet, EnumHasAllFlags(Value, EBugsnagEnabledBreadcrumbTypes::User), Cache, Cache->BreadcrumbTypeUser))
	{
		return jSet;
	}
	return nullptr;
}

jobject FAndroidPlatformJNI::ParseInteger(JNIEnv* Env, const JNIReferenceCache* Cache, int32 Value)
{
	jobject jValue = (*Env).NewObject(Cache->IntegerClass, Cache->IntegerConstructor, Value);
	if (FAndroidPlatformJNI::CheckAndClearException(Env))
	{
		return nullptr;
	}
	return jValue;
}

EBugsnagSeverity FAndroidPlatformJNI::ParseSeverity(JNIEnv* Env, const JNIReferenceCache* Cache, jobject Value)
{
	const char* Name = FAndroidPlatformJNI::GetNameFromEnum(Env, Cache, Value);
	if (!Name)
	{
		return EBugsnagSeverity::Warning;
	}
	switch (Name[0])
	{
	case 'E':
		return EBugsnagSeverity::Error;
	case 'I':
		return EBugsnagSeverity::Info;
	case 'W':
	default:
		return EBugsnagSeverity::Warning;
	}
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
	(*Env).CallBooleanMethod(jDependencies, Cache->ListAdd, jDependency);
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

TSharedPtr<FJsonObject> FAndroidPlatformJNI::ConvertJavaBytesToJson(JNIEnv* Env, const JNIReferenceCache* Cache, jbyteArray jData)
{
	TSharedPtr<FJsonObject> Json = MakeShareable(new FJsonObject);
	int Len = (*Env).GetArrayLength(jData);
	ReturnValueOnException(Env, Json);
	const jbyte* Bytes = (*Env).GetByteArrayElements(jData, nullptr);
	ReturnValueOnException(Env, Json);
	FLargeMemoryReader Archive((const uint8*)Bytes, Len);
	if (FJsonSerializer::Deserialize(TJsonReaderFactory<char>::Create(&Archive), Json))
	{
		return Json;
	}
	else
	{
		return MakeShareable(new FJsonObject);
	}
}

TSharedPtr<FJsonObject> FAndroidPlatformJNI::ConvertJavaMapToJson(JNIEnv* Env, const JNIReferenceCache* Cache, jobject jMap)
{
	TSharedPtr<FJsonObject> Json = MakeShareable(new FJsonObject);
	jbyteArray jData = (jbyteArray)(*Env).CallStaticObjectMethod(Cache->MetadataSerializerClass, Cache->MetadataSerializerSerialize, jMap);
	if (FAndroidPlatformJNI::CheckAndClearException(Env))
	{
		return Json;
	}
	return FAndroidPlatformJNI::ConvertJavaBytesToJson(Env, Cache, jData);
}

FDateTime FAndroidPlatformJNI::ParseDateTime(JNIEnv* Env, const JNIReferenceCache* Cache, jobject Value)
{
	if (!Value)
	{
		return FDateTime(0);
	}
	jlong milliseconds = (*Env).CallLongMethod(Value, Cache->DateGetTime);
	if (FAndroidPlatformJNI::CheckAndClearException(Env))
	{
		return FDateTime(0);
	}
	return FDateTime(FDateTime(1970, 1, 1).GetTicks() + milliseconds * ETimespan::TicksPerMillisecond);
}

jobject FAndroidPlatformJNI::ParseJavaDate(JNIEnv* Env, const JNIReferenceCache* Cache, FDateTime Value)
{
	jlong milliseconds = (Value.GetTicks() - FDateTime(1970, 1, 1).GetTicks()) / ETimespan::TicksPerMillisecond;
	jobject jValue = (*Env).NewObject(Cache->DateClass, Cache->DateConstructor, milliseconds);
	return FAndroidPlatformJNI::CheckAndClearException(Env) ? nullptr : jValue;
}

const char* FAndroidPlatformJNI::GetNameFromEnum(JNIEnv* Env, const JNIReferenceCache* Cache, jobject EnumValue)
{
	jobject jName = (*Env).CallObjectMethod(EnumValue, Cache->EnumGetName);
	ReturnNullOnException(Env);
	const char* Name = (*Env).GetStringUTFChars((jstring)jName, nullptr);
	ReturnNullOnException(Env);
	(*Env).DeleteLocalRef(jName);
	return Name;
}

EBugsnagErrorType FAndroidPlatformJNI::ParseErrorType(JNIEnv* Env, const JNIReferenceCache* Cache, jobject Value)
{
	const char* Name = FAndroidPlatformJNI::GetNameFromEnum(Env, Cache, Value);
	if (!Name)
	{
		return EBugsnagErrorType::C;
	}
	switch (Name[0])
	{
	case 'A':
		return EBugsnagErrorType::Android;
	case 'C':
	default:
		return EBugsnagErrorType::C;
	}
}
