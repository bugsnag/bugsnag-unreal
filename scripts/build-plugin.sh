#!/usr/bin/env bash

set -a

UE_VERSION="${UE_VERSION:-4.27}"

if [[ "$UE_VERSION" == "4.26" ]]; then
  NDKROOT="$HOME/Library/Android/sdk/ndk/21.4.7075529"
elif [[ "$UE_VERSION" == "4.27" ]]; then
  NDKROOT="$HOME/Library/Android/sdk/ndk/21.4.7075529"
elif [[ "$UE_VERSION" == "5.0" ]]; then
  NDKROOT="$HOME/Library/Android/sdk/ndk/21.4.7075529"
fi

rm -rf "$HOME/Library/Logs/Unreal Engine/LocalBuildLogs/*"

make package
