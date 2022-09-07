#!/usr/bin/env bash

set -Eeuo pipefail

UE_VERSION="${UE_VERSION:-4.27}"

echo 1
UE_HOME="C:/Program\ Files/Epic Games/UE_$UE_VERSION"

echo 2
UE_RUNUAT="$UE_HOME/Engine/Build/BatchFiles/RunUAT.bat"

echo "UE_HOME ${UE_HOME}"
echo "UE_RUNUAT ${UE_RUNUAT}"

SCRIPT_DIR=$(pwd)

/mnt/c/Windows/System32/Cmd.exe /C $UE_RUNUAT BuildPlugin -Plugin="$(wslpath -w .)/plugins/Bugsnag/Bugsnag.uplugin" -Package="$(wslpath -w .)/Build/Plugin/Bugsnag" -TargetPlatforms=Win32+Win64

cd "$SCRIPT_DIR/Build/Plugin"

zip -r "$SCRIPT_DIR/Bugsnag-UE$UE_VERSION-Windows.zip" Bugsnag
