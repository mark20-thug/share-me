# Share me v1.0

A **cross-platform** C++ application for securely transferring files and folders to your neighbor's PC over a network.

## Features

- ✅ **Cross-Platform**: Works on Windows, macOS, and Linux
- ✅ **Network Support**: Copy files to neighbor's shared drives
- ✅ **Directory Listing**: Browse and select files/folders by number
- ✅ **File Information**: Shows file sizes before copying
- ✅ **Backup Creation**: Optional backup before copying
- ✅ **Error Handling**: Detailed error messages with troubleshooting tips
- ✅ **Undo Capability**: Revert copies if needed
- ✅ **Config Management**: Save frequently used paths
- ✅ **Permission Checking**: Validates read/write permissions
- ✅ **Logging**: All actions logged to `copy_log.txt` with timestamps

## Requirements

- C++17 compatible compiler (g++, clang, MSVC)
- CMake 3.10+ (for building)
- Network access to neighbor's shared folder

## Quick Start

### Build the Application

```bash
# Create build directory
mkdir build && cd build

# Generate build files
cmake ..

# Build (on Linux/macOS)
make

# Build (on Windows)
cmake --build . --config Release
```

### Run the Application

```bash
# Linux/macOS
./folder_app

# Windows
folder_app.exe
```

## Usage

1. **Enter source directory path**
   ```
   Enter the path to the directory you want to use: /home/user/Documents
   ```

2. **Select file or folder**
   ```
   1. document.pdf (2.50 MB)
   2. photos
   3. data.xlsx (0.15 MB)
   
   Select a file/folder (enter number): 1
   ```

3. **Create backup (optional)**
   ```
   Create backup before copying? (y/n): y
   ```

4. **Enter destination path**
   
   **For Windows:**
   ```
   \\192.168.1.5\shared_folder
   ```
   
   **For macOS:**
   ```
   /Volumes/SharedDrive/folder
   ```
   
   **For Linux:**
   ```
   /mnt/neighbor_share/folder
   ```

5. **Save path for future use**
   ```
   Save this path for future use? (y/n): y
   Enter name for this path: neighbor_backup
   ```

## Network Setup

### Windows to Windows
1. Enable file sharing on neighbor's PC
2. Right-click folder → Share → Choose people
3. Get their IP address: `ipconfig` → IPv4 Address
4. Use path: `\\192.168.1.x\shared_folder`

### Windows to Linux
1. Install Samba: `sudo apt-get install samba`
2. Configure `/etc/samba/smb.conf`
3. Mount on your PC: `net use Z: \\neighbor_ip\share /persistent:yes`

### Linux to Linux
1. Share folder via NFS or Samba
2. Mount: `sudo mount -t cifs //neighbor_ip/share /mnt/neighbor -o username=user`
3. Use path: `/mnt/neighbor/folder`

### macOS Setup
1. System Preferences → Sharing → File Sharing
2. Mount: `mount_smbfs //user@neighbor/share /Volumes/share`
3. Use path: `/Volumes/share/folder`

## Configuration File

Saved paths are stored in `config.txt` with format:
```
# Saved network paths and credentials
path_neighbor_backup=/mnt/neighbor_share/backups
path_office_drive=\\office_server\Documents
path_mac_share=/Volumes/MacShare
```

## Log File

All operations are logged to `copy_log.txt`:
```
2026-05-05 14:23:45 | Application started on Linux
2026-05-05 14:24:10 | Backup created: /home/user/file.txt.backup
2026-05-05 14:24:15 | SUCCESS: Copied /home/user/file.txt to /mnt/neighbor_share/file.txt
2026-05-05 14:24:20 | Copy operation undone
```

## Troubleshooting

### Network Path Not Found
- Check network connectivity: `ping neighbor_ip`
- Verify the share is accessible
- Ensure correct path format for your OS
- Check firewall settings

### Permission Denied
```
ERROR: No read permissions for: /path/to/file
```
- Check file permissions: `ls -l` (Linux/macOS)
- Grant read access: `chmod +r filename`
- Run with appropriate user/administrator privileges

### Insufficient Disk Space
- Check destination disk space: `df -h` (Linux/macOS)
- Check neighbor's available space on shared folder
- Consider breaking large transfers into smaller batches

### CMake Build Error
- Ensure C++17 support: `g++ --version`
- Install CMake: See `BUILD_INSTRUCTIONS.md`
- Check compiler: `which g++` or `which clang`

## Building from Source

For detailed build instructions on your platform, see [BUILD_INSTRUCTIONS.md](BUILD_INSTRUCTIONS.md).

## Architecture

```
folder_app
├── folder.h (header with cross-platform utilities)
├── folder.cpp (main application logic)
├── config.txt (saved network paths)
├── copy_log.txt (operation logs)
└── CMakeLists.txt (build configuration)
```

### Key Functions
- `log_action()` - Cross-platform logging
- `check_permissions()` - Platform-specific permission checking
- `validate_source()` / `validate_destination()` - Path validation
- `create_backup()` - Backup creation
- `load_config()` / `save_config()` - Configuration management
- `get_platform_info()` - Platform detection

## Performance

- Small files (< 10 MB): < 1 second
- Medium files (10-100 MB): 1-5 seconds
- Large folders (> 1 GB): Depends on network speed
- Network bandwidth: Utilizes available network speed

## Security Considerations

1. **Passwords**: Store network credentials securely in OS keychain
2. **Backups**: Automatic backups saved as `filename.backup`
3. **Logging**: Review `copy_log.txt` for unauthorized access attempts
4. **Permissions**: Verify file/folder permissions before copying
5. **Network**: Use VPN for sensitive data transfers over public networks

## Supported Platforms

| Platform | Status | Notes |
|----------|--------|-------|
| Windows 10/11 | ✅ Tested | MSVC, MinGW compilers |
| macOS 10.15+ | ✅ Tested | Clang compiler |
| Ubuntu 18.04+ | ✅ Tested | GCC compiler |
| CentOS 7+ | ✅ Tested | GCC compiler |
| Other Linux | ✅ Likely | Requires C++17 compiler |

## Future Enhancements

- [ ] GUI interface using Qt or wxWidgets
- [ ] Encryption for network transfers
- [ ] Resume partial transfers
- [ ] Bandwidth throttling
- [ ] Scheduled transfers
- [ ] File sync mode (rsync-like)
- [ ] Mobile app companion

## License

This project is open source. Feel free to modify and distribute.

## Author

Created by Sserunjogi Mark Tark Ssendawula form Tarkx for secure file transfer between network-connected machines.

## Support

For issues, questions, or suggestions:
1. Check `copy_log.txt` for error details
2. Review [BUILD_INSTRUCTIONS.md](BUILD_INSTRUCTIONS.md)
3. Verify network connectivity and permissions
4. Ensure correct path format for your platform

---

**Version**: 2.0 (Cross-Platform)  
**Last Updated**: May 5, 2026  
**Status**: Production Ready ✅
