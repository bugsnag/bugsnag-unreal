#!/usr/bin/env bash

set -a

NDKROOT=/Users/administrator/Library/Android/sdk/ndk/21.4.7075529

rm -rf "/Users/administrator/Library/Logs/Unreal Engine/LocalBuildLogs/*"

make package
