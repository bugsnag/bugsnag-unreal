#!/bin/bash

set -o errexit
set -o nounset
set -x

PLATFORM=$1

UPROJECT="${PWD}/features/fixtures/generic/TestFixture.uproject"

XCODE_EXPORT_OPTIONS="${PWD}/features/scripts/exportOptions.plist"

UE_VERSION="${UE_VERSION:-4.27}"
UE_HOME="/Users/Shared/Epic Games/UE_${UE_VERSION}"
UE_BUILD="${UE_HOME}/Engine/Build/BatchFiles/Mac/Build.sh"
UE_RUNUAT="${UE_HOME}/Engine/Build/BatchFiles/RunUAT.sh"
MODERN_IOS=false

if [[ "${UE_VERSION}" == "5.4" && "${PLATFORM}" == "IOS" ]]; then
  echo "--- Using iOS modern xcode setup"
  MODERN_IOS=true
fi

if [[ "$MODERN_IOS" == true ]]; then
  echo "--- Deleting Xcode Archives directory"
  rm -rf ~/Library/Developer/Xcode/Archives
fi

echo "--- Installing plugin"

unzip -o "Build/Plugin/Bugsnag-$(cat VERSION)-$(git rev-parse --short=7 HEAD)-UE_${UE_VERSION}-macOS.zip" -d features/fixtures/generic/Plugins

case "${UE_VERSION}" in
  5.3)
    if [[ "$PLATFORM" == "Mac" ]]; then
      echo "--- Enabling Modern Xcode Build"
      sed -i '' 's/bUseModernXcode=False/bUseModernXcode=True/' features/fixtures/generic/Config/DefaultEngine.ini
    fi
  ;;
esac

if [[ "$MODERN_IOS" == true ]]; then
  echo "--- Enabling Modern Xcode Build"
  sed -i '' 's/bUseModernXcode=False/bUseModernXcode=True/' features/fixtures/generic/Config/DefaultEngine.ini
fi

echo "--- Building Editor dependencies"

"${UE_BUILD}" TestFixture Mac Development -TargetType=Editor -ForceUnity "${UPROJECT}"


echo "--- Building test fixture"

RUNUAT_ARGS=(BuildCookRun -project="${UPROJECT}" -targetplatform="${PLATFORM}" -clientconfig=Shipping)
RUNUAT_ARGS+=(-build -nocompileeditor -ubtargs=-ForceUnity)
RUNUAT_ARGS+=(-cook -compressed -pak)
RUNUAT_ARGS+=(-stage -prereqs)
RUNUAT_ARGS+=(-package)

case "${PLATFORM}" in
  Android)  RUNUAT_ARGS+=(-distribution) ;;
  Mac)      RUNUAT_ARGS+=(-archive) ;;
esac

if [[ "$MODERN_IOS" == true ]]; then
  RUNUAT_ARGS+=(-distribution)
fi

"${UE_RUNUAT}" "${RUNUAT_ARGS[@]}" -unattended -utf8output


echo "--- Preparing artifact(s) for upload"

case "${PLATFORM}" in
  Android)
    mv features/fixtures/generic/Binaries/Android/TestFixture-Android-Shipping-arm64.apk build/TestFixture-Android-Shipping-"${UE_VERSION}"-arm64.apk
    if [[ -f features/fixtures/generic/Binaries/Android/TestFixture-Android-Shipping-armv7.apk ]]; then
      mv features/fixtures/generic/Binaries/Android/TestFixture-Android-Shipping-armv7.apk build/TestFixture-Android-Shipping-"${UE_VERSION}"-armv7.apk
    fi
    ;;

  IOS)
    if [[ "$MODERN_IOS" == true ]]; then
      echo "--- Building ipa and dsym files after modern xcode build"
      echo "--- Finding the xcarchive file"
      ARCHIVE_PATH=$(find ~/Library/Developer/Xcode/Archives -type d -name "*.xcarchive" -print -quit)

    if [[ -n "$ARCHIVE_PATH" ]]; then
      echo "--- Found xcarchive at $ARCHIVE_PATH"
      echo "--- Building IPA from xcarchive"
      xcodebuild -exportArchive -archivePath "$ARCHIVE_PATH" \
        -exportPath build/TestFixture-IOS-Shipping-"${UE_VERSION}" \
        -exportOptionsPlist "$XCODE_EXPORT_OPTIONS"

      echo "xcode finished building"

      echo "--- IPA built successfully"

      mv build/TestFixture-IOS-Shipping-"${UE_VERSION}"/TestFixture-IOS-Shipping.ipa build/TestFixture-IOS-Shipping-"${UE_VERSION}".ipa

       # Check and move the dSYM file
      DSYM_PATH="${ARCHIVE_PATH}/dSYMs/TestFixture-IOS-Shipping.app.dSYM"
      if [[ -d "$DSYM_PATH" ]]; then
        echo "--- Found dSYM at $DSYM_PATH"
        mv "$DSYM_PATH" build/TestFixture-IOS-Shipping-"${UE_VERSION}".dSYM
        echo "--- Zipping dSYM file"
        cd build/
        zip -r ../TestFixture-IOS-Shipping-"${UE_VERSION}".dSYM.zip TestFixture-IOS-Shipping-"${UE_VERSION}".dSYM
        cd ../
      else
        echo "Error: dSYM file not found."
        exit 1
      fi
    else
      echo "Error: No xcarchive found."
      exit 1
    fi
  else
    mv features/fixtures/generic/Binaries/IOS/TestFixture-IOS-Shipping.dSYM build/TestFixture-IOS-Shipping-"${UE_VERSION}".dSYM
    mv features/fixtures/generic/Binaries/IOS/TestFixture-IOS-Shipping.ipa build/TestFixture-IOS-Shipping-"${UE_VERSION}".ipa
  fi
  ;;

  Mac)
    case "${UE_VERSION}" in
      4.23)
        mv features/fixtures/generic/ArchivedBuilds/MacNoEditor/TestFixture.app features/fixtures/generic/ArchivedBuilds/MacNoEditor/TestFixture-Mac-Shipping.app
        mv features/fixtures/generic/ArchivedBuilds/MacNoEditor/TestFixture-Mac-Shipping.app/Contents/MacOS/TestFixture features/fixtures/generic/ArchivedBuilds/MacNoEditor/TestFixture-Mac-Shipping.app/Contents/MacOS/TestFixture-Mac-Shipping
        ;;
      5.*)
        mv features/fixtures/generic/ArchivedBuilds/Mac/ features/fixtures/generic/ArchivedBuilds/MacNoEditor/
        ;;
    esac
    zip -r TestFixture-macOS-"${UE_VERSION}".zip features/fixtures/generic/ArchivedBuilds
    ;;
esac
