#!/bin/bash

# Function to display usage
function usage() {
    echo "Usage: $0 <Unreal Engine Version> <Project Name> <Project Directory>"
    echo "Example: $0 5.1 MyNewProject /Users/username/Documents/UnrealProjects"
    exit 1
}

# Check if the correct number of arguments are passed
if [ $# -ne 3 ]; then
    usage
fi

# Parameters
UE_VERSION=$1
PROJECT_NAME=$2
PROJECT_DIR=$3

# Define the base path where Unreal Engine is installed
UE_BASE_PATH="/Users/Shared/Epic Games"

# Construct the path to the Unreal Engine executable
UE_EXECUTABLE="$UE_BASE_PATH/UE_$UE_VERSION/Engine/Binaries/Mac/UnrealEditor.app/Contents/MacOS/UnrealEditor"

# Check if Unreal Engine executable exists
if [ ! -f "$UE_EXECUTABLE" ]; then
    echo "Unreal Engine version $UE_VERSION not found in $UE_BASE_PATH."
    exit 1
fi

# Define the full path where the project will be created
FULL_PROJECT_PATH="$PROJECT_DIR/$PROJECT_NAME"

# Create the project directory
mkdir -p "$FULL_PROJECT_PATH"

# Generate a new blank Unreal Engine project
"$UE_EXECUTABLE" "$FULL_PROJECT_PATH/$PROJECT_NAME.uproject" -project="$FULL_PROJECT_PATH/$PROJECT_NAME.uproject" -template=Blank -console

echo "Project '$PROJECT_NAME' created successfully at '$FULL_PROJECT_PATH'."
