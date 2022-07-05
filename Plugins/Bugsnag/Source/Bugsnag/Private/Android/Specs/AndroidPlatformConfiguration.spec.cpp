// Copyright 2022 Bugsnag. All Rights Reserved.

#include "Android/AndroidJavaEnv.h"
#include "AutomationTest.h"

#include "../AndroidPlatformConfiguration.h"

//
// To run the unit tests on Android:
// * Build & run the example app on a connected Android device.
// * Open Unreal Editor's "Session Frontend" and find the running game in "My Sessions"
// * Click the "Automation" tab, select the tests to run, and click "Start Tests"!
//
BEGIN_DEFINE_SPEC(FAndroidPlatformConfigurationSpec, "Bugsnag.FAndroidPlatformConfigurationSpec",
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
END_DEFINE_SPEC(FAndroidPlatformConfigurationSpec)
void FAndroidPlatformConfigurationSpec::Define()
{
	static const FString ApiKey = TEXT("0192837465afbecd0192837465afbecd");

	static JNIReferenceCache JNICache;
	if (!JNICache.loaded)
	{
		JNICache.loaded = FAndroidPlatformJNI::LoadReferenceCache(AndroidJavaEnv::GetJavaEnv(), &JNICache);
	}

	Describe("Telemetry", [this]()
		{
			It("Should contain all types by default", [this]()
				{
					JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
					jmethodID SizeMethod = Env->GetMethodID(Env->FindClass("java/util/Set"), "size", "()I");
					jmethodID GetTelemetryMethod = Env->GetMethodID(JNICache.ConfigClass, "getTelemetry", "()Ljava/util/Set;");
					TEST_FALSE(Env->ExceptionCheck());

					TSharedRef<FBugsnagConfiguration> Configuration = MakeShared<FBugsnagConfiguration>(ApiKey);

					jobject AndroidConfig = FAndroidPlatformConfiguration::Parse(Env, &JNICache, Configuration);
					jobject AndroidTelemetry = Env->CallObjectMethod(AndroidConfig, GetTelemetryMethod);
					TEST_TRUE(AndroidTelemetry != nullptr);
					TEST_FALSE(Env->ExceptionCheck());
					int Size = Env->CallIntMethod(AndroidTelemetry, SizeMethod);
					TEST_FALSE(Env->ExceptionCheck());
					TestEqual(TEXT("Telemetry Set should contain [INTERNAL_ERRORS]"), Size, 1);
				});

			It("Should be empty after setting EBugsnagTelemetryTypes::None", [this]()
				{
					JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
					jmethodID SizeMethod = Env->GetMethodID(Env->FindClass("java/util/Set"), "size", "()I");
					jmethodID GetTelemetryMethod = Env->GetMethodID(JNICache.ConfigClass, "getTelemetry", "()Ljava/util/Set;");

					TSharedRef<FBugsnagConfiguration> Configuration = MakeShared<FBugsnagConfiguration>(ApiKey);
					Configuration->SetTelemetry(EBugsnagTelemetryTypes::None);

					jobject AndroidConfig = FAndroidPlatformConfiguration::Parse(Env, &JNICache, Configuration);
					jobject AndroidTelemetry = Env->CallObjectMethod(AndroidConfig, GetTelemetryMethod);
					TEST_TRUE(AndroidTelemetry != nullptr);
					TEST_FALSE(Env->ExceptionCheck());
					int Size = Env->CallIntMethod(AndroidTelemetry, SizeMethod);
					TEST_FALSE(Env->ExceptionCheck());
					TestEqual(TEXT("Telemetry Set should be empty"), Size, 0);
				});
		});
}
