#!/bin/bash

# Ensure the script exits if any command fails
set -e

# Define the input PNG file and output ICNS file
INPUT_PNG="../res/workflow.png"
OUTPUT_DIR="../res/"

# Create the iconset directory
ICONSET_DIR="workflow.iconset"
ICONSET_ICNS="workflow.icns"
mkdir -p $ICONSET_DIR

# Generate the various icon sizes
sips -z 16 16     $INPUT_PNG --out $ICONSET_DIR/icon_16x16.png
sips -z 32 32     $INPUT_PNG --out $ICONSET_DIR/icon_16x16@2x.png
sips -z 32 32     $INPUT_PNG --out $ICONSET_DIR/icon_32x32.png
sips -z 64 64     $INPUT_PNG --out $ICONSET_DIR/icon_32x32@2x.png
sips -z 128 128   $INPUT_PNG --out $ICONSET_DIR/icon_128x128.png
sips -z 256 256   $INPUT_PNG --out $ICONSET_DIR/icon_128x128@2x.png
sips -z 256 256   $INPUT_PNG --out $ICONSET_DIR/icon_256x256.png
sips -z 512 512   $INPUT_PNG --out $ICONSET_DIR/icon_256x256@2x.png
sips -z 512 512   $INPUT_PNG --out $ICONSET_DIR/icon_512x512.png
sips -z 1024 1024 $INPUT_PNG --out $ICONSET_DIR/icon_512x512@2x.png

# Convert the iconset to an ICNS file
iconutil -c icns $ICONSET_DIR

# Move the ICNS file to the desired location
mv $ICONSET_ICNS $OUTPUT_DIR

# Clean up the iconset directory
rm -r $ICONSET_DIR

echo "ICNS file created at $OUTPUT_DIR"