#!/bin/bash

# Set working directory.
cd "$(dirname "$0")"

# Set package name.
appname="visParaflow"
verison="1.0.0"

# Define deployment directory.
depdir="../../deploy/macos"
if [ ! -d $depdir ]; then
    echo "The deployment directory does not exist."
    exit 1
fi

# Define setup directory.
setupdir="../../setup/macos"
mkdir -p $setupdir

# Deploy app.
macdeployqt6 $depdir/$appname.app -always-overwrite -dmg
echo "The app has been deployed."

# Rename app.
mv $depdir/$appname.dmg $setupdir/$appname-$verison.dmg
echo "The app has been renamed."
