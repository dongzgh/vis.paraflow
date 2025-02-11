# visParaflowAssets

This repository is for accumulating assets to be used for visParaflow workflow modeling.

## Asset List

- `image/`: Contains image processing assets and related configurations.
- `video/`: Contains video assets and related configurations.

## Working on Assets

Following the commands below to work on assets:

```bash
# Enter into asset folder.
cd <asset> # e.g., image, video etc.

# Create a Python virtual environment.
python -m venv .env

# Activate the Python virtual environment.
.env/Scripts/activate.bat # on Windows.
source .env/bin/activate # on Linux/macOS.

# Install required packages.
python -m pip install -r requirements.txt

# Open Visual Studio Code for editing.
code . 
```
