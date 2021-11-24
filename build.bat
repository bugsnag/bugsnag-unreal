if not defined UE_VERSION set UE_VERSION=4.27
if not defined UE_HOME set UE_HOME=C:\Program Files\Epic Games\UE_%UE_VERSION%
set UE_RUN_UAT=%UE_HOME%\Engine\Build\BatchFiles\RunUAT.bat
set SCRIPT_DIR=%~dp0

call "%UE_RUN_UAT%" BuildPlugin -Plugin="%SCRIPT_DIR%\Plugins\Bugsnag\Bugsnag.uplugin" -Package="%SCRIPT_DIR%\Build\Plugin\Bugsnag" -TargetPlatforms=Win32+Win64
if errorlevel 1 exit 1
@echo on

pushd "%SCRIPT_DIR%\Build\Plugin"
powershell Compress-Archive Bugsnag "%SCRIPT_DIR%\Bugsnag-UE%UE_VERSION%-Windows.zip"
popd
