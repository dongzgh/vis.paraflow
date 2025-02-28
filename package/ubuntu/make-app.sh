#!/bin/bash

# Set working directory.
cd "$(dirname "$0")"

# Set package name.
appName="visParaflow"
verison="1.0.0"

# Define deployment directory.
deployDir="../../deploy/ubuntu"
if [ ! -d $deployDir ]; then
    echo "The deployment directory does not exist."
    exit 1
fi

# Define setup directory.
setupDir="../../setup/ubuntu"
mkdir -p $setupDir

# Define staging directory.
tmpDir="app"

# Delete old staging directory.
rm -rf $tmpDir
echo "The old staging directory has been deleted."

# Create destination directory.
mkdir -p $tmpDir/DEBIAN
mkdir -p $tmpDir/opt/$appName/bin
mkdir -p $tmpDir/opt/$appName/lib
mkdir -p $tmpDir/opt/$appName/plugins
mkdir -p $tmpDir/usr/share/applications
mkdir -p $tmpDir/usr/share/icons/hicolor/48x48/apps
echo "The staging direcotry has been created."

# Copy DEBIAN control files.
cp -r debian/* $tmpDir/DEBIAN/
echo "The DEBIAN control files have been copied to the staging directory."

# Copy deployed files.
cp -r $deployDir/* $tmpDir/
echo "The deployed files have been copied to the staging directory."

# Create DEBIAN package.
dpkg-deb --build $tmpDir $setupDir/$appName-$verison.deb
echo "The DEBIAN package has been created."

# Delete staging directory.
rm -rf $tmpDir
echo "The staging directory has been deleted."
