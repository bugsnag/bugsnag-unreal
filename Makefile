BUGSNAG_COCOA_VERSION?=e049f6192ff3193afe889c0e2b26e12964c57567

UE_VERSION?=4.26
UE_HOME?=/Users/Shared/Epic Games/UE_$(UE_VERSION)
UE_BUILD=$(UE_HOME)/Engine/Build/BatchFiles/Mac/Build.sh
UE_RUNUAT=$(UE_HOME)/Engine/Build/BatchFiles/RunUAT.sh
UE_EDITOR=$(UE_HOME)/Engine/Binaries/Mac/UE4Editor.app/Contents/MacOS/UE4Editor
UE_BUILDCOOK_ARGS=BuildCookRun -nocompileeditor -nop4 -stage -package \
				  -clientconfig=Shipping -clean -compressed -pak -prereqs \
				  -nodebuginfo -build -utf8output -cook

UPROJECT=$(PWD)/BugsnagExample.uproject
TESTPROJ=$(PWD)/features/fixtures/mobile/TestFixture.uproject
TESTSCOPE?=Bugsnag
PLATFORM?=Android

.PHONY: BugsnagCocoa bump clean e2e_android e2e_android_local e2e_ios e2e_ios_local editor format package test

all: Binaries/Mac/UE4Editor-BugsnagExample.dylib

# Bump the version numbers to $VERSION
bump:
ifeq ($(VERSION),)
	$(error VERSION is not defined. Run with `make bump VERSION=number`)
endif
	echo Bumping the version number to $(VERSION)
	echo $(VERSION) > VERSION
	sed -i '' "s/\"VersionName\": .*,/\"VersionName\": \"$(VERSION)\",/" Plugins/Bugsnag/Bugsnag.uplugin
	sed -i '' "s/BUGSNAG_UNREAL_VERSION_STRING .*/BUGSNAG_UNREAL_VERSION_STRING \"$(VERSION)\"/" Plugins/Bugsnag/Source/Bugsnag/Public/Version.h
	sed -i '' "s/## TBD/## $(VERSION) ($(shell date '+%Y-%m-%d'))/" CHANGELOG.md

clean:
	find . -type d -name Binaries -or -name Intermediate | xargs rm -rf
	git clean -dfx Plugins/Bugsnag/Source/ThirdParty/BugsnagCocoa
	rm -rf Build deps features/fixtures/mobile/Plugins/Bugsnag

# Convenience target that ensures editor modules are up to date and opens the example project in Unreal Editor.
editor: Binaries/Mac/UE4Editor-BugsnagExample.dylib
	"$(UE_EDITOR)" "$(UPROJECT)"

format:
	find Source Plugins/Bugsnag/Source/Bugsnag features/fixtures/mobile/Source -name '*.h' -o -name '*.cpp' | xargs clang-format -i

lint:
	find Source Plugins/Bugsnag/Source/Bugsnag features/fixtures/mobile/Source -name '*.h' -o -name '*.cpp' | xargs clang-format --dry-run -Werror
	cspell cspell **/*.{cpp,h}

# If this target isn't built beforehand, the Editor will show the "Missing BugsnagExample Modules" prompt
Binaries/Mac/UE4Editor-BugsnagExample.dylib: BugsnagCocoa
	"$(UE_BUILD)" BugsnagExample Mac Development -TargetType=Editor "$(UPROJECT)"

# To run a subset of tests, do make e2e_android TESTS=features/my_test.feature
e2e_android: features/fixtures/mobile/Binaries/Android/TestFixture-Android-Shipping-arm64.apk
	bundle exec maze-runner --app=$< --farm=bs --device=ANDROID_10_0 $(TESTS)

e2e_android_local: features/fixtures/mobile/Binaries/Android/TestFixture-Android-Shipping-arm64.apk
	bundle exec maze-runner --app=$< --farm=local --os=android --os-version=10 $(TESTS)

e2e_ios: features/fixtures/mobile/Binaries/IOS/TestFixture-IOS-Shipping.ipa
	bundle exec maze-runner --app=$< --farm=bs --device=IOS_12 $(TESTS)

e2e_ios_local: features/fixtures/mobile/Binaries/IOS/TestFixture-IOS-Shipping.ipa
	ideviceinstaller --uninstall com.bugsnag.TestFixture
	bundle exec maze-runner --app=$< --farm=local --os=ios --os-version=14 --apple-team-id=372ZUL2ZB7 --udid="$(shell idevice_id -l)" $(TESTS)

features/fixtures/mobile/Binaries/Android/TestFixture-Android-Shipping-arm64.apk: features/fixtures/mobile/Binaries/Mac/UE4Editor-TestFixture.dylib
	"$(UE_RUNUAT)" $(UE_BUILDCOOK_ARGS) -project="$(TESTPROJ)" -targetplatform=Android

features/fixtures/mobile/Binaries/IOS/TestFixture-IOS-Shipping.ipa: features/fixtures/mobile/Binaries/Mac/UE4Editor-TestFixture.dylib
	"$(UE_RUNUAT)" $(UE_BUILDCOOK_ARGS) -project="$(TESTPROJ)" -targetplatform=IOS

# UE4Editor-TestFixture.dylib is required for BuildCookRun to succeed
features/fixtures/mobile/Binaries/Mac/UE4Editor-TestFixture.dylib: BugsnagCocoa
	rsync --exclude 'Binaries' --exclude 'Intermediate' --delete --recursive --times Plugins/Bugsnag features/fixtures/mobile/Plugins
	"$(UE_BUILD)" TestFixture Mac Development -TargetType=Editor "$(TESTPROJ)"

test: Binaries/Mac/UE4Editor-BugsnagExample.dylib
	"$(UE_EDITOR)" "$(UPROJECT)" -ExecCmds="Automation RunTests $(TESTSCOPE); Quit" -NoSplash -NullRHI -ReportOutputPath="$(PWD)/Saved/Automation/Reports"

# https://www.unrealengine.com/en-US/marketplace-guidelines#263b
package: BugsnagCocoa
	"$(UE_RUNUAT)" BuildPlugin -Plugin="$(PWD)/Plugins/Bugsnag/Bugsnag.uplugin" -Package="$(PWD)/Build/Package"

BugsnagCocoa: Plugins/Bugsnag/Source/ThirdParty/BugsnagCocoa/include Plugins/Bugsnag/Source/ThirdParty/BugsnagCocoa/IOS/Release/libBugsnagStatic.a Plugins/Bugsnag/Source/ThirdParty/BugsnagCocoa/Mac/Release/libBugsnagStatic.a

deps/bugsnag-cocoa:
	git init --quiet $@ && cd $@ && git remote add origin https://github.com/bugsnag/bugsnag-cocoa && git fetch --no-tags --depth=1 origin $(BUGSNAG_COCOA_VERSION) && git checkout --quiet -f $(BUGSNAG_COCOA_VERSION)

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

.PHONY: build_example_android
build_example_android:
	"$(UE_RUNUAT)" $(UE_BUILDCOOK_ARGS) -project="$(UPROJECT)" -targetplatform=Android

.PHONY: install_example_android
install_example_android: build_example_android
	adb install Binaries/Android/BugsnagExample-Android-Shipping-arm64.apk

.PHONY: build_example_ios
build_example_ios:
	"$(UE_RUNUAT)" $(UE_BUILDCOOK_ARGS) -project="$(UPROJECT)" -targetplatform=IOS

# Note: ideviceinstaller does not make the app visible on the home screen :-/
.PHONY: install_example_ios
install_example_ios: build_example_ios
	ideviceinstaller --install Binaries/IOS/BugsnagExample-IOS-Shipping.ipa --udid="$(shell idevice_id -l)"

.PHONY: build
ifeq ($(PLATFORM),Android)
build: build_example_android
endif
