#!/bin/bash

# Set working directory.
cd "$(dirname "$0")"

# Set package name.
appname="visParaflow"
verison="1.0.0"

# Define deployment directory.
depdir="../../deploy/ubuntu"
if [ ! -d $depdir ]; then
    echo "The deployment directory does not exist."
    exit 1
fi

# Define setup directory.
setupdir="../../setup/ubuntu"
mkdir -p $setupdir

# Define staging directory.
tmpdir="app"

# Delete old staging directory.
rm -rf $tmpdir
echo "The old staging directory has been deleted."

# Create destination directory.
mkdir -p $tmpdir/DEBIAN
mkdir -p $tmpdir/opt/$appname/bin
mkdir -p $tmpdir/opt/$appname/lib
mkdir -p $tmpdir/opt/$appname/plugins
mkdir -p $tmpdir/usr/share/applications
mkdir -p $tmpdir/usr/share/icons/hicolor/48x48/apps
echo "The staging direcotry has been created."

# Copy DEBIAN control files.
cp -r debian/* $tmpdir/DEBIAN/
echo "The DEBIAN control files have been copied to the staging directory."

# Copy deployed files.
cp -r $depdir/* $tmpdir/
echo "The deployed files have been copied to the staging directory."

# Create DEBIAN package.
dpkg-deb --build $tmpdir $setupdir/$appname-$verison.deb
echo "The DEBIAN package has been created."

# Delete staging directory.
rm -rf $tmpdir
echo "The staging directory has been deleted."
