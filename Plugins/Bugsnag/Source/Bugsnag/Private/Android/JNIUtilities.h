#pragma once

#include <jni.h>

#include "BugsnagBreadcrumb.h"
#include "BugsnagEvent.h"
#include "BugsnagSettings.h"

typedef struct
{
	bool loaded;
	bool initialized;
	jclass InterfaceClass;
	jclass BreadcrumbClass;
	jclass BreadcrumbTypeClass;
	jclass BugsnagClass;
	jclass BugsnagUnrealPluginClass;
	jclass ConfigClass;
	jclass EndpointConfigurationClass;
	jclass ErrorTypesClass;
	jclass MetadataParserClass;
	jclass MetadataSerializerClass;
	jclass NotifierClass;
	jclass ThreadSendPolicyClass;
	jclass SeverityClass;
	jclass TraceClass;
	jclass DateClass;
	jclass EnumClass;
	jclass HashMapClass;
	jclass HashSetClass;
	jclass ArrayListClass;
	jclass IntegerClass;

	jmethodID ArrayListConstructor;
	jmethodID ArrayListAdd;
	jmethodID BreadcrumbGetMessage;
	jmethodID BreadcrumbGetMetadata;
	jmethodID BreadcrumbGetTimestamp;
	jmethodID BreadcrumbGetType;
	jmethodID BreadcrumbSetMessage;
	jmethodID BreadcrumbSetMetadata;
	jmethodID BreadcrumbSetType;
	jmethodID BugsnagStartMethod;
	jmethodID BugsnagNotifyMethod;
	jmethodID BugsnagSetContext;
	jmethodID BugsnagLeaveBreadcrumb;
	jmethodID BugsnagStartSession;
	jmethodID BugsnagPauseSession;
	jmethodID BugsnagResumeSession;
	jmethodID BugsnagUnrealPluginConstructor;
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
	jmethodID ConfigSetPersistUser;
	jmethodID ConfigSetRedactedKeys;
	jmethodID ConfigSetReleaseStage;
	jmethodID ConfigSetSendLaunchCrashesSynchronously;
	jmethodID ConfigSetSendThreads;
	jmethodID ConfigSetUser;
	jmethodID ConfigSetVersionCode;
	jmethodID EndpointConfigurationConstructor;
	jmethodID ErrorTypesConstructor;
	jmethodID DateGetTime;
	jmethodID EnumGetName;
	jmethodID HashMapConstructor;
	jmethodID NotifierConstructor;
	jmethodID NotifierGetName;
	jmethodID NotifierGetUrl;
	jmethodID NotifierGetVersion;
	jmethodID NotifierSetName;
	jmethodID NotifierSetUrl;
	jmethodID NotifierSetVersion;
	jmethodID NotifierSetDependencies;
	jmethodID HashSetConstructor;
	jmethodID HashSetAdd;
	jmethodID MetadataParserParse;
	jmethodID MetadataSerializerSerialize;
	jmethodID IntegerConstructor;
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
	jfieldID ThreadSendPolicyAlways;
	jfieldID ThreadSendPolicyUnhandledOnly;
	jfieldID ThreadSendPolicyNever;
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

	static jstring ParseFStringPtr(JNIEnv* Env, const TSharedPtr<FString>& Text);

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
	static jobject ParseBreadcrumbTypeSet(JNIEnv* Env, const JNIReferenceCache* Cache, const FBugsnagEnabledBreadcrumbTypes Value);

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
	static jobject ParseInteger(JNIEnv* Env, const JNIReferenceCache* Cache, int Value);

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
     * Call a void function with a string value
     *
     * @param Env        A JNI environment for the current thread
     * @param Target     The object to call
     * @param Method     The method to call
     * @param IsNullable true if the method should be called with null if Value is invalid
     * @param Value      the assigned value
     */
	static void SetStringValue(JNIEnv* Env, jobject Target, jmethodID Method, bool IsNullable, const TSharedPtr<FString>& Value);
};
