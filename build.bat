@echo off

set UE_HOME=C:\Program Files\Epic Games\UE_4.27
set UE_BUILD=%UE_HOME%\Engine\Build\BatchFiles\Build.bat

set UPROJECT=%~dp0\BugsnagExample.uproject

call "%UE_BUILD%" BugsnagExampleEditor Win64 Development "%UPROJECT%" -waitmutex -NoHotReload
