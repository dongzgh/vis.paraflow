#!/bin/bash

# Set working directory.
cd "$(dirname "$0")"

# Set package name.
appName="visParaflow"
verison="1.0.0"

# Define deployment directory.
deployDir="../../deploy/macos"
if [ ! -d $deployDir ]; then
    echo "The deployment directory does not exist."
    exit 1
fi

# Define setup directory.
setupDir="../../setup/macos"
mkdir -p $setupDir

# Deploy app.
cd $deployDir
macdeployqt6 $appName.app -always-overwrite -dmg
echo "The app has been deployed."

# Rename app.
mv $appName.dmg $setupDir/$appName-$verison.dmg
echo "The app has been renamed."
