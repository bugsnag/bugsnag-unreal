.DELETE_ON_ERROR: # delete generated files if something fails
.SUFFIXES:        # remove default suffix rules
MAKEFLAGS += --no-builtin-rules # skip trying automatic rules (small speedup)

SUBMODULE=deps/bugsnag-cocoa
SRCDIR=$(SUBMODULE)/Bugsnag
OUTDIR=Plugins/Bugsnag/Source/ThirdParty/BugsnagCocoa
HEADERS=$(OUTDIR)/include
IOS_LIB=$(OUTDIR)/IOS/Release/libBugsnagStatic.a
MAC_LIB=$(OUTDIR)/Mac/Release/libBugsnagStatic.a
DERIVED_DATA=Intermediate/Build/DerivedData
XCWORKSPACE=$(SUBMODULE)/Bugsnag.xcworkspace
XCODEBUILD=xcodebuild -workspace $(XCWORKSPACE) -scheme BugsnagStatic -derivedDataPath $(DERIVED_DATA) -configuration Release -quiet build

.PHONY: package
package: $(HEADERS) $(IOS_LIB)

.PHONY: clean
clean:
	rm -rf $(DERIVED_DATA) $(HEADERS) $(IOS_LIB) $(MAC_LIB)

$(HEADERS): $(shell find $(SRCDIR) -type f -name '*.h')
	rsync --delete --recursive --times $(SRCDIR)/include/ $@/
	find $(SRCDIR) -type f -name '*.h' | grep -v $(SRCDIR)/include/ | rsync --delete --files-from - --no-relative --times . $@/BugsnagPrivate/
	touch $@

$(IOS_LIB): $(shell find $(SRCDIR) -type f)
	$(XCODEBUILD) SDKROOT=iphoneos IOS_DEPLOYMENT_TARGET=11.0
	mkdir -p $(@D)
	ln -f $(DERIVED_DATA)/Build/Products/Release-iphoneos/$(@F) $@

$(MAC_LIB): $(shell find $(SRCDIR) -type f)
	$(XCODEBUILD) SDKROOT=macosx MACOSX_DEPLOYMENT_TARGET=10.11
	mkdir -p $(@D)
	ln -f $(DERIVED_DATA)/Build/Products/Release/$(@F) $@
