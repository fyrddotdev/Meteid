@echo off
setlocal enabledelayedexpansion

:: --- Meteid: Raylib Edition - Android Build Setup ---
:: Author: Fyrd.dev_
:: ---------------------------------------------------

echo [INFO] Starting Android Build Environment Setup...

:: 1. Ask user for NDK Path if not set in environment variables
set /p NDK_PATH="Enter your Android NDK Path (e.g., C:/Android/ndk/27.x.x): "

:: 2. Validate NDK Path
if not exist "%NDK_PATH%/build/cmake/android.toolchain.cmake" (
    echo [ERROR] Invalid NDK Path! Could not find android.toolchain.cmake.
    pause
    exit /b 1
)

:: 3. Create build directory if it doesn't exist
if not exist build_android (
    echo [INFO] Creating build_android directory...
    mkdir build_android
)

cd build_android

:: 4. Run CMake Configuration
echo [INFO] Running CMake Configuration...
cmake -G "Ninja" ^
    -DPLATFORM=Android ^
    -DCMAKE_TOOLCHAIN_FILE="%NDK_PATH%/build/cmake/android.toolchain.cmake" ^
    -DANDROID_ABI="arm64-v8a" ^
    -DANDROID_PLATFORM=android-34 ^
    -DANDROID_NDK="%NDK_PATH%" ^
    -DCMAKE_BUILD_TYPE=Release ..

if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] CMake configuration failed!
    pause
    exit /b 1
)

echo.
echo ---------------------------------------------------
echo [SUCCESS] Setup Complete!
echo [NEXT STEP] To compile the APK, run:
echo [NEXT STEP] cd build_android
echo [NEXT STEP] cmake --build .
echo ---------------------------------------------------
pause