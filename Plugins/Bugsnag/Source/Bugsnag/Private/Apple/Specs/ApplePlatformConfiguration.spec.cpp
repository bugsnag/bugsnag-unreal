// Copyright 2021 Bugsnag. All Rights Reserved.

#include "AutomationTest.h"

#include "../ApplePlatformConfiguration.h"
#include "BugsnagConfiguration.h"

#import <BugsnagPrivate/BugsnagInternals.h>

//
// This is an example of an Automation Spec, a newer type of test that follows BDD methodology.
//
// https://docs.unrealengine.com/en-US/TestingAndOptimization/Automation/AutomationSpec/
//
BEGIN_DEFINE_SPEC(FApplePlatformConfigurationSpec, "Bugsnag.FApplePlatformConfigurationSpec",
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
END_DEFINE_SPEC(FApplePlatformConfigurationSpec)
void FApplePlatformConfigurationSpec::Define()
{
	static const FString ApiKey = TEXT("0192837465afbecd0192837465afbecd");

	Describe("Properties", [this]()
		{
			It("Defaults", [this]()
				{
					TSharedRef<FBugsnagConfiguration> Configuration = MakeShared<FBugsnagConfiguration>(ApiKey);
					BugsnagConfiguration* CocoaConfig = FApplePlatformConfiguration::Configuration(Configuration);
					BugsnagConfiguration* DefaultConfig = [[BugsnagConfiguration alloc] initWithApiKey:@(TCHAR_TO_UTF8(*ApiKey))];
					TEST_TRUE(CocoaConfig.appHangThresholdMillis == DefaultConfig.appHangThresholdMillis);
					TEST_TRUE(CocoaConfig.enabledBreadcrumbTypes == DefaultConfig.enabledBreadcrumbTypes);
					TEST_TRUE(CocoaConfig.enabledErrorTypes.appHangs);
					TEST_TRUE(CocoaConfig.enabledErrorTypes.cppExceptions);
					TEST_TRUE(CocoaConfig.enabledErrorTypes.machExceptions);
					TEST_TRUE(CocoaConfig.enabledErrorTypes.ooms);
					TEST_TRUE(CocoaConfig.enabledErrorTypes.signals);
					TEST_TRUE(CocoaConfig.enabledErrorTypes.thermalKills);
					TEST_TRUE(CocoaConfig.enabledErrorTypes.unhandledExceptions);
					TEST_TRUE([CocoaConfig.apiKey isEqual:DefaultConfig.apiKey]);
					TEST_TRUE([CocoaConfig.appType isEqual:DefaultConfig.appType]);
					TEST_TRUE([CocoaConfig.bundleVersion isEqual:DefaultConfig.bundleVersion]);
					TEST_TRUE([CocoaConfig.redactedKeys isEqual:DefaultConfig.redactedKeys]);
				});

			It("ApiKey", [this]()
				{
					TSharedRef<FBugsnagConfiguration> Configuration = MakeShared<FBugsnagConfiguration>(ApiKey);
					BugsnagConfiguration* CocoaConfig = FApplePlatformConfiguration::Configuration(Configuration);
					TEST_EQUAL(UTF8_TO_TCHAR(CocoaConfig.apiKey.UTF8String), ApiKey);
				});

			It("AutoDetectErrors", [this]()
				{
					TSharedRef<FBugsnagConfiguration> Configuration = MakeShared<FBugsnagConfiguration>(ApiKey);
					Configuration->SetAutoDetectErrors(false);
					BugsnagConfiguration* CocoaConfig = FApplePlatformConfiguration::Configuration(Configuration);
					TEST_EQUAL(CocoaConfig.autoDetectErrors, Configuration->GetAutoDetectErrors());
				});

			It("AutoTrackSessions", [this]()
				{
					TSharedRef<FBugsnagConfiguration> Configuration = MakeShared<FBugsnagConfiguration>(ApiKey);
					Configuration->SetAutoTrackSessions(false);
					BugsnagConfiguration* CocoaConfig = FApplePlatformConfiguration::Configuration(Configuration);
					TEST_EQUAL(CocoaConfig.autoTrackSessions, Configuration->GetAutoTrackSessions());
				});

			It("Context", [this]()
				{
					TSharedRef<FBugsnagConfiguration> Configuration = MakeShared<FBugsnagConfiguration>(ApiKey);
					Configuration->SetContext(FString(TEXT("LevelOne")));
					BugsnagConfiguration* CocoaConfig = FApplePlatformConfiguration::Configuration(Configuration);
					TEST_EQUAL(UTF8_TO_TCHAR(CocoaConfig.context.UTF8String), TEXT("LevelOne"));
				});

			It("DiscardClasses", [this]()
				{
					TSharedRef<FBugsnagConfiguration> Configuration = MakeShared<FBugsnagConfiguration>(ApiKey);
					Configuration->SetDiscardClasses({TEXT("Error1"), TEXT("Error2")});
					BugsnagConfiguration* CocoaConfig = FApplePlatformConfiguration::Configuration(Configuration);
					TEST_TRUE([CocoaConfig.discardClasses containsObject:@"Error1"]);
					TEST_TRUE([CocoaConfig.discardClasses containsObject:@"Error2"]);
				});

			It("EnabledBreadcrumbTypes", [this]()
				{
					TSharedRef<FBugsnagConfiguration> Configuration = MakeShared<FBugsnagConfiguration>(ApiKey);
					Configuration->SetEnabledBreadcrumbTypes(EBugsnagEnabledBreadcrumbTypes::All & ~EBugsnagEnabledBreadcrumbTypes::Log);
					BugsnagConfiguration* CocoaConfig = FApplePlatformConfiguration::Configuration(Configuration);
					TEST_EQUAL(CocoaConfig.enabledBreadcrumbTypes, BSGEnabledBreadcrumbTypeAll & ~BSGEnabledBreadcrumbTypeLog);
				});

			It("EnabledErrorTypes", [this]()
				{
					TSharedRef<FBugsnagConfiguration> Configuration = MakeShared<FBugsnagConfiguration>(ApiKey);
					FBugsnagErrorTypes EnabledErrorTypes;
					EnabledErrorTypes.bOOMs = false;
					Configuration->SetEnabledErrorTypes(EnabledErrorTypes);
					BugsnagConfiguration* CocoaConfig = FApplePlatformConfiguration::Configuration(Configuration);
					TEST_FALSE(CocoaConfig.enabledErrorTypes.ooms);
				});

			It("EnabledReleaseStages", [this]()
				{
					TSharedRef<FBugsnagConfiguration> Configuration = MakeShared<FBugsnagConfiguration>(ApiKey);
					Configuration->SetEnabledReleaseStages({TEXT("production"), TEXT("staging")});
					BugsnagConfiguration* CocoaConfig = FApplePlatformConfiguration::Configuration(Configuration);
					TEST_TRUE([CocoaConfig.enabledReleaseStages containsObject:@"production"]);
					TEST_TRUE([CocoaConfig.enabledReleaseStages containsObject:@"staging"]);
				});

			It("RedactedKeys", [this]()
				{
					TSharedRef<FBugsnagConfiguration> Configuration = MakeShared<FBugsnagConfiguration>(ApiKey);
					Configuration->SetRedactedKeys({TEXT("password"), TEXT("secret")});
					BugsnagConfiguration* CocoaConfig = FApplePlatformConfiguration::Configuration(Configuration);
					TEST_TRUE([CocoaConfig.redactedKeys containsObject:@"password"]);
					TEST_TRUE([CocoaConfig.redactedKeys containsObject:@"secret"]);
				});

			It("AppHangThreshold", [this]()
				{
					TSharedRef<FBugsnagConfiguration> Configuration = MakeShared<FBugsnagConfiguration>(ApiKey);
					Configuration->SetAppHangThresholdMillis(FBugsnagConfiguration::AppHangThresholdFatalOnly);
					BugsnagConfiguration* CocoaConfig = FApplePlatformConfiguration::Configuration(Configuration);
					TEST_EQUAL(CocoaConfig.appHangThresholdMillis, BugsnagAppHangThresholdFatalOnly);

					Configuration->SetAppHangThresholdMillis(5000);
					CocoaConfig = FApplePlatformConfiguration::Configuration(Configuration);
					TEST_EQUAL((int64)CocoaConfig.appHangThresholdMillis, (int64)5000);
				});

			It("LaunchDurationMillis", [this]()
				{
					TSharedRef<FBugsnagConfiguration> Configuration = MakeShared<FBugsnagConfiguration>(ApiKey);
					Configuration->SetLaunchDurationMillis(0);
					BugsnagConfiguration* CocoaConfig = FApplePlatformConfiguration::Configuration(Configuration);
					TEST_EQUAL((int64)CocoaConfig.launchDurationMillis, (int64)0);
				});

			It("SendLaunchCrashesSynchronously", [this]()
				{
					TSharedRef<FBugsnagConfiguration> Configuration = MakeShared<FBugsnagConfiguration>(ApiKey);
					Configuration->SetSendLaunchCrashesSynchronously(false);
					BugsnagConfiguration* CocoaConfig = FApplePlatformConfiguration::Configuration(Configuration);
					TEST_EQUAL(CocoaConfig.sendLaunchCrashesSynchronously, NO);
				});

			It("MaxBreadcrumbs", [this]()
				{
					TSharedRef<FBugsnagConfiguration> Configuration = MakeShared<FBugsnagConfiguration>(ApiKey);
					Configuration->SetMaxBreadcrumbs(100);
					BugsnagConfiguration* CocoaConfig = FApplePlatformConfiguration::Configuration(Configuration);
					TEST_EQUAL((int32)CocoaConfig.maxBreadcrumbs, (int32)100);
				});

			It("MaxPersistedEvents", [this]()
				{
					TSharedRef<FBugsnagConfiguration> Configuration = MakeShared<FBugsnagConfiguration>(ApiKey);
					Configuration->SetMaxPersistedEvents(100);
					BugsnagConfiguration* CocoaConfig = FApplePlatformConfiguration::Configuration(Configuration);
					TEST_EQUAL((int32)CocoaConfig.maxPersistedEvents, (int32)100);
				});

			It("MaxPersistedSessions", [this]()
				{
					TSharedRef<FBugsnagConfiguration> Configuration = MakeShared<FBugsnagConfiguration>(ApiKey);
					Configuration->SetMaxPersistedSessions(100);
					BugsnagConfiguration* CocoaConfig = FApplePlatformConfiguration::Configuration(Configuration);
					TEST_EQUAL((int32)CocoaConfig.maxPersistedSessions, (int32)100);
				});

			It("PersistUser", [this]()
				{
					TSharedRef<FBugsnagConfiguration> Configuration = MakeShared<FBugsnagConfiguration>(ApiKey);
					Configuration->SetPersistUser(false);
					BugsnagConfiguration* CocoaConfig = FApplePlatformConfiguration::Configuration(Configuration);
					TEST_EQUAL(CocoaConfig.persistUser, NO);
				});

			It("Telemetry", [this]()
				{
					TSharedRef<FBugsnagConfiguration> Configuration = MakeShared<FBugsnagConfiguration>(ApiKey);
					BugsnagConfiguration* CocoaConfig = FApplePlatformConfiguration::Configuration(Configuration);
					TEST_TRUE(CocoaConfig.telemetry == BSGTelemetryAll);
					Configuration->SetTelemetry(EBugsnagTelemetryTypes::None);
					CocoaConfig = FApplePlatformConfiguration::Configuration(Configuration);
					TEST_TRUE(CocoaConfig.telemetry == 0);
					Configuration->SetTelemetry(EBugsnagTelemetryTypes::Usage);
					CocoaConfig = FApplePlatformConfiguration::Configuration(Configuration);
					TEST_TRUE(CocoaConfig.telemetry == BSGTelemetryUsage);
				});

			It("ReleaseStage", [this]()
				{
					TSharedRef<FBugsnagConfiguration> Configuration = MakeShared<FBugsnagConfiguration>(ApiKey);
					Configuration->SetReleaseStage(FString(TEXT("testing")));
					BugsnagConfiguration* CocoaConfig = FApplePlatformConfiguration::Configuration(Configuration);
					TEST_EQUAL(UTF8_TO_TCHAR(CocoaConfig.releaseStage.UTF8String), TEXT("testing"));
				});

			It("AppType", [this]()
				{
					TSharedRef<FBugsnagConfiguration> Configuration = MakeShared<FBugsnagConfiguration>(ApiKey);
					Configuration->SetAppType(FString(TEXT("unreal")));
					BugsnagConfiguration* CocoaConfig = FApplePlatformConfiguration::Configuration(Configuration);
					TEST_EQUAL(UTF8_TO_TCHAR(CocoaConfig.appType.UTF8String), TEXT("unreal"));
				});

			It("AppVersion", [this]()
				{
					TSharedRef<FBugsnagConfiguration> Configuration = MakeShared<FBugsnagConfiguration>(ApiKey);
					Configuration->SetAppVersion(FString(TEXT("1.2.3")));
					BugsnagConfiguration* CocoaConfig = FApplePlatformConfiguration::Configuration(Configuration);
					TEST_EQUAL(UTF8_TO_TCHAR(CocoaConfig.appVersion.UTF8String), TEXT("1.2.3"));
				});

			It("BundleVersion", [this]()
				{
					TSharedRef<FBugsnagConfiguration> Configuration = MakeShared<FBugsnagConfiguration>(ApiKey);
					Configuration->SetBundleVersion(FString(TEXT("123.4")));
					BugsnagConfiguration* CocoaConfig = FApplePlatformConfiguration::Configuration(Configuration);
					TEST_EQUAL(UTF8_TO_TCHAR(CocoaConfig.bundleVersion.UTF8String), TEXT("123.4"));
				});

			It("Endpoints", [this]()
				{
					TSharedRef<FBugsnagConfiguration> Configuration = MakeShared<FBugsnagConfiguration>(ApiKey);
					Configuration->SetEndpoints(TEXT("https://notify.example.com"), TEXT("https://sessions.example.com"));
					BugsnagConfiguration* CocoaConfig = FApplePlatformConfiguration::Configuration(Configuration);
					TEST_EQUAL(UTF8_TO_TCHAR(CocoaConfig.endpoints.notify.UTF8String), TEXT("https://notify.example.com"));
					TEST_EQUAL(UTF8_TO_TCHAR(CocoaConfig.endpoints.sessions.UTF8String), TEXT("https://sessions.example.com"));
				});

			It("User", [this]()
				{
					TSharedRef<FBugsnagConfiguration> Configuration = MakeShared<FBugsnagConfiguration>(ApiKey);
					Configuration->SetPersistUser(false);
					Configuration->SetUser(
						FString(TEXT("123")),
						FString(TEXT("foobar@example.com")),
						FString(TEXT("Foo Bar")));

					BugsnagConfiguration* CocoaConfig = FApplePlatformConfiguration::Configuration(Configuration);
					TEST_EQUAL(UTF8_TO_TCHAR(CocoaConfig.user.id.UTF8String), TEXT("123"));
					TEST_EQUAL(UTF8_TO_TCHAR(CocoaConfig.user.email.UTF8String), TEXT("foobar@example.com"));
					TEST_EQUAL(UTF8_TO_TCHAR(CocoaConfig.user.name.UTF8String), TEXT("Foo Bar"));

					Configuration->SetUser();
					CocoaConfig = FApplePlatformConfiguration::Configuration(Configuration);
					TEST_TRUE(CocoaConfig.user.id == nil);
					TEST_TRUE(CocoaConfig.user.email == nil);
					TEST_TRUE(CocoaConfig.user.name == nil);
				});

			It("Metadata", [this]()
				{
					TSharedRef<FBugsnagConfiguration> Configuration = MakeShared<FBugsnagConfiguration>(ApiKey);
					Configuration->AddMetadata(TEXT("Info"), TEXT("foo"), TEXT("bar"));

					TSharedRef<FJsonObject> JsonObject = MakeShared<FJsonObject>();
					JsonObject->SetBoolField(TEXT("bHasCompletedLevel1"), true);
					JsonObject->SetBoolField(TEXT("bHasCompletedLevel2"), false);
					Configuration->AddMetadata(TEXT("Flags"), JsonObject);

					BugsnagConfiguration* CocoaConfig = FApplePlatformConfiguration::Configuration(Configuration);

					id Info = [CocoaConfig getMetadataFromSection:@"Info"];
					TEST_TRUE([Info isEqual:@{@"foo": @"bar"}]);

					id Flags = [CocoaConfig getMetadataFromSection:@"Flags"];
					TEST_TRUE([Flags isEqual:(@{@"bHasCompletedLevel1": @YES,
						@"bHasCompletedLevel2": @NO})]);
				});

			It("Callbacks", [this]()
				{
					TSharedRef<FBugsnagConfiguration> Configuration = MakeShared<FBugsnagConfiguration>(ApiKey);

					bool OnBreadcrumbCalled = false;
					Configuration->AddOnBreadcrumb([&OnBreadcrumbCalled](TSharedRef<IBugsnagBreadcrumb> Breadcrumb) mutable
						{
							Breadcrumb->SetMessage(TEXT("I'm going to ignore you"));
							OnBreadcrumbCalled = true;
							return false;
						});

					bool OnSendErrorCalled = false;
					Configuration->AddOnSendError([&OnSendErrorCalled](TSharedRef<IBugsnagEvent> Error) mutable
						{
							OnSendErrorCalled = true;
							return false;
						});

					bool OnSessionCalled = false;
					Configuration->AddOnSession([&OnSessionCalled](TSharedRef<IBugsnagSession> Session) mutable
						{
							Session->SetUser(FString(TEXT("user123")));
							Session->GetApp()->SetReleaseStage(FString(TEXT("testing")));
							Session->GetDevice()->SetLocale(TOptional<FString>());
							OnSessionCalled = true;
							return false;
						});

					BugsnagConfiguration* CocoaConfig = FApplePlatformConfiguration::Configuration(Configuration);

					for (BugsnagOnBreadcrumbBlock Block in CocoaConfig.onBreadcrumbBlocks)
					{
						Block([[BugsnagBreadcrumb alloc] init]);
					}
					TEST_TRUE(OnBreadcrumbCalled);

					for (BugsnagOnErrorBlock Block in CocoaConfig.onSendBlocks)
					{
						Block([[BugsnagEvent alloc] init]);
					}
					// Not yet implemented - PLAT-7260
					// TEST_TRUE(OnSendErrorCalled);

					for (BugsnagOnSessionBlock Block in CocoaConfig.onSessionBlocks)
					{
						Block([[BugsnagSession alloc] init]);
					}
					TEST_TRUE(OnSessionCalled);
				});
		});
}
