// Copyright 2021 Bugsnag. All Rights Reserved.

#pragma once

#include <jni.h>

#include "BugsnagBreadcrumb.h"
#include "BugsnagEvent.h"
#include "BugsnagSettings.h"

typedef struct
{
	bool loaded;
	bool initialized;
	jclass AppClass;
	jclass AppWithStateClass;
	jclass BreadcrumbClass;
	jclass BreadcrumbTypeClass;
	jclass BugsnagClass;
	jclass BugsnagUnrealPluginClass;
	jclass ClientClass;
	jclass ConfigClass;
	jclass DeviceClass;
	jclass DeviceWithStateClass;
	jclass EndpointConfigurationClass;
	jclass ErrorClass;
	jclass ErrorTypesClass;
	jclass ErrorTypeClass;
	jclass EventClass;
	jclass LastRunInfoClass;
	jclass MetadataParserClass;
	jclass MetadataSerializerClass;
	jclass NotifierClass;
	jclass ThreadClass;
	jclass ThreadSendPolicyClass;
	jclass ThreadTypeClass;
	jclass SessionClass;
	jclass SeverityClass;
	jclass StackframeClass;
	jclass UserClass;
	jclass TraceClass;
	jclass ContextClass;
	jclass DateClass;
	jclass EnumClass;
	jclass HashMapClass;
	jclass HashSetClass;
	jclass ArrayListClass;
	jclass BooleanClass;
	jclass FileClass;
	jclass IntegerClass;
	jclass ListClass;
	jclass LongClass;
	jclass MapClass;
	jclass NumberClass;
	jclass StringClass;

	jmethodID ArrayListConstructor;
	jmethodID ArrayListCollectionConstructor;
	jmethodID AppGetBinaryArch;
	jmethodID AppGetBuildUuid;
	jmethodID AppGetId;
	jmethodID AppGetReleaseStage;
	jmethodID AppGetType;
	jmethodID AppGetVersion;
	jmethodID AppGetVersionCode;
	jmethodID AppSetBinaryArch;
	jmethodID AppSetBuildUuid;
	jmethodID AppSetId;
	jmethodID AppSetReleaseStage;
	jmethodID AppSetType;
	jmethodID AppSetVersion;
	jmethodID AppSetVersionCode;
	jmethodID AppWithStateGetDuration;
	jmethodID AppWithStateGetDurationInForeground;
	jmethodID AppWithStateGetInForeground;
	jmethodID AppWithStateGetIsLaunching;
	jmethodID AppWithStateSetDuration;
	jmethodID AppWithStateSetDurationInForeground;
	jmethodID AppWithStateSetInForeground;
	jmethodID AppWithStateSetIsLaunching;
	jmethodID BreadcrumbGetMessage;
	jmethodID BreadcrumbGetMetadata;
	jmethodID BreadcrumbGetTimestamp;
	jmethodID BreadcrumbGetType;
	jmethodID BreadcrumbSetMessage;
	jmethodID BreadcrumbSetMetadata;
	jmethodID BreadcrumbSetType;
	jmethodID BugsnagAddMetadataToSection;
	jmethodID BugsnagAddMetadataValue;
	jmethodID BugsnagClearMetadataSection;
	jmethodID BugsnagClearMetadataValue;
	jmethodID BugsnagGetBreadcrumbs;
	jmethodID BugsnagGetLastRunInfo;
	jmethodID BugsnagGetUser;
	jmethodID BugsnagStartMethod;
	jmethodID BugsnagNotifyMethod;
	jmethodID BugsnagSetContext;
	jmethodID BugsnagLeaveBreadcrumb;
	jmethodID BugsnagMarkLaunchCompleted;
	jmethodID BugsnagStartSession;
	jmethodID BugsnagPauseSession;
	jmethodID BugsnagResumeSession;
	jmethodID BugsnagSetUser;
	jmethodID BugsnagUnrealPluginConstructor;
	jmethodID BugsnagUnrealPluginGetEventMetadataSection;
	jmethodID BugsnagUnrealPluginGetEventMetadataValue;
	jmethodID BugsnagUnrealPluginGetMetadataSection;
	jmethodID BugsnagUnrealPluginGetMetadataValue;
	jmethodID BugsnagUnrealPluginNotify;
	jmethodID ClientAddRuntimeVersionInfo;
	jmethodID ConfigAddMetadata;
	jmethodID ConfigAddPlugin;
	jmethodID ConfigConstructor;
	jmethodID ConfigGetNotifier;
	jmethodID ConfigSetAppType;
	jmethodID ConfigSetAppVersion;
	jmethodID ConfigSetAutoDetectErrors;
	jmethodID ConfigSetAutoTrackSessions;
	jmethodID ConfigSetContext;
	jmethodID ConfigSetDiscardClasses;
	jmethodID ConfigSetEnabledBreadcrumbTypes;
	jmethodID ConfigSetEnabledErrorTypes;
	jmethodID ConfigSetEnabledReleaseStages;
	jmethodID ConfigSetEndpoints;
	jmethodID ConfigSetLaunchDurationMillis;
	jmethodID ConfigSetMaxBreadcrumbs;
	jmethodID ConfigSetMaxPersistedEvents;
	jmethodID ConfigSetMaxPersistedSessions;
	jmethodID ConfigSetPersistenceDirectory;
	jmethodID ConfigSetPersistUser;
	jmethodID ConfigSetProjectPackages;
	jmethodID ConfigSetRedactedKeys;
	jmethodID ConfigSetReleaseStage;
	jmethodID ConfigSetSendLaunchCrashesSynchronously;
	jmethodID ConfigSetSendThreads;
	jmethodID ConfigSetUser;
	jmethodID ConfigSetVersionCode;
	jmethodID ContextGetApplication;
	jmethodID DeviceGetCpuAbi;
	jmethodID DeviceGetId;
	jmethodID DeviceGetJailbroken;
	jmethodID DeviceGetLocale;
	jmethodID DeviceGetManufacturer;
	jmethodID DeviceGetModel;
	jmethodID DeviceGetOsName;
	jmethodID DeviceGetOsVersion;
	jmethodID DeviceGetRuntimeVersions;
	jmethodID DeviceGetTotalMemory;
	jmethodID DeviceSetCpuAbi;
	jmethodID DeviceSetId;
	jmethodID DeviceSetJailbroken;
	jmethodID DeviceSetLocale;
	jmethodID DeviceSetManufacturer;
	jmethodID DeviceSetModel;
	jmethodID DeviceSetOsName;
	jmethodID DeviceSetOsVersion;
	jmethodID DeviceSetRuntimeVersions;
	jmethodID DeviceSetTotalMemory;
	jmethodID DeviceWithStateGetFreeDisk;
	jmethodID DeviceWithStateGetFreeMemory;
	jmethodID DeviceWithStateGetOrientation;
	jmethodID DeviceWithStateGetTime;
	jmethodID DeviceWithStateSetFreeDisk;
	jmethodID DeviceWithStateSetFreeMemory;
	jmethodID DeviceWithStateSetOrientation;
	jmethodID DeviceWithStateSetTime;
	jmethodID EndpointConfigurationConstructor;
	jmethodID ErrorTypesConstructor;
	jmethodID DateConstructor;
	jmethodID DateGetTime;
	jmethodID EnumGetName;
	jmethodID ErrorGetErrorClass;
	jmethodID ErrorGetErrorMessage;
	jmethodID ErrorGetStacktrace;
	jmethodID ErrorGetType;
	jmethodID ErrorSetErrorClass;
	jmethodID ErrorSetErrorMessage;
	jmethodID ErrorSetType;
	jmethodID EventAddMetadataToSection;
	jmethodID EventAddMetadataValue;
	jmethodID EventClearMetadataSection;
	jmethodID EventClearMetadataValue;
	jmethodID EventGetApiKey;
	jmethodID EventGetApp;
	jmethodID EventGetBreadcrumbs;
	jmethodID EventGetContext;
	jmethodID EventGetDevice;
	jmethodID EventGetErrors;
	jmethodID EventGetGroupingHash;
	jmethodID EventGetSeverity;
	jmethodID EventGetThreads;
	jmethodID EventGetUnhandled;
	jmethodID EventGetUser;
	jmethodID EventSetApiKey;
	jmethodID EventSetContext;
	jmethodID EventSetGroupingHash;
	jmethodID EventSetSeverity;
	jmethodID EventSetUnhandled;
	jmethodID EventSetUser;
	jmethodID EventUpdateSeverityReason;
	jmethodID HashMapConstructor;
	jmethodID HashMapGet;
	jmethodID LastRunInfoGetCrashed;
	jmethodID LastRunInfoGetCrashedDuringLaunch;
	jmethodID LastRunInfoGetConsecutiveLaunchCrashes;
	jmethodID NotifierConstructor;
	jmethodID NotifierGetName;
	jmethodID NotifierGetUrl;
	jmethodID NotifierGetVersion;
	jmethodID NotifierSetName;
	jmethodID NotifierSetUrl;
	jmethodID NotifierSetVersion;
	jmethodID NotifierSetDependencies;
	jmethodID SessionGetApp;
	jmethodID SessionGetDevice;
	jmethodID SessionGetId;
	jmethodID SessionGetStartedAt;
	jmethodID SessionGetUser;
	jmethodID SessionSetId;
	jmethodID SessionSetStartedAt;
	jmethodID SessionSetUser;
	jmethodID StackframeGetFile;
	jmethodID StackframeGetInProject;
	jmethodID StackframeGetLineNumber;
	jmethodID StackframeGetMethod;
	jmethodID StackframeGetType;
	jmethodID StackframeSetFile;
	jmethodID StackframeSetInProject;
	jmethodID StackframeSetLineNumber;
	jmethodID StackframeSetMethod;
	jmethodID StackframeSetType;
	jmethodID ThreadGetErrorReportingThread;
	jmethodID ThreadGetId;
	jmethodID ThreadGetName;
	jmethodID ThreadGetType;
	jmethodID ThreadGetStacktrace;
	jmethodID ThreadSetId;
	jmethodID ThreadSetName;
	jmethodID ThreadSetStacktrace;
	jmethodID UserGetEmail;
	jmethodID UserGetId;
	jmethodID UserGetName;
	jmethodID BooleanBooleanValue;
	jmethodID BooleanConstructor;
	jmethodID FileConstructor;
	jmethodID HashSetConstructor;
	jmethodID HashSetAdd;
	jmethodID ListAdd;
	jmethodID ListClear;
	jmethodID ListGet;
	jmethodID ListSize;
	jmethodID LongConstructor;
	jmethodID MetadataParserParse;
	jmethodID MetadataSerializerSerialize;
	jmethodID IntegerConstructor;
	jmethodID MapKeySet;
	jmethodID MapSize;
	jmethodID NumberIntValue;
	jmethodID NumberLongValue;
	jmethodID TraceConstructor;

	jfieldID SeverityFieldInfo;
	jfieldID SeverityFieldWarning;
	jfieldID SeverityFieldError;
	jfieldID BreadcrumbTypeError;
	jfieldID BreadcrumbTypeLog;
	jfieldID BreadcrumbTypeManual;
	jfieldID BreadcrumbTypeNavigation;
	jfieldID BreadcrumbTypeProcess;
	jfieldID BreadcrumbTypeRequest;
	jfieldID BreadcrumbTypeState;
	jfieldID BreadcrumbTypeUser;
	jfieldID ErrorTypeAndroid;
	jfieldID ErrorTypeC;
	jfieldID ThreadSendPolicyAlways;
	jfieldID ThreadSendPolicyUnhandledOnly;
	jfieldID ThreadSendPolicyNever;
	jfieldID ThreadTypeAndroid;
	jfieldID ThreadTypeC;
} JNIReferenceCache;

class FAndroidPlatformJNI
{
public:
	/**
   * Load global references to all JNI classes, methods, and fields which are
   * needed to run Bugsnag on Android
   *
   * @param Env   A JNI environment for the current thread
   * @param Cache A reference to a cache object to populate. Must not be null.
   *
   * @return true if all references are successfully loaded and cached as globals
   */
	static bool LoadReferenceCache(JNIEnv* Env, JNIReferenceCache* Cache);

	/**
   * Convert an FString into a Java String reference
   *
   * @param Env  A JNI environment for the current thread
   * @param Text The text to convert
   *
   * @return a Java String reference or NULL upon failure
   */
	static jstring ParseFString(JNIEnv* Env, const FString& Text);

	static jstring ParseFStringOptional(JNIEnv* Env, const TOptional<FString>& Text);

	/**
   * Convert a bool value into a Java boolean
   *
   * @param value The boolean value
   *
   * @return JNI true or false value
   */
	static jboolean ParseBoolean(bool value);

	/**
   * Convert a JSON structure into a Java Map object
   *
   * @param Env    A JNI environment for the current thread
   * @param Cache  A populated reference cache
   * @param Object The object to convert
   *
   * @return A Java object reference or null on failure
   */
	static jobject ParseJsonObject(JNIEnv* Env, const JNIReferenceCache* Cache, const TSharedPtr<FJsonObject>& Object);

	/**
   * Convert a BreadcrumbType enum value into a Java object
   *
   * @param Env   A JNI environment for the current thread
   * @param Cache A populated reference cache
   * @param Type  The type to convert
   *
   * @return A Java object reference or null on failure
   */
	static jobject ParseBreadcrumbType(JNIEnv* Env, const JNIReferenceCache* Cache, EBugsnagBreadcrumbType Type);
	/**
   * Convert an array of strings into a Java Set
   *
   * @param Env    A JNI environment for the current thread
   * @param Cache  A reference to a cache object to populate. Must not be null.
   * @param Values The array to convert
   *
   * @return A Java object reference or null on failure
   */
	static jobject ParseStringSet(JNIEnv* Env, const JNIReferenceCache* Cache, const TArray<FString>& Values);

	/**
   * Convert enabled breadcrumb types into Set<BreadcrumbType>
   *
   * @param Env   A JNI environment for the current thread
   * @param Cache A reference to a cache object to populate. Must not be null.
   * @param Value The enabled types to convert
   *
   * @return A Java object reference or null on failure
   */
	static jobject ParseBreadcrumbTypeSet(JNIEnv* Env, const JNIReferenceCache* Cache, const EBugsnagEnabledBreadcrumbTypes Value);

	/**
   * Convert a value into a Java Severity
   *
   * @param Env   A JNI environment for the current thread
   * @param Cache A populated reference cache
   * @param Value The severity value to convert
   *
   * @return A Java object reference or null on failure
   */
	static jobject ParseSeverity(JNIEnv* Env, const JNIReferenceCache* Cache, const EBugsnagSeverity Value);

	static EBugsnagSeverity ParseSeverity(JNIEnv* Env, const JNIReferenceCache* Cache, jobject Value);

	/**
   * Convert a value into a Java ThreadSendPolicy
   *
   * @param Env    A JNI environment for the current thread
   * @param Cache  A reference to a cache object to populate. Must not be null.
   * @param Policy The policy to convert
   *
   * @return A Java object reference or null on failure
   */
	static jobject ParseThreadSendPolicy(JNIEnv* Env, const JNIReferenceCache* Cache, const EBugsnagSendThreadsPolicy Policy);

	/**
   * Convert an int into a Java Integer
   *
   * @param Env   A JNI environment for the current thread
   * @param Cache A reference to a cache object to populate. Must not be null.
   * @param Value The value to convert
   */
	static jobject ParseInteger(JNIEnv* Env, const JNIReferenceCache* Cache, int32 Value);

	/**
     * Convert a Java Date into a FDateTime
     *
     * @param Env   A JNI environment for the current thread
     * @param Cache A populated reference cache
     * @param Value A jobject representing a Java Date
     *
     * @return A date time object, representing the Unix epoch in the event of failure
     */
	static FDateTime ParseDateTime(JNIEnv* Env, const JNIReferenceCache* Cache, jobject Value);

	/**
     * Convert a FDateTime into a Java Date
     *
     * @param Env   A JNI environment for the current thread
     * @param Cache A populated reference cache
     * @param Value A value to convert
     *
     * @return A jobject representing a date or null upon failure
     */
	static jobject ParseJavaDate(JNIEnv* Env, const JNIReferenceCache* Cache, FDateTime Value);

	/**
     * Convert a Java String into a FString
     *
     * @param Env   A JNI environment for the current thread
     * @param Cache A populated reference cache
     * @param Value A jobject representing a Java String
     *
     * @return A string object, empty in the event of failure
     */
	static FString ParseJavaString(JNIEnv* Env, const JNIReferenceCache* Cache, jobject Value);

	/**
     * Convert a Bugsnag Java User into a FBugsnagUser
     *
     * @param Env   A JNI environment for the current thread
     * @param Cache A populated reference cache
     * @param Value A jobject representing a Java User
     *
     * @return A user object, with empty fields in the event of an error
     */
	static FBugsnagUser ParseJavaUser(JNIEnv* Env, const JNIReferenceCache* Cache, jobject Value);

	/**
   * Convert an array of bytes into a JSON object
   *
   * @param Env     A JNI environment for the current thread
   * @param Cache   A populated reference cache
   * @param jBytes  A byte array representing json data
   *
   * @return A JSON object, empty if the map could not be parsed as JSON
   */
	static TSharedPtr<FJsonObject> ConvertJavaBytesToJson(JNIEnv* Env, const JNIReferenceCache* Cache, jbyteArray jBytes);

	/**
   * Convert a Java map into a JSON object
   *
   * @param Env   A JNI environment for the current thread
   * @param Cache A populated reference cache
   * @param jMap  A jobject representing a Java Map<String, Object>
   *
   * @return A JSON object, empty if the map could not be parsed as JSON
   */
	static TSharedPtr<FJsonObject> ConvertJavaMapToJson(JNIEnv* Env, const JNIReferenceCache* Cache, jobject jMap);

	/**
   * Check if a Java runtime exception was thrown and if so, clear it.
   *
   * @param Env A JNI environment for the current thread
   *
   * @return true if an exception was detected
   */
	static bool CheckAndClearException(JNIEnv* Env);

	/**
   * Set bugsnag unreal notifier info on the client, converting the existing notifier
   * info into a dependency
   *
   * @param Env     A JNI environment for the current thread
   * @param Cache   A reference to a cache object to populate. Must not be null.
   * @param jClient The client containing the notifier
   */
	static bool SetNotifierInfo(JNIEnv* Env, const JNIReferenceCache* Cache, jobject jClient);

	/**
     * Get the name of a Java enum value as a string
     *
     * @param Env        A JNI environment for the current thread
     * @param Cache      A populated reference cache
     * @param EnumValue  The named value
     *
     * @return the name or null upon exception
     */
	static const char* GetNameFromEnum(JNIEnv* Env, const JNIReferenceCache* Cache, jobject EnumValue);

	static EBugsnagErrorType ParseErrorType(JNIEnv* Env, const JNIReferenceCache* Cache, jobject Value);
};
