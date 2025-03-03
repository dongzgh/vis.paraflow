@echo off

REM===============================================================================
REM ASSET CUSTOMIZATION
REM===============================================================================
REM Set the conda environment name
set PYTHON_VERSION=3.11.9
REM===============================================================================

REM Set the current working directory to the parent directory of the current file
cd /d %~dp0/../
echo Current working directory set to: %cd%.

REM Check if the system Python version matches the required version
for /f "tokens=2 delims= " %%a in ('python --version') do set SYSTEM_PYTHON_VERSION=%%a
echo %SYSTEM_PYTHON_VERSION% | findstr /C:%PYTHON_VERSION% >nul
if not %errorlevel%==0 (
    echo System Python version %SYSTEM_PYTHON_VERSION% does not match the required version %PYTHON_VERSION%. 
    exit /b 1    
) else (
    echo System Python version matches the required version.
)

REM Activate the Python virtual environment
if exist .env (
    call .env\Scripts\activate
    echo Python virtual environment activated.
) else (
    echo .env directory not found.
)


REM Check if a Python script path is provided
if "%1"=="" (
    echo No Python script path provided.
    exit /b 1
) else (
    SET PYTHON_SCRIPT_PATH=%1
    echo Python script path: %1.
)

REM Run the Python script
echo Running the Python script...
python %PYTHON_SCRIPT_PATH%