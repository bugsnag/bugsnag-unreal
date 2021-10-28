#include "AndroidPlatformConfiguration.h"
#include "JNIUtilities.h"
#include "Shorthand.h"

// Call a void JNI method, sending the arguments exactly as specified. Clears
// any exceptions if thrown, then returning null.
#define jniCallWithObjects(env, obj, method, ...)    \
	(*env).CallVoidMethod(obj, method, __VA_ARGS__); \
	ReturnNullOnException(env);

// Call a void JNI method, parsing value as a jboolean
#define jniCallWithBool(env, obj, method, value) \
	jniCallWithObjects(env, obj, method, FAndroidPlatformJNI::ParseBoolean(value));

// Call a void JNI method, parsing value as a (nullable) jstring
#define jniCallWithString(env, obj, method, value) \
	jniCallWithObjects(env, obj, method, FAndroidPlatformJNI::ParseFStringPtr(env, value));

// Call a void JNI method, parsing value as a Java HashSet<String>
#define jniCallWithSet(env, cache, obj, method, value)                         \
	{                                                                          \
		jobject jSet = FAndroidPlatformJNI::ParseStringSet(env, cache, value); \
		ReturnNullOnFail(jSet);                                                \
		jniCallWithObjects(env, obj, method, jSet);                            \
	}

jobject FAndroidPlatformConfiguration::Parse(JNIEnv* Env,
	const JNIReferenceCache* Cache,
	const TSharedPtr<FBugsnagConfiguration>& Config)
{
	jstring jApiKey = FAndroidPlatformJNI::ParseFString(Env, Config->GetApiKey());
	ReturnNullOnFail(jApiKey);

	jobject jConfig = (*Env).NewObject(Cache->ConfigClass, Cache->ConfigConstructor, jApiKey);
	ReturnNullOnException(Env);

	FAndroidPlatformJNI::SetNotifierInfo(Env, Cache, jConfig);
	if (Config->GetAppType().IsValid())
	{
		jniCallWithString(Env, jConfig, Cache->ConfigSetAppType, Config->GetAppType());
	}
	if (Config->GetAppVersion().IsValid())
	{
		jniCallWithString(Env, jConfig, Cache->ConfigSetAppVersion, Config->GetAppVersion());
	}
	jniCallWithBool(Env, jConfig, Cache->ConfigSetAutoDetectErrors, Config->GetAutoDetectErrors());
	jniCallWithBool(Env, jConfig, Cache->ConfigSetAutoTrackSessions, Config->GetAutoTrackSessions());
	if (Config->GetContext().IsValid())
	{
		jniCallWithString(Env, jConfig, Cache->ConfigSetContext, Config->GetContext());
	}
	if (Config->GetDiscardClasses().Num())
	{
		jniCallWithSet(Env, Cache, jConfig, Cache->ConfigSetDiscardClasses, Config->GetDiscardClasses());
	}
	jobject jEnabledTypes = FAndroidPlatformJNI::ParseBreadcrumbTypeSet(Env, Cache, Config->GetEnabledBreadcrumbTypes());
	ReturnNullOnFail(jEnabledTypes);
	jniCallWithObjects(Env, jConfig, Cache->ConfigSetEnabledBreadcrumbTypes, jEnabledTypes);

	FBugsnagErrorTypes ErrorTypes = Config->GetEnabledErrorTypes();
	jobject jErrorTypes = (*Env).NewObject(Cache->ErrorTypesClass, Cache->ErrorTypesConstructor,
		FAndroidPlatformJNI::ParseBoolean(ErrorTypes.bANRs),
		FAndroidPlatformJNI::ParseBoolean(ErrorTypes.bNativeCrashes),
		FAndroidPlatformJNI::ParseBoolean(ErrorTypes.bUnhandledExceptions),
		JNI_FALSE);
	ReturnNullOnFail(jErrorTypes);
	jniCallWithObjects(Env, jConfig, Cache->ConfigSetEnabledErrorTypes, jErrorTypes);

	if (Config->GetEnabledReleaseStages().Num())
	{
		jniCallWithSet(Env, Cache, jConfig, Cache->ConfigSetEnabledReleaseStages, Config->GetEnabledReleaseStages());
	}

	FBugsnagEndpointConfiguration Endpoints = Config->GetEndpoints();
	jobject jNotifyUrl = FAndroidPlatformJNI::ParseFString(Env, Endpoints.GetNotify());
	ReturnNullOnFail(jNotifyUrl);
	jobject jSessionsUrl = FAndroidPlatformJNI::ParseFString(Env, Endpoints.GetSessions());
	ReturnNullOnFail(jSessionsUrl);
	jobject jEndpoints = (*Env).NewObject(Cache->EndpointConfigurationClass, Cache->EndpointConfigurationConstructor, jNotifyUrl, jSessionsUrl);
	if (FAndroidPlatformJNI::CheckAndClearException(Env) || !jEndpoints)
	{
		return nullptr;
	}
	jniCallWithObjects(Env, jConfig, Cache->ConfigSetEndpoints, jEndpoints);

	jniCallWithObjects(Env, jConfig, Cache->ConfigSetLaunchDurationMillis, Config->GetLaunchDurationMillis());
	jniCallWithObjects(Env, jConfig, Cache->ConfigSetMaxBreadcrumbs, Config->GetMaxBreadcrumbs());
	jniCallWithObjects(Env, jConfig, Cache->ConfigSetMaxPersistedEvents, Config->GetMaxPersistedEvents());
	jniCallWithBool(Env, jConfig, Cache->ConfigSetPersistUser, Config->GetPersistUser());
	if (Config->GetRedactedKeys().Num())
	{
		jniCallWithSet(Env, Cache, jConfig, Cache->ConfigSetRedactedKeys, Config->GetRedactedKeys());
	}
	if (Config->GetReleaseStage().IsValid())
	{
		jniCallWithString(Env, jConfig, Cache->ConfigSetReleaseStage, Config->GetReleaseStage());
	}
	else
	{
		// explicitly set production for shipping builds, otherwise use bugsnag-android default detection
#if UE_BUILD_SHIPPING
		jniCallWithString(Env, jConfig, Cache->ConfigSetReleaseStage, MakeShareable(new FString("production")));
#endif
	}
	jniCallWithBool(Env, jConfig, Cache->ConfigSetSendLaunchCrashesSynchronously, Config->GetSendLaunchCrashesSynchronously());
	jobject jThreadPolicy = FAndroidPlatformJNI::ParseThreadSendPolicy(Env, Cache, Config->GetSendThreads());
	ReturnNullOnFail(jThreadPolicy);
	jniCallWithObjects(Env, jConfig, Cache->ConfigSetSendThreads, jThreadPolicy);

	if (Config->GetVersionCode().IsSet())
	{
		jobject jVersionCode = FAndroidPlatformJNI::ParseInteger(Env, Cache, *Config->GetVersionCode());
		ReturnNullOnFail(jVersionCode);
		jniCallWithObjects(Env, jConfig, Cache->ConfigSetVersionCode, jVersionCode);
	}
	if (!Config->GetUser().IsEmpty())
	{
		jstring jId = FAndroidPlatformJNI::ParseFStringPtr(Env, Config->GetUser().GetId());
		jstring jEmail = FAndroidPlatformJNI::ParseFStringPtr(Env, Config->GetUser().GetEmail());
		jstring jName = FAndroidPlatformJNI::ParseFStringPtr(Env, Config->GetUser().GetName());
		jniCallWithObjects(Env, jConfig, Cache->ConfigSetUser, jId, jEmail, jName);
	}

	for (auto& Item : Config->GetMetadataValues())
	{
		jstring jSection = FAndroidPlatformJNI::ParseFString(Env, Item.Key);
		if (!jSection)
		{
			continue;
		}
		jobject jValues = FAndroidPlatformJNI::ParseJsonObject(Env, Cache, Item.Value);
		if (!jValues)
		{
			continue;
		}
		(*Env).CallVoidMethod(jConfig, Cache->ConfigAddMetadata, jSection, jValues);
		FAndroidPlatformJNI::CheckAndClearException(Env);
	}

	jobject jPlugin = (*Env).NewObject(Cache->BugsnagUnrealPluginClass, Cache->BugsnagUnrealPluginConstructor);
	ReturnNullOnException(Env);
	jniCallWithObjects(Env, jConfig, Cache->ConfigAddPlugin, jPlugin);

	return jConfig;
}
