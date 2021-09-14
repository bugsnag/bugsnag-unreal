#pragma once

#include <jni.h>

typedef struct
{
	bool loaded;
	bool initialized;
	jclass InterfaceClass;
	jclass BugsnagClass;
	jclass TraceClass;
	jclass ConfigClass;
	jclass SeverityClass;

	jmethodID BugsnagStartMethod;
	jmethodID BugsnagNotifyMethod;
	jmethodID ConfigConstructor;
	jmethodID ConfigSetApiKey;
	jmethodID TraceConstructor;

	jfieldID SeverityField;
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
   * Check if a Java runtime exception was thrown and if so, clear it.
   *
   * @param Env A JNI environment for the current thread
   *
   * @return true if an exception was detected
   */
	static bool CheckAndClearException(JNIEnv* Env);
};
