#!/bin/bash

#===============================================================================
# ASSET CUSTOMIZATION
#===============================================================================
# Set the conda environment name
PYTHON_VERSION="3.11.11"
#===============================================================================

# Set the current working directory to the directory of the current file
cd "$(dirname "$(dirname "$0")")"
echo "Current working directory set to: $(pwd)."

# Check if the system Python version matches the required version
SYSTEM_PYTHON_VERSION=$(python --version 2>&1)
if [[ "$SYSTEM_PYTHON_VERSION" != *"$PYTHON_VERSION"* ]]; then
  echo "System Python version ($SYSTEM_PYTHON_VERSION) does not match the required version ($PYTHON_VERSION)."
  exit 1
else
  echo "System Python version matches the required version."
fi

# Activate the Python virtual environment
if [ -d .env ]; then
  source .env/bin/activate
  echo "Python virtual environment activated."
else
  echo ".env directory not found."
fi

# Check if a Python script path is provided
if [ -z "$1" ]; then
    echo "No Python script path provided."
    exit 1
else
    PYTHON_SCRIPT_PATH="$1"
    echo "Python script path: $PYTHON_SCRIPT_PATH."
fi

# Run the Python script
echo "Running the Python script..."
python $PYTHON_SCRIPT_PATH
