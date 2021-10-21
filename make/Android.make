.DELETE_ON_ERROR: # delete generated files if something fails
.SUFFIXES:        # remove default suffix rules
MAKEFLAGS += --no-builtin-rules # skip trying automatic rules (small speedup)

PACKAGE_VERSION=$(shell cat VERSION)
PACKAGE_SOURCES=$(wildcard deps/bugsnag-plugin-android-unreal/src/main/java/com/bugsnag/android/unreal/*.java)
PACKAGE_OUTPUT=Plugins/Bugsnag/Source/ThirdParty/Android/lib/com/bugsnag/bugsnag-plugin-android-unreal/$(PACKAGE_VERSION)/bugsnag-plugin-android-unreal-$(PACKAGE_VERSION).aar
PACKAGE_INPUT=deps/bugsnag-plugin-android-unreal/build/outputs/aar/bugsnag-plugin-android-unreal-release.aar
POM_OUTPUT=$(PACKAGE_OUTPUT:.aar=.pom)
POM_INPUT=$(PACKAGE_INPUT:.aar=.pom)
GRADLE=./gradlew --quiet

all: package

.PHONY: package
package: $(PACKAGE_OUTPUT) $(POM_OUTPUT) ## Generate Android package files

.PHONY: test
test: ## Run unit tests (requires a device)
	@cd deps/bugsnag-plugin-android-unreal && \
		$(GRADLE) connectedAndroidTest

.PHONY: bump
bump: ## Update the library version
ifeq ($(VERSION),)
	$(error VERSION is not defined. Run with `make bump VERSION=number`)
endif
	sed -i '' "s/version '.*'/version '$(VERSION)'/" deps/bugsnag-plugin-android-unreal/build.gradle
	sed -i '' "s/com\.bugsnag,bugsnag-plugin-android-unreal,.*/com.bugsnag,bugsnag-plugin-android-unreal,$(VERSION)/" Plugins/Bugsnag/Source/Bugsnag/Bugsnag_UPL.xml

$(PACKAGE_OUTPUT): $(PACKAGE_INPUT)
	mkdir -p $(@D)
	cp -a "$<" "$@"

$(POM_OUTPUT): $(POM_INPUT)
	mkdir -p $(@D)
	cp -a "$<" "$@"

$(PACKAGE_INPUT): $(PACKAGE_SOURCES)
	cd deps/bugsnag-plugin-android-unreal && \
		$(GRADLE) assembleRelease

$(POM_INPUT):
	cd deps/bugsnag-plugin-android-unreal && \
		$(GRADLE) generatePom

.PHONY: help
help: ## Show help text
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'
