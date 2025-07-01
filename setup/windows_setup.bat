@echo off
SETLOCAL EnableDelayedExpansion

:: Setup script for Windows dependencies
:: Installs vcpkg, Git, and Doxygen with checks for existing installations

:: Check if running as administrator
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo ERROR: This script requires administrator privileges.
    echo Please right-click the script and select "Run as administrator".
    pause
    exit /b 1
)

:: Set installation paths
set VCPKG_ROOT=C:\vcpkg
set DOXYGEN_PATH=C:\Program Files\doxygen
set GIT_PATH=C:\Program Files\Git

:: Check and install Git
echo Checking Git installation...
where git >nul 2>&1
if %errorLevel% neq 0 (
    echo Git not found. Installing Git...
    
    :: Download Git installer
    curl -L -o git-installer.exe https://github.com/git-for-windows/git/releases/download/v2.45.1.windows.1/Git-2.45.1-64-bit.exe
    
    :: Install Git silently
    start /wait git-installer.exe /VERYSILENT /NORESTART /NOCANCEL /SP- /CLOSEAPPLICATIONS /RESTARTAPPLICATIONS /COMPONENTS="icons,ext\reg\shellhere,assoc,assoc_sh"
    
    del git-installer.exe
    
    :: Add Git to PATH
    setx PATH "%PATH%;%GIT_PATH%\bin" /M
    echo Git installed successfully.
) else (
    echo Git is already installed.
)

:: Check and install Doxygen
echo Checking Doxygen installation...
where doxygen >nul 2>&1
if %errorLevel% neq 0 (
    echo Doxygen not found. Installing Doxygen...
    
    :: Download Doxygen
    curl -L -o doxygen-installer.exe https://www.doxygen.nl/files/doxygen-1.10.0-setup.exe
    
    :: Install Doxygen silently
    start /wait doxygen-installer.exe /VERYSILENT /NORESTART /SUPPRESSMSGBOXES
    
    del doxygen-installer.exe
    
    :: Add Doxygen to PATH
    setx PATH "%PATH%;%DOXYGEN_PATH%\bin" /M
    echo Doxygen installed successfully.
) else (
    echo Doxygen is already installed.
)

:: Check and install vcpkg
echo Checking vcpkg installation...
if not exist "%VCPKG_ROOT%\vcpkg.exe" (
    echo vcpkg not found. Installing vcpkg...
    
    :: Clone vcpkg
    git clone https://github.com/microsoft/vcpkg.git "%VCPKG_ROOT%"
    
    :: Bootstrap vcpkg
    cd "%VCPKG_ROOT%"
    .\bootstrap-vcpkg.bat -disableMetrics
    
    :: Add vcpkg to PATH
    setx PATH "%PATH%;%VCPKG_ROOT%" /M
    setx VCPKG_ROOT "%VCPKG_ROOT%" /M
    
    echo vcpkg installed successfully.
) else (
    echo vcpkg is already installed.
)

:: Final setup
echo.
echo ============================================
echo Setup completed successfully!
echo ============================================
echo.
echo Installed components:
echo - Git:         %GIT_PATH%
echo - Doxygen:     %DOXYGEN_PATH%
echo - vcpkg:       %VCPKG_ROOT%
echo.
echo You may need to restart your terminal or IDE for PATH changes to take effect.
pause