#!/bin/bash

# Set the working directory
cd "$(dirname "$0")"

# Set the package names
appname="visParaflow"
verison="1.0.0"

# Set package directory
pkgdir="app"

# Set build directory
bindir="../../build/release/bin"

# Copy app to the package directory
mkdir -p $pkgdir
cp -r $bindir/$appname.app $pkgdir/
echo "The app has been copied to the package directory."

# Deploy the app
macdeployqt6 $pkgdir/$appname.app -always-overwrite -dmg
echo "The app has been deployed."

# Rename the app
mv $pkgdir/$appname.dmg ./$appname-$verison.dmg
echo "The app has been renamed."

# Remove the package directory
rm -rf $pkgdir
echo "The package directory has been removed."
