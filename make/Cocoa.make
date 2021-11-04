.DELETE_ON_ERROR: # delete generated files if something fails
.SUFFIXES:        # remove default suffix rules
MAKEFLAGS += --no-builtin-rules # skip trying automatic rules (small speedup)

BUGSNAG_COCOA_VERSION?=a0e2c1922c4c45bd7a2ed30c1ffa5194f1cf83ac
BUGSNAG_COCOA_SRC=deps/bugsnag-cocoa
BUGSNAG_COCOA_SENTINEL=$(BUGSNAG_COCOA_SRC)/VERSION

OUTDIR=Plugins/Bugsnag/Source/ThirdParty/BugsnagCocoa
BUGSNAG_COCOA_INCLUDES_DEST=$(OUTDIR)/include
BUGSNAG_COCOA_INCLUDES_SENTINEL=$(BUGSNAG_COCOA_INCLUDES_DEST)/Bugsnag/Bugsnag.h
IOS_LIB=$(OUTDIR)/IOS/Release/libBugsnagStatic.a
MAC_LIB=$(OUTDIR)/Mac/Release/libBugsnagStatic.a
DERIVED_DATA=DerivedData
IOS_LIB_SRC=$(BUGSNAG_COCOA_SRC)/$(DERIVED_DATA)/Build/Products/Release-iphoneos/libBugsnagStatic.a
MAC_LIB_SRC=$(BUGSNAG_COCOA_SRC)/$(DERIVED_DATA)/Build/Products/Release/libBugsnagStatic.a

.PHONY: package
package: $(BUGSNAG_COCOA_INCLUDES_SENTINEL) $(IOS_LIB) $(MAC_LIB)

.PHONY: clean
clean:
	@rm -rf $(BUGSNAG_COCOA_SRC)/$(DERIVED_DATA) $(OUTDIR)

# Ensure the correct version of the library is checked out
.PHONY: bugsnag_cocoa
bugsnag_cocoa: $(BUGSNAG_COCOA_SENTINEL)
ifneq ($(BUGSNAG_COCOA_VERSION),$(shell cat $(BUGSNAG_COCOA_SRC)/.git/HEAD | tr -d "\n"))
	cd $(BUGSNAG_COCOA_SRC) \
		&& git fetch --no-tags --depth=1 origin $(BUGSNAG_COCOA_VERSION) \
		&& git checkout --quiet -f $(BUGSNAG_COCOA_VERSION)
endif

$(BUGSNAG_COCOA_INCLUDES_SENTINEL): bugsnag_cocoa
	cp -pR $(BUGSNAG_COCOA_SRC)/Bugsnag/include/ $(BUGSNAG_COCOA_INCLUDES_DEST)/
	mkdir -p $(BUGSNAG_COCOA_INCLUDES_DEST)/BugsnagPrivate
	cd $(BUGSNAG_COCOA_SRC) \
		&& find Bugsnag \( -name '*.h' ! -path 'Bugsnag/include/*' \) \
			-exec cp -a {} $(PWD)/$(BUGSNAG_COCOA_INCLUDES_DEST)/BugsnagPrivate \;

$(BUGSNAG_COCOA_SENTINEL):
	git clone --quiet --no-checkout https://github.com/bugsnag/bugsnag-cocoa $(@D)

$(IOS_LIB): $(IOS_LIB_SRC)
	mkdir -p $(@D)
	cp -a $< $@

$(MAC_LIB): $(MAC_LIB_SRC)
	mkdir -p $(@D)
	cp -a $< $@

$(IOS_LIB_SRC): bugsnag_cocoa
	cd $(BUGSNAG_COCOA_SRC) && xcodebuild -scheme BugsnagStatic \
		-derivedDataPath $(DERIVED_DATA) -configuration Release -quiet build \
		SDKROOT=iphoneos IOS_DEPLOYMENT_TARGET=11.0

$(MAC_LIB_SRC):  bugsnag_cocoa
	cd $(BUGSNAG_COCOA_SRC) && xcodebuild -scheme BugsnagStatic \
		-derivedDataPath $(DERIVED_DATA) -configuration Release -quiet build \
		SDKROOT=macosx MACOSX_DEPLOYMENT_TARGET=10.11
