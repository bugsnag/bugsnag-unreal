UE_VERSION?=4.26
UE_HOME?=/Users/Shared/Epic Games/UE_$(UE_VERSION)
UE_BUILD=$(UE_HOME)/Engine/Build/BatchFiles/Mac/Build.sh
UE_RUNUAT=$(UE_HOME)/Engine/Build/BatchFiles/RunUAT.sh
UE_EDITOR=$(UE_HOME)/Engine/Binaries/Mac/UE4Editor.app/Contents/MacOS/UE4Editor

UPROJECT=$(PWD)/Project.uproject
TESTPROJ=$(PWD)/features/fixtures/mobile/TestFixture.uproject

.PHONY: clean e2e_android e2e_android_local e2e_ios e2e_ios_local editor format package run test

Binaries/Mac/UE4Editor-Project.dylib:
	"$(UE_BUILD)" Project Mac Development -TargetType=Editor "$(UPROJECT)"

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
features/fixtures/mobile/Binaries/Mac/UE4Editor-TestFixture.dylib:
	"$(UE_BUILD)" TestFixture Mac Development -TargetType=Editor "$(TESTPROJ)"

format:
	find Source/ProjectCore Plugins/Bugsnag/Source/Bugsnag features/fixtures/mobile/Source -name '*.h' -o -name '*.cpp' | xargs clang-format -i

editor: Binaries/Mac/UE4Editor-Project.dylib
	"$(UE_EDITOR)" "$(UPROJECT)"

run: Binaries/Mac/UE4Editor-Project.dylib
	"$(UE_EDITOR)" "$(UPROJECT)" -game -windowed -resx=1280 -resy=720

test: Binaries/Mac/UE4Editor-Project.dylib
	"$(UE_EDITOR)" "$(UPROJECT)" -ExecCmds="Automation RunTests Bugsnag; Quit" -NoSplash -NullRHI -ReportOutputPath="$(PWD)/Saved/Automation/Reports"

# https://www.unrealengine.com/en-US/marketplace-guidelines#263b
package:
	"$(UE_RUNUAT)" BuildPlugin -Plugin="$(PWD)/Plugins/Bugsnag/Bugsnag.uplugin" -Package="$(PWD)/Build/Package"

clean:
	find . -type d -name Binaries -or -name Intermediate | xargs rm -rf