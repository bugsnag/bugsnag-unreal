@echo off

if not defined UE_VERSION set UE_VERSION=4.27
if not defined UE_HOME set UE_HOME=C:\Program Files\Epic Games\UE_%UE_VERSION%
set UE_BUILD=%UE_HOME%\Engine\Build\BatchFiles\Build.bat

set UPROJECT=%~dp0\BugsnagExample.uproject

call "%UE_BUILD%" BugsnagExampleEditor Win64 Development "%UPROJECT%" -waitmutex -NoHotReload
