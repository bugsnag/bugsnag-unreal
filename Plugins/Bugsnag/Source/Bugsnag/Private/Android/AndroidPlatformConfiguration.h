#pragma once

#include <jni.h>

#include "BugsnagConfiguration.h"
#include "JNIUtilities.h"

class FAndroidPlatformConfiguration
{
public:
	/**
   * Convert an FBugsnagConfiguration into a Java bugsnag.Configuration object
   *
   * @param Env      A JNI environment for the current thread
   * @param JNICache A set of references for all requires Java classes/methods
   * @param Config   The configuration
   *
   * @return A Java configuration object or null if the configuration failed to initialize
   */
	static jobject Parse(JNIEnv* Env, const JNIReferenceCache* Cache, const TSharedPtr<FBugsnagConfiguration>& Config);
};
