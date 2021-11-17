// Copyright 2021 Bugsnag. All Rights Reserved.

#include "AutomationTest.h"

#include "../WrappedDevice.h"

BEGIN_DEFINE_SPEC(FWrappedDeviceSpec, "Bugsnag.FWrappedDeviceSpec",
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
END_DEFINE_SPEC(FWrappedDeviceSpec)
void FWrappedDeviceSpec::Define()
{
	Describe("FWrappedDevice", [this]()
		{
			It("Gets values from the Cocoa object", [this]()
				{
					BugsnagDevice* CocoaDevice = [[BugsnagDevice alloc] init];
					CocoaDevice.jailbroken = YES;
					CocoaDevice.id = @"uniqueId";
					CocoaDevice.locale = @"en_EN";
					CocoaDevice.manufacturer = @"Apple";
					CocoaDevice.model = @"iPhone99,9";
					CocoaDevice.modelNumber = @"D79AP";
					CocoaDevice.osName = @"iOS";
					CocoaDevice.osVersion = @"14.1";
					CocoaDevice.runtimeVersions = @{
						@"something": @"1.0",
						@"else": @"2.0"
					};
					CocoaDevice.totalMemory = @(100 * 1024 * 1024);

					TSharedRef<IBugsnagDevice> Device = FWrappedDevice::From(CocoaDevice);
					TEST_EQUAL(Device->GetJailbroken().GetValue(), true);
					TEST_EQUAL(Device->GetId().GetValue(), TEXT("uniqueId"));
					TEST_EQUAL(Device->GetLocale().GetValue(), TEXT("en_EN"));
					TEST_EQUAL(Device->GetManufacturer().GetValue(), TEXT("Apple"));
					TEST_EQUAL(Device->GetModel().GetValue(), TEXT("iPhone99,9"));
					TEST_EQUAL(Device->GetModelNumber().GetValue(), TEXT("D79AP"));
					TEST_EQUAL(Device->GetOsName().GetValue(), TEXT("iOS"));
					TEST_EQUAL(Device->GetOsVersion().GetValue(), TEXT("14.1"));
					TSharedPtr<TMap<FString, FString>> RuntimeVersions = Device->GetRuntimeVersions();
					TEST_EQUAL(RuntimeVersions->Num(), 2);
					TEST_EQUAL((*RuntimeVersions)[TEXT("something")], TEXT("1.0"));
					TEST_EQUAL((*RuntimeVersions)[TEXT("else")], TEXT("2.0"));
					TEST_EQUAL(Device->GetTotalMemory().GetValue(), 100 * 1024 * 1024);
				});

			It("Sets values on the Cocoa object", [this]()
				{
					BugsnagDevice* CocoaDevice = [[BugsnagDevice alloc] init];

					TSharedRef<IBugsnagDevice> Device = FWrappedDevice::From(CocoaDevice);
					Device->SetJailbroken(true);
					Device->SetId(FString(TEXT("uniqueId")));
					Device->SetLocale(FString(TEXT("en_EN")));
					Device->SetManufacturer(FString(TEXT("Apple")));
					Device->SetModel(FString(TEXT("iPhone99,9")));
					Device->SetModelNumber(FString(TEXT("D79AP")));
					Device->SetOsName(FString(TEXT("iOS")));
					Device->SetOsVersion(FString(TEXT("14.1")));
					TSharedPtr<TMap<FString, FString>> RuntimeVersions = MakeShareable(new TMap<FString, FString>);
					RuntimeVersions->Add(TEXT("something"), TEXT("1.0"));
					RuntimeVersions->Add(TEXT("else"), TEXT("2.0"));
					Device->SetRuntimeVersions(RuntimeVersions);
					Device->SetTotalMemory(100 * 1024 * 1024);

					TEST_EQUAL(CocoaDevice.jailbroken, YES);
					TEST_EQUAL_OBJC(CocoaDevice.id, @"uniqueId");
					TEST_EQUAL_OBJC(CocoaDevice.locale, @"en_EN");
					TEST_EQUAL_OBJC(CocoaDevice.manufacturer, @"Apple");
					TEST_EQUAL_OBJC(CocoaDevice.model, @"iPhone99,9");
					TEST_EQUAL_OBJC(CocoaDevice.modelNumber, @"D79AP");
					TEST_EQUAL_OBJC(CocoaDevice.osName, @"iOS");
					TEST_EQUAL_OBJC(CocoaDevice.osVersion, @"14.1");
					TEST_EQUAL_OBJC(CocoaDevice.runtimeVersions, (@{@"something": @"1.0", @"else": @"2.0"}));
					TEST_EQUAL_OBJC(CocoaDevice.totalMemory, @(100 * 1024 * 1024));
				});
		});
}
