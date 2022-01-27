.DELETE_ON_ERROR: # delete generated files if something fails
.SUFFIXES:        # remove default suffix rules
MAKEFLAGS += --no-builtin-rules # skip trying automatic rules (small speedup)

UE_VERSION?=4.27
UE_HOME?=/Users/Shared/Epic Games/UE_$(UE_VERSION)
UE_BUILD=$(UE_HOME)/Engine/Build/BatchFiles/Mac/Build.sh
UE_RUNUAT=$(UE_HOME)/Engine/Build/BatchFiles/RunUAT.sh
UE_EDITOR=$(UE_HOME)/Engine/Binaries/Mac/UE4Editor.app/Contents/MacOS/UE4Editor
UE_BUILDCOOK_ARGS=BuildCookRun -nocompileeditor -nop4 -stage -package \
				  -clientconfig=Shipping -compressed -pak -prereqs \
				  -nodebuginfo -build -utf8output -cook

UPROJECT=$(PWD)/BugsnagExample.uproject
EXAMPLE_MAC_LIB=Binaries/Mac/UE4Editor-BugsnagExample.dylib
EXAMPLE_IOS_APP=Binaries/IOS/BugsnagExample-IOS-Shipping.ipa

TESTPROJ=$(PWD)/features/fixtures/mobile/TestFixture.uproject
TEST_OUTDIR=features/fixtures/mobile/Binaries
TEST_ANDROID_APP=$(TEST_OUTDIR)/Android/TestFixture-Android-Shipping-arm64.apk
TEST_IOS_APP=$(TEST_OUTDIR)/IOS/TestFixture-IOS-Shipping.ipa
TEST_MAC_LIB=$(TEST_OUTDIR)/Mac/UE4Editor-TestFixture.dylib

GIT_COMMIT=$(shell git rev-parse --short=7 HEAD)
PLUGIN_PACKAGE=$(PWD)/Build/Plugin/Bugsnag
UPLUGIN=$(PWD)/Plugins/Bugsnag/Bugsnag.uplugin
PRESET_VERSION=$(shell cat VERSION)
ZIP_NAME=Bugsnag-$(PRESET_VERSION)-$(GIT_COMMIT)-UE_$(UE_VERSION)

# Change to run specific tests files
TESTSCOPE?=Bugsnag
# Default platform for commands - supported values: iOS, Android
PLATFORM?=

all: $(EXAMPLE_MAC_LIB)

.PHONY: test
test: $(EXAMPLE_MAC_LIB) ## run unit tests
ifeq ($(PLATFORM),Android)
	$(MAKE) -f make/Android.make test
else
	"$(UE_EDITOR)" "$(UPROJECT)" \
		-ExecCmds="Automation RunTests $(TESTSCOPE); Quit" -NoSplash \
		-NullRHI -ReportOutputPath="$(PWD)/Saved/Automation/Reports"
endif

# https://www.unrealengine.com/en-US/marketplace-guidelines#263b
.PHONY: package
package: ## Build plugin for release or testing
	$(MAKE) -f make/Android.make package
	$(MAKE) -f make/Cocoa.make package
	"$(UE_RUNUAT)" BuildPlugin -Plugin="$(UPLUGIN)" -Package="$(PLUGIN_PACKAGE)" -Rocket
	cd "$(PLUGIN_PACKAGE)/.." && zip -r "$(ZIP_NAME)-macOS.zip" Bugsnag
# Binaries, Build and Intermediate folders should not be included in the Marketplace submission.
# https://marketplacehelp.epicgames.com/s/article/Marketplace-Plugin-Guide
# https://www.unrealengine.com/en-US/marketplace-guidelines#273
	cd "$(PLUGIN_PACKAGE)/.." && zip -r "$(ZIP_NAME)-src.zip" Bugsnag/Bugsnag.uplugin Bugsnag/Config Bugsnag/Resources Bugsnag/Source

.PHONY: clean
clean: ## remove build artifacts
	$(MAKE) -f make/Cocoa.make clean
	$(MAKE) -f make/Android.make clean
	find . -type d -name Binaries -or -name Intermediate | xargs rm -rf
	rm -rf Build features/fixtures/mobile/Plugins/Bugsnag

.PHONY: editor
editor: $(EXAMPLE_MAC_LIB) ## Build the project and open in Unreal Editor
	"$(UE_EDITOR)" "$(UPROJECT)"

.PHONY: format
format: ## format all c/c++ source to match .clang-format
	find Source Plugins/Bugsnag/Source/Bugsnag features/fixtures/mobile/Source -name '*.h' -o -name '*.cpp' | xargs clang-format -i

.PHONY: lint
lint: ## check the project for formatting or spelling issues
	find Source Plugins/Bugsnag/Source/Bugsnag features/fixtures/mobile/Source -name '*.h' -o -name '*.cpp' | xargs clang-format --dry-run --Werror
	cspell **/*.{cpp,h}

.PHONY: e2e
ifeq ($(PLATFORM),iOS)
e2e: e2e_ios_local ## Run end-to-end tests locally on $PLATFORM
endif
ifeq ($(PLATFORM),Android)
e2e: e2e_android_local
endif

.PHONY: e2e_android
# To run a subset of tests, do make e2e_android TESTS=features/my_test.feature
e2e_android: $(TEST_ANDROID_APP)
	bundle exec maze-runner --app=$< --farm=bs --device=ANDROID_10_0 $(TESTS)

.PHONY: e2e_android_local
e2e_android_local: $(TEST_ANDROID_APP)
	./$(TEST_OUTDIR)/Android/Install_TestFixture-Android-Shipping-arm64.command
	bundle exec maze-runner --app=$< --farm=local --os=android --os-version=10 $(TESTS) --color

.PHONY: e2e_ios
e2e_ios: $(TEST_IOS_APP)
	bundle exec maze-runner --app=$< --farm=bs --device=IOS_12 $(TESTS)

.PHONY: e2e_ios_local
e2e_ios_local: $(TEST_IOS_APP)
	ideviceinstaller --uninstall com.bugsnag.TestFixture
	bundle exec maze-runner --app=$< --farm=local --os=ios --os-version=14 --apple-team-id=372ZUL2ZB7 --udid="$(shell idevice_id -l)" $(TESTS) --color

.PHONY: build_example_android
build_example_android: $(EXAMPLE_MAC_LIB)
	"$(UE_RUNUAT)" $(UE_BUILDCOOK_ARGS) -project="$(UPROJECT)" -targetplatform=Android

.PHONY: install_example_android
install_example_android: build_example_android
	./Binaries/Android/Install_TestFixture-Android-Shipping-arm64.command

.PHONY: build_example_ios
build_example_ios: $(EXAMPLE_MAC_LIB)
	"$(UE_RUNUAT)" $(UE_BUILDCOOK_ARGS) -project="$(UPROJECT)" -targetplatform=IOS

# Note: ideviceinstaller does not make the app visible on the home screen :-/
.PHONY: install_example_ios
install_example_ios: build_example_ios
	ideviceinstaller --install $(EXAMPLE_IOS_APP) --udid="$(shell idevice_id -l)"

.PHONY: build
ifeq ($(PLATFORM),iOS)
build: build_example_ios ## build example app for $PLATFORM
endif
ifeq ($(PLATFORM),Android)
build: build_example_android
endif

# UE4Editor-BugsnagExample.dylib is required for BuildCookRun to succeed
# If this target isn't built beforehand, the Editor will show the "Missing
# BugsnagExample Modules" prompt
$(EXAMPLE_MAC_LIB): $(shell find Plugins/Bugsnag/Source Source -type f)
	$(MAKE) -f make/Cocoa.make package
	"$(UE_BUILD)" BugsnagExample Mac Development -TargetType=Editor "$(UPROJECT)"

$(TEST_ANDROID_APP): $(TEST_MAC_LIB)
	"$(UE_RUNUAT)" $(UE_BUILDCOOK_ARGS) -project="$(TESTPROJ)" -targetplatform=Android

$(TEST_IOS_APP): $(TEST_MAC_LIB)
	"$(UE_RUNUAT)" $(UE_BUILDCOOK_ARGS) -project="$(TESTPROJ)" -targetplatform=IOS

.PHONY: copy_package_to_fixture
copy_package_to_fixture: package
	rsync --exclude 'Binaries' --exclude 'Intermediate' --delete --recursive --times Plugins/Bugsnag features/fixtures/mobile/Plugins

# UE4Editor-TestFixture.dylib is required for BuildCookRun to succeed
# Treating this target as PHONY since it does not verify when to rebuild from
# sources.
.PHONY: $(TEST_MAC_LIB)
$(TEST_MAC_LIB): copy_package_to_fixture
	"$(UE_BUILD)" TestFixture Mac Development -TargetType=Editor "$(TESTPROJ)"

#--------------------------------------------------------------------------
# Release
#--------------------------------------------------------------------------

.PHONY: bump
bump: ## Bump the version numbers to $VERSION
ifeq ($(VERSION),)
	$(error VERSION is not defined. Run with `make bump VERSION=number`)
endif
	echo $(VERSION) > VERSION
	ruby -rjson -e 'f="$(UPLUGIN)"; v="$(VERSION)"; k="VersionName"; j=JSON.parse(File.read(f)); if j[k] != v then j[k]=v; j["Version"]+=1; File.write(f, JSON.pretty_generate(j).gsub("  ", "	") + "\n") end'
	sed -i '' "s/BUGSNAG_UNREAL_VERSION_STRING .*/BUGSNAG_UNREAL_VERSION_STRING \"$(VERSION)\"/" Plugins/Bugsnag/Source/Bugsnag/Private/Version.h
	sed -i '' "s/## TBD/## $(VERSION) ($(shell date '+%Y-%m-%d'))/" CHANGELOG.md
	$(MAKE) -f make/Android.make bump

.PHONY: prerelease
prerelease: bump ## Generates a PR for the $VERSION release
ifeq ($(VERSION),)
	$(error VERSION is not defined. Run with `make prerelease VERSION=number`)
endif
	git checkout -b release-v$(VERSION)
	git add CHANGELOG.md Makefile Plugins/Bugsnag/Bugsnag.uplugin Plugins/Bugsnag/Source/Bugsnag/Bugsnag_UPL.xml Plugins/Bugsnag/Source/Bugsnag/Private/Version.h VERSION deps/bugsnag-plugin-android-unreal/build.gradle
	git diff --exit-code || (echo "you have unstaged changes - Makefile may need updating to `git add` some more files"; exit 1)
	git commit -m "Release v$(VERSION)"
	git push origin release-v$(VERSION)
	open "https://github.com/bugsnag/bugsnag-unreal/compare/main...release-v$(VERSION)?expand=1&title=Release%20v$(VERSION)"

.PHONY: release
release: ## Releases the current main branch as $VERSION
	git fetch origin
ifneq ($(shell git rev-parse --abbrev-ref HEAD),main) # Check the current branch name
	git checkout main
	git rebase origin/main
endif
ifneq ($(shell git diff origin/main..main),)
	$(error you have unpushed commits on the main branch)
endif
	git tag v$(PRESET_VERSION)
	git push origin v$(PRESET_VERSION)
	git checkout next
	git rebase origin/next
	git merge main
	git push origin next
	open "https://github.com/bugsnag/bugsnag-unreal/releases/new?title=v$(PRESET_VERSION)&tag=v$(PRESET_VERSION)"

#--------------------------------------------------------------------------
# Miscellaneous
#--------------------------------------------------------------------------

.PHONY: help
help: ## Show help text
	@grep -E '^[a-zA-Z0-9_-]+:.*?## .*$$' $(MAKEFILE_LIST) \
		| sort \
		| awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'
