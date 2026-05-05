# File Transfer Application - Cross-Platform Build Instructions

## Prerequisites
- CMake 3.10 or higher
- C++17 compatible compiler
- Git (optional, for version control)

### Windows
- Visual Studio 2015 or later (with C++ development tools)
- Or: MinGW-w64

### macOS
- Xcode Command Line Tools: `xcode-select --install`
- Homebrew (optional): `brew install cmake`

### Linux
- GCC or Clang compiler
- CMake: `sudo apt-get install cmake` (Ubuntu/Debian)
- Or: `sudo yum install cmake` (CentOS/RHEL)
- Or: `brew install cmake` (if using Homebrew)

---

## Building the Application

### Method 1: Using CMake (Recommended for all platforms)

#### Step 1: Create build directory
```bash
mkdir build
cd build
```

#### Step 2: Generate build files
```bash
cmake ..
```

#### Step 3: Build the application

**On Windows (with Visual Studio):**
```bash
cmake --build . --config Release
```

**On macOS and Linux:**
```bash
make
```

#### Step 4: Run the application

**Windows:**
```bash
Release\folder_app.exe
```

**macOS/Linux:**
```bash
./folder_app
```

---

### Method 2: Using g++/clang (Quick build)

#### On Linux/macOS:
```bash
g++ -std=c++17 -o folder_app folder.cpp
./folder_app
```

#### On Windows (MinGW):
```bash
g++ -std=c++17 -o folder_app.exe folder.cpp
folder_app.exe
```

---

## Installing the Application

### After building with CMake:

**On Windows:**
```bash
cmake --install . --prefix "C:\Program Files\FileTransferApp"
```

**On macOS/Linux:**
```bash
sudo cmake --install . --prefix /usr/local
```

Then run from anywhere:
```bash
folder_app
```

---

## Platform-Specific Notes

### Windows
- Supports UNC paths: `\\server\share`
- Ensure network drives are mapped or use full UNC paths
- Administrator privileges may be needed for some network locations

### macOS
- Mount network shares: `mount_smbfs //user:pass@server/share /Volumes/share`
- Use `/Volumes` path for mounted drives
- May need to enable file sharing in System Preferences

### Linux
- Mount network shares: `sudo mount -t cifs //server/share /mnt/share -o username=user`
- Or use NFS: `sudo mount -t nfs server:/share /mnt/share`
- Check permissions with `ls -l`

---

## Troubleshooting

### CMake not found
```bash
# Install CMake
# Ubuntu/Debian:
sudo apt-get install cmake

# macOS:
brew install cmake

# Windows: Download from cmake.org
```

### Permission denied (Linux/macOS)
```bash
chmod +x folder_app
```

### Network path not accessible
- Check network connectivity: `ping neighbor_ip`
- Verify share exists and is shared
- Check credentials and permissions
- Review logs in `copy_log.txt`

### Compilation errors
- Ensure C++17 support: `g++ --version`
- For older compilers, use: `g++ -std=c++1z` instead of `-std=c++17`

---

## Example Usage

```bash
$ ./folder_app

========== FILE TRANSFER APPLICATION v2.0 (Cross-Platform) ==========
Platform: Linux

Enter the path to the directory you want to use: /home/user/Documents

--------Printing contents of the directory---------

1. file1.txt (256.50 KB)
2. folder1
3. document.pdf (1.25 MB)

Select a file/folder (enter number): 1

You selected: file1.txt
Size: 0.25 MB

Create backup before copying? (y/n): y
Backup created: /home/user/Documents/file1.txt.backup

=== Destination Path ===
For neighbor's PC, use platform-specific paths:
  Linux: /mnt/neighbor_share or /media/username/shared_folder

Enter destination path: /mnt/neighbor_share/transfers/
Or enter 'new' for new path, or number for saved path
> /mnt/neighbor_share/transfers/

--- Starting copy operation ---
Copying file...

✓ Successfully copied to: /mnt/neighbor_share/transfers/
Time taken: 2 seconds

Undo this copy? (y/n): n

Application closed successfully
```

---

## Features

✓ Cross-platform support (Windows, macOS, Linux)
✓ Directory listing with file sizes
✓ Input validation and error checking
✓ Permission verification
✓ Backup creation before copying
✓ Undo capability
✓ Config file for saved paths
✓ Logging to `copy_log.txt`
✓ Network path support
✓ Progress indication and timing

---

## Building for Distribution

### Create a release build:
```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

### Windows installer (requires NSIS):
```bash
cpack -G NSIS
```

### macOS DMG package:
```bash
cpack -G DragNDrop
```

### Linux DEB package:
```bash
cpack -G DEB
```

---

For more information or to report issues, check the logs in `copy_log.txt`.
