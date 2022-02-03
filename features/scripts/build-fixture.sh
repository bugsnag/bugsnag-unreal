#!/bin/bash

set -o errexit
set -o nounset

PLATFORM=$1

UPROJECT="${PWD}/features/fixtures/generic/TestFixture.uproject"

UE_VERSION="${UE_VERSION:-4.27}"
UE_HOME="/Users/Shared/Epic Games/UE_${UE_VERSION}"
UE_BUILD="${UE_HOME}/Engine/Build/BatchFiles/Mac/Build.sh"
UE_RUNUAT="${UE_HOME}/Engine/Build/BatchFiles/RunUAT.sh"


echo "--- Installing plugin"

unzip -o "Build/Plugin/Bugsnag-$(cat VERSION)-$(git rev-parse --short=7 HEAD)-UE_${UE_VERSION}-macOS.zip" -d features/fixtures/generic/Plugins


echo "--- Building Editor dependencies"

"${UE_BUILD}" TestFixture Mac Development -TargetType=Editor -ForceUnity "${UPROJECT}"


echo "--- Building test fixture"

RUNUAT_ARGS=(BuildCookRun -project="${UPROJECT}" -targetplatform="${PLATFORM}" -clientconfig=Shipping)
RUNUAT_ARGS+=(-build -nocompileeditor -ubtargs=-ForceUnity)
RUNUAT_ARGS+=(-cook -compressed -pak)
RUNUAT_ARGS+=(-stage -prereqs)
RUNUAT_ARGS+=(-package)

if [ "${PLATFORM}" == "Mac" ]; then
	RUNUAT_ARGS+=(-archive)
fi

"${UE_RUNUAT}" "${RUNUAT_ARGS[@]}" -unattended -utf8output


if [ "${PLATFORM}" == "Mac" ]; then
	echo "--- Archiving test fixture"
	zip -r TestFixture-macOS.zip features/fixtures/generic/ArchivedBuilds
fi
