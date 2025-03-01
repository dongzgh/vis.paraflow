#!/bin/sh
# Set appName.
appName="visParaflow"

# Set LD_LIBRARY_PATH.
LD_LIBRARY_PATH=/opt/$appName/lib
export LD_LIBRARY_PATH

# Set QT_PLUGIN_PATH.
QT_PLUGIN_PATH=/opt/$appName/plugins
export QT_PLUGIN_PATH

# Set QT_QPA_PLATFORM to xcb.
export QT_QPA_PLATFORM=xcb

# Run the application.
/opt/$appName/bin/$appName "$@"
