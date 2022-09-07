#!/usr/bin/env bash

set -Eeuo pipefail

UE_VERSION="${UE_VERSION:-4.27}"

UE_HOME="C:\Program Files\Epic Games\UE_$UE_VERSION"
UE_RUNUAT="$UE_HOME\Engine\Build\BatchFiles\RunUAT.bat"

SCRIPT_DIR=$(pwd)

echo "\"$UE_RUNUAT\" BuildPlugin -Plugin="$(wslpath -w .)/plugins/Bugsnag/Bugsnag.uplugin" -Package="$(wslpath -w .)/Build/Plugin/Bugsnag" -TargetPlatforms=Win32+Win64" > run_plugin.bat

cat run_plugin.bat

/mnt/c/Windows/System32/Cmd.exe /C run_plugin.bat

cd "$SCRIPT_DIR/Build/Plugin"

zip -r "$SCRIPT_DIR/Bugsnag-UE$UE_VERSION-Windows.zip" Bugsnag
