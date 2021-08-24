BUGSNAG_COCOA_VERSION?=v6.10.4

UE_VERSION?=4.26
UE_HOME?=/Users/Shared/Epic Games/UE_$(UE_VERSION)
UE_BUILD=$(UE_HOME)/Engine/Build/BatchFiles/Mac/Build.sh
UE_RUNUAT=$(UE_HOME)/Engine/Build/BatchFiles/RunUAT.sh
UE_EDITOR=$(UE_HOME)/Engine/Binaries/Mac/UE4Editor.app/Contents/MacOS/UE4Editor

UPROJECT=$(PWD)/BugsnagExample.uproject
TESTPROJ=$(PWD)/features/fixtures/mobile/TestFixture.uproject

.PHONY: BugsnagCocoa clean e2e_android e2e_android_local e2e_ios e2e_ios_local format package test

all: package

clean:
	find . -type d -name Binaries -or -name Intermediate | xargs rm -rf
	git clean -dfx Plugins/Bugsnag/Source/ThirdParty/BugsnagCocoa
	rm -rf Build deps

# TODO: Prevent this from touching files that need no changes, to avoid unnecessary recompiling
format:
	find Source Plugins/Bugsnag/Source/Bugsnag features/fixtures/mobile/Source -name '*.h' -o -name '*.cpp' | xargs clang-format -i

Binaries/Mac/UE4Editor-BugsnagExample.dylib: BugsnagCocoa
	"$(UE_BUILD)" BugsnagExample Mac Development -TargetType=Editor "$(UPROJECT)"

e2e_android: features/fixtures/mobile/Binaries/Android/TestFixture-Android-Shipping-arm64.apk
	bundle exec maze-runner --app=$< --farm=bs --device=ANDROID_10_0

e2e_android_local: features/fixtures/mobile/Binaries/Android/TestFixture-Android-Shipping-arm64.apk
	bundle exec maze-runner --app=$< --farm=local --os=android --os-version=10

e2e_ios: features/fixtures/mobile/Binaries/IOS/TestFixture-IOS-Shipping.ipa
	bundle exec maze-runner --app=$< --farm=bs --device=IOS_12

e2e_ios_local: features/fixtures/mobile/Binaries/IOS/TestFixture-IOS-Shipping.ipa
	bundle exec maze-runner --app=$< --farm=local --os=ios --os-version=14 --apple-team-id=372ZUL2ZB7 --udid="$(shell idevice_id -l)"

features/fixtures/mobile/Binaries/Android/TestFixture-Android-Shipping-arm64.apk: features/fixtures/mobile/Binaries/Mac/UE4Editor-TestFixture.dylib
	"$(UE_RUNUAT)" BuildCookRun -nocompileeditor -nop4 -project="$(TESTPROJ)" -cook -stage -package -clientconfig=Shipping -clean -compressed -pak -prereqs -nodebuginfo -targetplatform=Android -build -utf8output

features/fixtures/mobile/Binaries/IOS/TestFixture-IOS-Shipping.ipa: features/fixtures/mobile/Binaries/Mac/UE4Editor-TestFixture.dylib
	"$(UE_RUNUAT)" BuildCookRun -nocompileeditor -nop4 -project="$(TESTPROJ)" -cook -stage -package -clientconfig=Shipping -clean -compressed -pak -prereqs -nodebuginfo -targetplatform=IOS -build -utf8output

# UE4Editor-TestFixture.dylib is required for BuildCookRun to succeed
features/fixtures/mobile/Binaries/Mac/UE4Editor-TestFixture.dylib: BugsnagCocoa
	"$(UE_BUILD)" TestFixture Mac Development -TargetType=Editor "$(TESTPROJ)"

test: Binaries/Mac/UE4Editor-BugsnagExample.dylib
	"$(UE_EDITOR)" "$(UPROJECT)" -ExecCmds="Automation RunTests Bugsnag; Quit" -NoSplash -NullRHI -ReportOutputPath="$(PWD)/Saved/Automation/Reports"

# https://www.unrealengine.com/en-US/marketplace-guidelines#263b
package: BugsnagCocoa
	"$(UE_RUNUAT)" BuildPlugin -Plugin="$(PWD)/Plugins/Bugsnag/Bugsnag.uplugin" -Package="$(PWD)/Build/Package"

BugsnagCocoa: Plugins/Bugsnag/Source/ThirdParty/BugsnagCocoa/include Plugins/Bugsnag/Source/ThirdParty/BugsnagCocoa/IOS/Release/libBugsnagStatic.a Plugins/Bugsnag/Source/ThirdParty/BugsnagCocoa/Mac/Release/libBugsnagStatic.a

deps/bugsnag-cocoa:
	git clone --depth 1 --quiet --branch $(BUGSNAG_COCOA_VERSION) https://github.com/bugsnag/bugsnag-cocoa $@

Plugins/Bugsnag/Source/ThirdParty/BugsnagCocoa/include: deps/bugsnag-cocoa
	cp -R $</Bugsnag/include $@
	mkdir -p $@/BugsnagPrivate
	cd $< && find Bugsnag \( -name '*.h' ! -path 'Bugsnag/include/*' \) -exec cp {} $(PWD)/$@/BugsnagPrivate \;

Plugins/Bugsnag/Source/ThirdParty/BugsnagCocoa/IOS/Release/libBugsnagStatic.a: deps/bugsnag-cocoa
	cd $< && xcodebuild -scheme BugsnagStatic -derivedDataPath DerivedData -configuration Release -quiet build SDKROOT=iphoneos IOS_DEPLOYMENT_TARGET=11.0
	mkdir -p $(@D)
	cp $</DerivedData/Build/Products/Release-iphoneos/libBugsnagStatic.a $@

Plugins/Bugsnag/Source/ThirdParty/BugsnagCocoa/Mac/Release/libBugsnagStatic.a: deps/bugsnag-cocoa
	cd $< && xcodebuild -scheme BugsnagStatic -derivedDataPath DerivedData -configuration Release -quiet build SDKROOT=macosx MACOSX_DEPLOYMENT_TARGET=10.11
	mkdir -p $(@D)
	cp $</DerivedData/Build/Products/Release/libBugsnagStatic.a $@
