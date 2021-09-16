#include "Misc/AutomationTest.h"

#include "BugsnagConfiguration.h"

//
// Macros copied from Engine/Source/Developer/AutomationDriver/Private/Specs/AutomationDriver.spec.cpp
//
#define TEST_TRUE(expression) \
	EPIC_TEST_BOOLEAN_(TEXT(#expression), expression, true)

#define TEST_FALSE(expression) \
	EPIC_TEST_BOOLEAN_(TEXT(#expression), expression, false)

#define TEST_EQUAL(expression, expected) \
	EPIC_TEST_BOOLEAN_(TEXT(#expression), expression, expected)

#define EPIC_TEST_BOOLEAN_(text, expression, expected) \
	TestEqual(text, expression, expected);

//
// This is an example of an Automation Spec, a newer type of test that follows BDD methodology.
//
// https://docs.unrealengine.com/4.27/en-US/TestingAndOptimization/Automation/AutomationSpec/
//
BEGIN_DEFINE_SPEC(FBugsnagConfigurationSpec, "Bugsnag.FBugsnagConfigurationSpec",
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
END_DEFINE_SPEC(FBugsnagConfigurationSpec)
void FBugsnagConfigurationSpec::Define()
{
	static const FString ValidApiKey = TEXT("0192837465afbecd0192837465afbecd");

	Describe("ApiKey", [this]()
		{
			It("Should be loaded from DefaultEngine.ini", [this]()
				{
					TSharedPtr<FBugsnagConfiguration> Configuration = FBugsnagConfiguration::Load();
					TEST_EQUAL(Configuration->GetApiKey(), TEXT("YOUR-API-KEY"));
				});

			It("Should log a warning if a non-empty invalid ApiKey is provided", [this]()
				{
					AddExpectedError(TEXT("Invalid configuration. ApiKey should be a string with 32 hexadecimal characters, got 0192837465afbecd"),
						EAutomationExpectedErrorFlags::Exact, 1);
					FBugsnagConfiguration(TEXT("0192837465afbecd"));
				});
		});

	Describe("Callbacks", [this]()
		{
			It("Should accept lambdas", [this]()
				{
					FBugsnagConfiguration Configuration(ValidApiKey);

					Configuration.AddOnBreadcrumb([](IBugsnagBreadcrumb* Breadcrumb)
						{
							UE_LOG(LogCore, Display, TEXT("Nothing to see here"));
							return true;
						});

					Configuration.AddOnBreadcrumb([](IBugsnagBreadcrumb* Breadcrumb)
						{
							UE_LOG(LogCore, Display, TEXT("Nothing to see here"));
							return false;
						});

					Configuration.AddOnError([](class IBugsnagEvent* Error)
						{
							UE_LOG(LogCore, Display, TEXT("Nothing to see here"));
							return true;
						});

					Configuration.AddOnError([](class IBugsnagEvent* Error)
						{
							UE_LOG(LogCore, Display, TEXT("Nothing to see here"));
							return false;
						});

					Configuration.AddOnSession([](IBugsnagSession* Session)
						{
							UE_LOG(LogCore, Display, TEXT("Nothing to see here"));
							return true;
						});

					Configuration.AddOnSession([](IBugsnagSession* Session)
						{
							UE_LOG(LogCore, Display, TEXT("Nothing to see here"));
							return false;
						});
				});
		});

	Describe("Endpoints", [this]()
		{
			It("Should have sensible defaults", [this]()
				{
					FBugsnagConfiguration Configuration(ValidApiKey);
					TEST_EQUAL(Configuration.GetEndpoints().GetNotify(), TEXT("https://notify.bugsnag.com"));
					TEST_EQUAL(Configuration.GetEndpoints().GetSessions(), TEXT("https://sessions.bugsnag.com"));
				});

			It("Should log a warning if only the Notify endpoints is overriden", [this]()
				{
					AddExpectedError(TEXT("Invalid configuration. Endpoints.Notify cannot be set without also setting Endpoints.Sessions. "
										  "Sessions will not be sent to Bugsnag."),
						EAutomationExpectedErrorFlags::Exact, 1);

					FString Custom = TEXT("https://notify.bugsnag-example.com");
					FBugsnagConfiguration Configuration(ValidApiKey);
					Configuration.SetEndpoints(Custom, TEXT("https://sessions.bugsnag.com"));
					TEST_EQUAL(Configuration.GetEndpoints().GetNotify(), Custom);
					// Should not send session data, to avoid leaking data to the cloud Bugsnag instance
					TEST_EQUAL(Configuration.GetEndpoints().GetSessions(), TEXT(""));
				});

			It("Should log a warning if only the Session endpoints is overriden", [this]()
				{
					AddExpectedError(TEXT("Invalid configuration. Endpoints.Sessions cannot be set without also setting Endpoints.Notify. "
										  "Events will not be sent to Bugsnag."),
						EAutomationExpectedErrorFlags::Exact, 1);

					FString Custom = TEXT("https://sessions.bugsnag-example.com");
					FBugsnagConfiguration Configuration(ValidApiKey);
					Configuration.SetEndpoints(TEXT("https://notify.bugsnag.com"), Custom);
					TEST_EQUAL(Configuration.GetEndpoints().GetSessions(), Custom);
					// Should not send event data, to avoid leaking data to the cloud Bugsnag instance
					TEST_EQUAL(Configuration.GetEndpoints().GetNotify(), TEXT(""));
				});
		});

	Describe("MaxBreadcrumbs", [this]()
		{
			It("Should be 50 by default", [this]()
				{
					TEST_EQUAL((int32)FBugsnagConfiguration(ValidApiKey).GetMaxBreadcrumbs(), 50);
				});

			It("Should accept a value of 100", [this]()
				{
					FBugsnagConfiguration Configuration(ValidApiKey);
					Configuration.SetMaxBreadcrumbs(100);
					TEST_EQUAL((int32)Configuration.GetMaxBreadcrumbs(), 100);
				});

			It("Should reject a value of 101", [this]()
				{
					AddExpectedError(TEXT("Invalid configuration. MaxBreadcrumbs should be a number between 0-100, got 101"),
						EAutomationExpectedErrorFlags::Exact, 1);
					FBugsnagConfiguration Configuration(ValidApiKey);
					Configuration.SetMaxBreadcrumbs(101);
					TEST_EQUAL((int32)Configuration.GetMaxBreadcrumbs(), 50);
				});
		});

	Describe("Metadata", [this]()
		{
			It("Should not crash when accessing non-existent sections", [this]()
				{
					FBugsnagConfiguration Configuration(ValidApiKey);
					Configuration.ClearMetadata(TEXT("Nope"));
					Configuration.ClearMetadata(TEXT("Nope"), TEXT("Nope"));
					TEST_FALSE(Configuration.GetMetadata(TEXT("Nope")).IsValid());
					TEST_FALSE(Configuration.GetMetadata(TEXT("Nope"), TEXT("Nope")).IsValid());
				});

			It("Should allow adding, replacing, and removing sections", [this]()
				{
					FBugsnagConfiguration Configuration(ValidApiKey);

					TSharedPtr<FJsonObject> Info = MakeShareable(new FJsonObject);
					Info->SetStringField(TEXT("foo"), TEXT("bar"));
					Configuration.AddMetadata(TEXT("Info"), Info);

					TEST_TRUE(Configuration.GetMetadata(TEXT("Info")).IsValid());
					TEST_EQUAL(Configuration.GetMetadata(TEXT("Info"))->GetStringField(TEXT("foo")), TEXT("bar"));

					Info = MakeShareable(new FJsonObject);
					Info->SetStringField(TEXT("bar"), TEXT("baz"));
					Configuration.AddMetadata(TEXT("Info"), Info);

					TEST_TRUE(Configuration.GetMetadata(TEXT("Info")).IsValid());
					TEST_FALSE(Configuration.GetMetadata(TEXT("Info"))->HasField(TEXT("foo")));
					TEST_EQUAL(Configuration.GetMetadata(TEXT("Info"))->GetStringField(TEXT("bar")), TEXT("baz"));

					Configuration.ClearMetadata(TEXT("Info"));
					TEST_FALSE(Configuration.GetMetadata(TEXT("Info")).IsValid());
				});

			It("Should allow mutating sections", [this]()
				{
					FBugsnagConfiguration Configuration(ValidApiKey);

					Configuration.AddMetadata(TEXT("Info"), TEXT("foo"), MakeShared<FJsonValueString>(TEXT("bar")));
					TEST_EQUAL(Configuration.GetMetadata(TEXT("Info"), TEXT("foo"))->AsString(), TEXT("bar"));

					Configuration.AddMetadata(TEXT("Info"), TEXT("foo"), MakeShared<FJsonValueString>(TEXT("baz")));
					TEST_EQUAL(Configuration.GetMetadata(TEXT("Info"), TEXT("foo"))->AsString(), TEXT("baz"));

					Configuration.AddMetadata(TEXT("Info"), TEXT("foo"), nullptr);
					TEST_FALSE(Configuration.GetMetadata(TEXT("Info"), TEXT("foo")).IsValid());

					Configuration.AddMetadata(TEXT("Info"), TEXT("name"), MakeShared<FJsonValueString>(TEXT("test")));
					TEST_EQUAL(Configuration.GetMetadata(TEXT("Info"), TEXT("name"))->AsString(), TEXT("test"));

					Configuration.ClearMetadata(TEXT("Info"), TEXT("name"));
					TEST_FALSE(Configuration.GetMetadata(TEXT("Info"), TEXT("name")).IsValid());
				});
		});
}
