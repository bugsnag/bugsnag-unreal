#include "ExampleBlueprintFunctionLibrary.h"

#include "BugsnagFunctionLibrary.h"
#include "BugsnagBreadcrumb.h"

void UExampleBlueprintFunctionLibrary::InvalidMemoryAccess()
{
	volatile int* ptr = NULL;
	*ptr = 42;
}

void UExampleBlueprintFunctionLibrary::StackOverflow()
{
	TFunction<void()> foo = []
	{
		char buffer[300];
		snprintf(buffer, sizeof(buffer), "%s:%d", __FILE__, __LINE__);
		UExampleBlueprintFunctionLibrary::StackOverflow();
	};
	foo();
}

void UExampleBlueprintFunctionLibrary::Notify()
{
	UBugsnagFunctionLibrary::Notify(TEXT("Non-fatal"), TEXT("Notify() called from C++"));
}

const FString UExampleBlueprintFunctionLibrary::GetApiKeyFromConfig()
{
	TSharedPtr<FBugsnagConfiguration> Configuration = FBugsnagConfiguration::Load();

	return Configuration->GetApiKey();
}

void UExampleBlueprintFunctionLibrary::OneThousandBreadcrumbs()
{
	UBugsnagFunctionLibrary::AddMetadata(TEXT("My section"), TEXT("password"), TEXT("mypassword"));
	UBugsnagFunctionLibrary::AddMetadata(TEXT("My section"), TEXT("my key"), TEXT("my value")); 

	UBugsnagFunctionLibrary::AddOnBreadcrumb([](TSharedRef<IBugsnagBreadcrumb> Breadcrumb){
		if (Breadcrumb->GetMessage() == TEXT("Ignore")){
			return false;
		} else {
			return true;
		}
	});

	TSharedPtr<FJsonObject> Metadata = MakeShared<FJsonObject>();
	Metadata->SetBoolField("cronut", false);
	Metadata->SetNumberField("macaron", 3);
	Metadata->SetStringField("password", TEXT("40"));

	for(int i = 0; i < 1000; i++){			


		UBugsnagFunctionLibrary::LeaveBreadcrumb(TEXT("With metadata"), Metadata, EBugsnagBreadcrumbType::Manual);
		UBugsnagFunctionLibrary::LeaveBreadcrumb(TEXT("Without metadata"), EBugsnagBreadcrumbType::Manual);
		UBugsnagFunctionLibrary::LeaveBreadcrumb(TEXT("Ignore"), EBugsnagBreadcrumbType::Manual);
		UBugsnagFunctionLibrary::LeaveBreadcrumb(TEXT("User breadcrumb"), EBugsnagBreadcrumbType::User);
	}
}

void UExampleBlueprintFunctionLibrary::iOSException()
{
#if PLATFORM_IOS
    [[NSArray array] objectAtIndex:42];
#endif
}

int user = 0;

void UExampleBlueprintFunctionLibrary::StartFromCode(const FString& ApiKey)
{
	TSharedPtr<FBugsnagConfiguration> Configuration = FBugsnagConfiguration::Load();
	if (Configuration.IsValid())
	{
		Configuration->SetApiKey(ApiKey);
	}
	else
	{
		UE_LOG(LogBugsnag, Error, TEXT("Could not load configuration from DefaultEngine.ini"));
		Configuration = MakeShared<FBugsnagConfiguration>(ApiKey);
	}

	Configuration->AddMetadata(TEXT("My section"), TEXT("Metadata from config"), TEXT("value"));
	Configuration->ClearMetadata(TEXT("My section"));
	Configuration->AddMetadata(TEXT("My section"), TEXT("Metadata from config 2"), TEXT("value"));

	Configuration->AddOnSendError([](TSharedRef<IBugsnagEvent> Event)
	{
		Event->AddMetadata(TEXT("My section"), TEXT("Metadata from config->AddOnSendError"), TEXT("Value"));
		return true;
	});
	Configuration->AddOnBreadcrumb([](TSharedRef<IBugsnagBreadcrumb> Breadcrumb)
	{
		Breadcrumb->SetMessage(TEXT("Message overridden by config->AddOnBreadcrumb"));
		return true;
	});
	Configuration->AddOnSession([](TSharedRef<IBugsnagSession> Session)
	{
		Session->SetUser(FString(TEXT("UserId from config->AddOnSession")));
		return true; 
	});
	Configuration->SetRedactedKeys({TEXT("macaron")});
	
	// Configuration->SetUser(FString(TEXT("userid from code config")), FString(TEXT("email from code config")), FString(TEXT("name from code config")));
	
	UBugsnagFunctionLibrary::Start(Configuration.ToSharedRef());

	UBugsnagFunctionLibrary::AddOnSendError([](TSharedRef<IBugsnagEvent> Event)
	{
		Event->AddMetadata(TEXT("My section"), TEXT("Metadata from UBugsnagFunctionLibrary::AddOnSendError"), TEXT("Value"));
		return true;
	});
	UBugsnagFunctionLibrary::AddOnBreadcrumb([](TSharedRef<IBugsnagBreadcrumb> Breadcrumb)
	{
		Breadcrumb->SetMessage(TEXT("Message overridden by UBugsnagFunctionLibrary::AddOnBreadcrumb"));
		return true;
	});
	UBugsnagFunctionLibrary::AddOnSession([](TSharedRef<IBugsnagSession> Session)
	{
		Session->GetDevice()->SetId(FString::FromInt(user++));
		return true; 
	});
}

//
// Unreal Engine disables C++ exceptions on non-desktop platforms, so
// for now there is no example throwing a C++ exception.
//
// See PLATFORM_EXCEPTIONS_DISABLED
//
