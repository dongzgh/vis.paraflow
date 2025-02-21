#!/bin/bash

# Set working directory to the directory of the script
cd "$(dirname "$0")"

# Set the package name.
appname="visParaflow"

# Define package directory
pkgdir="package"

# Delete the olde package folder
rm -rf $pkgdir/$appname
echo "The old package folder has been deleted."

# Create the destination directory if it doesn't exist
mkdir -p $pkgdir/DEBIAN
mkdir -p $pkgdir/opt/$appname/bin
mkdir -p $pkgdir/opt/$appname/lib
mkdir -p $pkgdir/opt/$appname/plugins
echo "The destination package folder have been created."

# Copy the DEBIAN control file.
cp control $pkgdir/DEBIAN/
echo "The DEBIAN control file has been copied to package."

# Copy the binary to the destination directory
cp ../../build/release/bin/visParaflow $pkgdir/opt/$appname/bin/
echo "The binary has been copied to package bin."

# # Get the list of libraries used by the binary
# libs=$(ldd ../build/release/bin/visParaflow | awk '{print $3}' | grep -v '^(')

# # Copy each library to the destination directory
# for lib in $libs; do
#   real_lib=$(readlink -f $lib)
#   # Skip system libraries
#   if [[ $real_lib == /lib* ]] || [[ $real_lib == /usr/lib* ]] || [[ $real_lib == /usr/local/lib* ]]; then
#     continue
#   fi
#   cp -L $real_lib visParaflow/lib/
# done
# echo "All libraries have been copied to visParaflow/lib."

# Copy the Qt libraries to the destination directory
cp -r /opt/Qt/6.8.1/gcc_64/lib $pkgdir/opt/$appname/
echo "The Qt libraries have been copied to package lib."

# Copy the Qt plugins to the destination directory
cp -r /opt/Qt/6.8.1/gcc_64/plugins/platforms $pkgdir/opt/$appname/plugins/
echo "The Qt plugins have been copied to package plugins."

# Copy the visParaflow script.
cp visParaflow.sh $pkgdir/opt/$appname/
echo "The visParaflow runner script has been copied to package."
