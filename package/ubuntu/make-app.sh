#!/bin/bash

# Set the working directory
cd "$(dirname "$0")"

# Set the package name
appname="visParaflow"
verison="1.0.0"

# Define the deployment directory.
depdir="../../deploy/ubuntu"
if [ ! -d $depdir ]; then
    echo "The deployment directory does not exist."
    exit 1
fi

# Define the setup directory.
setupdir="../../setup/ubuntu"
mkdir -p $setupdir

# Define the staging directory
tmpdir="app"

# Delete the old staging directory.
rm -rf $tmpdir
echo "The old staging directory has been deleted."

# Create the destination directory
mkdir -p $tmpdir/DEBIAN
mkdir -p $tmpdir/opt/$appname/bin
mkdir -p $tmpdir/opt/$appname/lib
mkdir -p $tmpdir/opt/$appname/plugins
mkdir -p $tmpdir/usr/share/applications
mkdir -p $tmpdir/usr/share/icons/hicolor/48x48/apps
echo "The staging direcotry has been created."

# Copy the DEBIAN control files.
cp -r debian/* $tmpdir/DEBIAN/
echo "The DEBIAN control files have been copied to the staging directory."

# Copy the deployed files.
cp -r $depdir/* $tmpdir/
echo "The deployed files have been copied to the staging directory."

# Create the DEBIAN package.
dpkg-deb --build $tmpdir $setupdir/$appname-$verison.deb
echo "The DEBIAN package has been created."

# Delete the staging directory.
rm -rf $tmpdir
echo "The staging directory has been deleted."
