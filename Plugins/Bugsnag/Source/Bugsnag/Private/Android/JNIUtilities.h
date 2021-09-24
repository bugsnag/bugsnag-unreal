#pragma once

#include <jni.h>

#include "BugsnagSettings.h"

typedef struct
{
	bool loaded;
	bool initialized;
	jclass InterfaceClass;
	jclass BreadcrumbTypeClass;
	jclass BugsnagClass;
	jclass ConfigClass;
	jclass EndpointConfigurationClass;
	jclass ErrorTypesClass;
	jclass MetadataParserClass;
	jclass ThreadSendPolicyClass;
	jclass SeverityClass;
	jclass TraceClass;
	jclass HashSetClass;

	jmethodID BugsnagStartMethod;
	jmethodID BugsnagNotifyMethod;
	jmethodID ConfigAddMetadata;
	jmethodID ConfigConstructor;
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
	jmethodID EndpointConfigurationConstructor;
	jmethodID ErrorTypesConstructor;
	jmethodID HashSetConstructor;
	jmethodID HashSetAdd;
	jmethodID MetadataParserParse;
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
   * Check if a Java runtime exception was thrown and if so, clear it.
   *
   * @param Env A JNI environment for the current thread
   *
   * @return true if an exception was detected
   */
	static bool CheckAndClearException(JNIEnv* Env);
};
