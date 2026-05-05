#!/bin/bash
# Quick build script for File Transfer Application
# Works on Linux and macOS
# For Windows, use the BUILD_INSTRUCTIONS.md

set -e

echo "=========================================="
echo "File Transfer Application - Build Script"
echo "=========================================="
echo ""

# Detect platform
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    PLATFORM="Linux"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    PLATFORM="macOS"
else
    echo "Error: Unsupported platform"
    exit 1
fi

echo "Platform detected: $PLATFORM"
echo ""

# Check if CMake is installed
if ! command -v cmake &> /dev/null; then
    echo "CMake not found. Installing..."
    if [[ "$PLATFORM" == "Linux" ]]; then
        sudo apt-get update
        sudo apt-get install -y cmake
    elif [[ "$PLATFORM" == "macOS" ]]; then
        if command -v brew &> /dev/null; then
            brew install cmake
        else
            echo "Please install Homebrew first: https://brew.sh"
            exit 1
        fi
    fi
fi

echo "CMake version:"
cmake --version
echo ""

# Create build directory
if [ -d "build" ]; then
    echo "Removing old build directory..."
    rm -rf build
fi

echo "Creating build directory..."
mkdir build
cd build

echo "Generating build files..."
cmake ..

echo ""
echo "Building application..."
if [[ "$PLATFORM" == "Linux" ]] || [[ "$PLATFORM" == "macOS" ]]; then
    make
fi

echo ""
echo "=========================================="
echo "✓ Build complete!"
echo "=========================================="
echo ""

# Check if executable exists
if [ -f "folder_app" ]; then
    echo "Executable: ./folder_app"
    echo ""
    echo "To run the application:"
    echo "  cd .."
    echo "  ./build/folder_app"
    echo ""
    echo "To install system-wide (optional):"
    echo "  sudo cmake --install . --prefix /usr/local"
    echo ""
else
    echo "Build failed or executable not found"
    exit 1
fi
