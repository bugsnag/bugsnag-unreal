#include "AndroidPlatformConfiguration.h"
#include "JNIUtilities.h"

jobject FAndroidPlatformConfiguration::Parse(JNIEnv* Env,
	const JNIReferenceCache* Cache,
	const TSharedPtr<FBugsnagConfiguration>& Config)
{
	jstring jApiKey = FAndroidPlatformJNI::ParseFString(Env, Config->GetApiKey());
	if (jApiKey)
	{
		jobject jConfig = (*Env).NewObject(Cache->ConfigClass,
			Cache->ConfigConstructor, jApiKey);
		return jConfig;
	}
	return NULL;
}
