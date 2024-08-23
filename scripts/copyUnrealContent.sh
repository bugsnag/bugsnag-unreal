#!/bin/bash

# Function to display usage
function usage() {
    echo "Usage: $0 <Source Project Directory> <Destination Project Directory>"
    echo "Example: $0 /Users/username/Documents/SourceProject /Users/username/Documents/UnrealProjects/MyNewProject"
    exit 1
}

# Check if the correct number of arguments are passed
if [ $# -ne 2 ]; then
    usage
fi

# Parameters
SOURCE_PROJECT_DIR=$1
DEST_PROJECT_DIR=$2

# Check if the source project directory exists
if [ ! -d "$SOURCE_PROJECT_DIR" ]; then
    echo "Source project directory '$SOURCE_PROJECT_DIR' does not exist."
    exit 1
fi

# Check if the destination project directory exists
if [ ! -d "$DEST_PROJECT_DIR" ]; then
    echo "Destination project directory '$DEST_PROJECT_DIR' does not exist."
    exit 1
fi

# Copy source project's code and content folders to the destination project
rsync -av --exclude='*.uproject' "$SOURCE_PROJECT_DIR/Source/" "$DEST_PROJECT_DIR/Source/"
rsync -av "$SOURCE_PROJECT_DIR/Content/" "$DEST_PROJECT_DIR/Content/"

# Copy other important files (Config, Plugins, etc.)
rsync -av "$SOURCE_PROJECT_DIR/Config/" "$DEST_PROJECT_DIR/Config/"
rsync -av "$SOURCE_PROJECT_DIR/Plugins/" "$DEST_PROJECT_DIR/Plugins/"

echo "Source project files copied to the destination project successfully."

# Notify user to regenerate project files
echo "Please open the destination project in Unreal Editor and regenerate project files."
