#!/bin/bash

# Set the working directory
cd "$(dirname "$0")"

# Set the package name
appname="visParaflow"
verison="1.0.0"

# Define the package directory
pkgdir="app"

# Delete the old package
rm -rf $pkgdir
echo "The old package directory has been deleted."

# Create the destination directory
mkdir -p $pkgdir/DEBIAN
mkdir -p $pkgdir/opt/$appname/bin
mkdir -p $pkgdir/opt/$appname/lib
mkdir -p $pkgdir/opt/$appname/plugins
mkdir -p $pkgdir/usr/share/applications
mkdir -p $pkgdir/usr/share/icons/hicolor/48x48/apps
echo "The destination package directory have been created."

# Copy the DEBIAN control file.
cp debian/control $pkgdir/DEBIAN/control
echo "The DEBIAN control file has been copied to package."

# Copy the DEBIAN postinst file.
cp debian/postinst $pkgdir/DEBIAN/postinst
echo "The DEBIAN postinst file has been copied to package."

# Copy the DEBIAN postrm file.
cp debian/postrm $pkgdir/DEBIAN/postrm
echo "The DEBIAN postrm file has been copied to package."

# Copy the executable to the destination directory
cp ../../build/release/bin/$appname $pkgdir/opt/$appname/bin/
echo "The executable has been copied to the package."

# Copy the Qt libraries to the destination directory
cp -r /opt/Qt/6.8.1/gcc_64/lib $pkgdir/opt/$appname/
echo "The dependency libraries have been copied to the package."

# Copy the Qt plugins to the destination directory
cp -r /opt/Qt/6.8.1/gcc_64/plugins/platforms $pkgdir/opt/$appname/plugins/
echo "The dependency plugins have been copied to the package."

# Copy the app script to the destination directory
# Cope app stript file directly to usr/local/bin sometimes run into permission issues,
# so we copy it to /opt/${PRODUCT} first
cp app.sh $pkgdir/opt/$appname
echo "The app script has been copied to the package."

# Copy the app desktop file to the destination directory
cp app.desktop $pkgdir/usr/share/applications/$appname.desktop
echo "The app desktop file has been copied to the package."

# Copy the app icon to the destination directory
cp app.svg $pkgdir/usr/share/icons/hicolor/48x48/apps/$appname.svg
echo "The app icon has been copied to the package."

# Package the app.
dpkg-deb --build $pkgdir $appname-$verison.deb
echo "The DEBIAN package has been created."

# Clean up the package directory
rm -rf $pkgdir
echo "The package directory has been cleaned up."
