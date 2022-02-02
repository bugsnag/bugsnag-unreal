#!/bin/bash

set -o errexit
set -o nounset

UPROJECT="${PWD}/features/fixtures/generic/TestFixture.uproject"

UE_HOME="/Users/Shared/Epic Games/UE_${UE_VERSION:-4.27}"
UE_BUILD="${UE_HOME}/Engine/Build/BatchFiles/Mac/Build.sh"
UE_RUNUAT="${UE_HOME}/Engine/Build/BatchFiles/RunUAT.sh"

echo "--- Installing plugin"

unzip -o Build/Plugin/Bugsnag-*-macOS.zip -d features/fixtures/generic/Plugins

echo "--- Building Editor dependencies"

"${UE_BUILD}" TestFixture Mac Development -TargetType=Editor -ForceUnity "${UPROJECT}"

echo "--- Building test fixture"

"${UE_RUNUAT}" BuildCookRun -targetplatform=Mac -clientconfig=Shipping -project="${UPROJECT}" -build -cook -stage -package -archive -unattended -nocompileeditor -nop4 -compressed -pak -prereqs -utf8output -ubtargs=-ForceUnity

echo "--- Archiving test fixture"

zip -r TestFixture-macOS.zip features/fixtures/generic/ArchivedBuilds
