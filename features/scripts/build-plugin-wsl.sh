#!/bin/bash

UE_VERSION="${UE_VERSION:-4.27}"

UE_HOME="C:\Program Files\Epic Games\UE_$UE_VERSION"

UE_RUNUAT="$UE_HOME\Engine\Build\BatchFiles\RunUAT.bat"

/mnt/c/windows/system32/cmd.exe /C $UE_RUNUAT BuildPlugin -Plugin="$(wslpath -w .)/plugins/Bugsnag/Bugsnag.uplugin" -Package="$(wslpath -w .)/Build/Plugin/Bugsnag" -TargetPlatforms=Win32+Win64
