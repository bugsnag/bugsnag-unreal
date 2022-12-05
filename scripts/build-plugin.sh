#!/usr/bin/env bash

set -a

UE_VERSION="${UE_VERSION:-4.27}"

if [[ "$UE_VERSION" == "4.27" ]]; then
  NDKROOT="/Users/administrator/Library/Android/sdk/ndk/21.4.7075529"
fi

rm -rf "/Users/administrator/Library/Logs/Unreal Engine/LocalBuildLogs/*"

make package
