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

# Define the staging package directory
pkgdir="app"

# Delete the old staging package directory.
rm -rf $pkgdir
echo "The old staging package directory has been deleted."

# Create the destination directory
mkdir -p $pkgdir/DEBIAN
mkdir -p $pkgdir/opt/$appname/bin
mkdir -p $pkgdir/usr/share/applications
mkdir -p $pkgdir/usr/share/icons/hicolor/48x48/apps
echo "The staging package directories have been created."

# Copy the DEBIAN control files.
cp -r debian/* $pkgdir/DEBIAN/
echo "The DEBIAN control files have been copied to the staging package directory."

# Copy the deployed files.
cp -r $depdir/* $pkgdir/
echo "The deployed files have been copied to the staging package directory."

# Create the DEBIAN package.
dpkg-deb --build $pkgdir $setupdir/$appname-$verison.deb
echo "The DEBIAN package has been created."

# Delete the staging package directory.
rm -rf $pkgdir
echo "The staging package directory has been deleted."
