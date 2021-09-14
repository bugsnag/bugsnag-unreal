#include "JNIUtilities.h"

#include <string>

#include "Android/AndroidJavaEnv.h"
#include "CoreMinimal.h"

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

#define CheckNonNull(env, var)     \
	if (var == NULL)               \
	{                              \
		if (env->ExceptionCheck()) \
		{                          \
			env->ExceptionClear(); \
		}                          \
		return false;              \
	}

bool FAndroidPlatformJNI::LoadReferenceCache(JNIEnv* env, JNIReferenceCache* cache)
{
	if (!env)
	{
		return false;
	}

	// Third-party classes are loaded via a class loader accessible to the Unreal JNI helpers
	cache->BugsnagClass = LoadJavaClass(env, "com.bugsnag.android.Bugsnag", true);
	CheckNonNull(env, cache->BugsnagClass);
	cache->ConfigClass = LoadJavaClass(env, "com.bugsnag.android.Configuration", true);
	CheckNonNull(env, cache->ConfigClass);
	cache->InterfaceClass = LoadJavaClass(env, "com.bugsnag.android.NativeInterface", true);
	CheckNonNull(env, cache->InterfaceClass);
	cache->SeverityClass = LoadJavaClass(env, "com.bugsnag.android.Severity", true);
	CheckNonNull(env, cache->SeverityClass);

	// Core classes are available through standard JNI functions only
	cache->TraceClass = LoadJavaClass(env, "java/lang/StackTraceElement", false);
	CheckNonNull(env, cache->TraceClass);

	cache->BugsnagStartMethod = (*env).GetStaticMethodID(cache->BugsnagClass, "start",
		"(Landroid/content/Context;Lcom/bugsnag/android/Configuration;)Lcom/bugsnag/android/Client;");
	CheckNonNull(env, cache->BugsnagStartMethod);
	cache->BugsnagNotifyMethod = (*env).GetStaticMethodID(cache->InterfaceClass, "notify",
		"([B[BLcom/bugsnag/android/Severity;[Ljava/lang/StackTraceElement;)V");
	CheckNonNull(env, cache->BugsnagNotifyMethod);
	cache->ConfigConstructor = (*env).GetMethodID(cache->ConfigClass, "<init>",
		"(Ljava/lang/String;)V");
	CheckNonNull(env, cache->ConfigConstructor);
	cache->ConfigSetApiKey = (*env).GetMethodID(cache->ConfigClass, "setApiKey",
		"(Ljava/lang/String;)V");
	CheckNonNull(env, cache->ConfigSetApiKey);
	cache->TraceConstructor = (*env).GetMethodID(cache->TraceClass, "<init>",
		"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V");
	CheckNonNull(env, cache->TraceConstructor);
	cache->SeverityField = (*env).GetStaticFieldID(cache->SeverityClass, "WARNING", "Lcom/bugsnag/android/Severity;");
	CheckNonNull(env, cache->SeverityField);

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

bool FAndroidPlatformJNI::CheckAndClearException(JNIEnv* Env)
{
	if ((*Env).ExceptionCheck())
	{
		(*Env).ExceptionClear();
		return true;
	}
	return false;
}
