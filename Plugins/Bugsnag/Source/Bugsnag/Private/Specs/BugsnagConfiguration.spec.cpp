#include "Misc/AutomationTest.h"

#include "BugsnagConfiguration.h"

#define TEST_TRUE(expression) \
	TestTrue(TEXT(#expression), expression)

#define TEST_FALSE(expression) \
	TestFalse(TEXT(#expression), expression)

#define TEST_EQUAL(expression, expected) \
	TestEqual(TEXT(#expression), expression, expected)

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

					Configuration.AddOnBreadcrumb([](TSharedRef<IBugsnagBreadcrumb> Breadcrumb)
						{
							UE_LOG(LogCore, Display, TEXT("Nothing to see here"));
							return true;
						});

					Configuration.AddOnBreadcrumb([](TSharedRef<IBugsnagBreadcrumb> Breadcrumb)
						{
							UE_LOG(LogCore, Display, TEXT("Nothing to see here"));
							return false;
						});

					Configuration.AddOnSession([](TSharedRef<IBugsnagSession> Session)
						{
							UE_LOG(LogCore, Display, TEXT("Nothing to see here"));
							return true;
						});

					Configuration.AddOnSession([](TSharedRef<IBugsnagSession> Session)
						{
							UE_LOG(LogCore, Display, TEXT("Nothing to see here"));
							return false;
						});
				});
		});

	Describe("EnabledBreadcrumbTypes", [this]()
		{
			It("All are enabled by default", [this]()
				{
					TEST_EQUAL(FBugsnagConfiguration(ValidApiKey).GetEnabledBreadcrumbTypes(), EBugsnagEnabledBreadcrumbTypes::All);
					TEST_EQUAL(FBugsnagConfiguration::Load()->GetEnabledBreadcrumbTypes(), EBugsnagEnabledBreadcrumbTypes::All);
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

					TSharedRef<FJsonObject> Info = MakeShared<FJsonObject>();
					Info->SetStringField(TEXT("foo"), TEXT("bar"));
					Configuration.AddMetadata(TEXT("Info"), Info);

					TEST_TRUE(Configuration.GetMetadata(TEXT("Info")).IsValid());
					TEST_EQUAL(Configuration.GetMetadata(TEXT("Info"))->GetStringField(TEXT("foo")), TEXT("bar"));

					Info = MakeShared<FJsonObject>();
					Info->SetStringField(TEXT("bar"), TEXT("baz"));
					Configuration.ClearMetadata(TEXT("Info"));
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

					Configuration.AddMetadata(TEXT("Info"), TEXT("foo"), TSharedPtr<FJsonValue>(nullptr));
					TEST_FALSE(Configuration.GetMetadata(TEXT("Info"), TEXT("foo")).IsValid());

					Configuration.AddMetadata(TEXT("Info"), TEXT("name"), MakeShared<FJsonValueString>(TEXT("test")));
					TEST_EQUAL(Configuration.GetMetadata(TEXT("Info"), TEXT("name"))->AsString(), TEXT("test"));

					Configuration.AddMetadata(TEXT("Info"), TEXT("name"), TEXT("test_1"));
					TEST_EQUAL(Configuration.GetMetadata(TEXT("Info"), TEXT("name"))->AsString(), TEXT("test_1"));

					Configuration.AddMetadata(TEXT("Info"), TEXT("name"), "test_2");
					TEST_EQUAL(Configuration.GetMetadata(TEXT("Info"), TEXT("name"))->AsString(), TEXT("test_2"));

					Configuration.ClearMetadata(TEXT("Info"), TEXT("name"));
					TEST_FALSE(Configuration.GetMetadata(TEXT("Info"), TEXT("name")).IsValid());
				});

			It("Should merge sections when added", [this]()
				{
					FBugsnagConfiguration Configuration(ValidApiKey);

					const TCHAR* const Section = TEXT("Section");

					TSharedRef<FJsonObject> Info1 = MakeShared<FJsonObject>();
					Info1->SetStringField(TEXT("foo"), TEXT("bar"));
					Configuration.AddMetadata(Section, Info1);

					TSharedRef<FJsonObject> Info2 = MakeShared<FJsonObject>();
					Info2->SetStringField(TEXT("bar"), TEXT("foo"));
					Configuration.AddMetadata(Section, Info2);

					TEST_EQUAL(Configuration.GetMetadata(Section, TEXT("foo"))->AsString(), TEXT("bar"));
					TEST_EQUAL(Configuration.GetMetadata(Section, TEXT("bar"))->AsString(), TEXT("foo"));

					TEST_EQUAL(Info1->Values.Num(), 1);
					TEST_EQUAL(Info2->Values.Num(), 1);
				});

			It("Should not remove other values when adding one to a section", [this]()
				{
					FBugsnagConfiguration Configuration(ValidApiKey);

					const TCHAR* const Section = TEXT("Section");

					Configuration.AddMetadata(Section, TEXT("name"), MakeShared<FJsonValueString>(TEXT("Bob")));
					Configuration.AddMetadata(Section, TEXT("email"), MakeShared<FJsonValueString>(TEXT("bob@example.com")));
					TEST_EQUAL(Configuration.GetMetadata(Section, TEXT("name"))->AsString(), TEXT("Bob"));
					TEST_EQUAL(Configuration.GetMetadata(Section, TEXT("email"))->AsString(), TEXT("bob@example.com"));
				});
		});
}
