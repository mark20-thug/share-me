#pragma once

#include<iostream>
#include<filesystem>
#include<vector>
#include<fstream>
#include<ctime>
#include<iomanip>
#include<chrono>
#include<string>

#ifdef _WIN32
    #include<windows.h>
#else
    #include<sys/stat.h>
#endif

namespace fs = std::filesystem;

// Logging function
void log_action(const std::string& message) {
    std::ofstream logfile("copy_log.txt", std::ios::app);
    if (logfile.is_open()) {
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        logfile << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " | " << message << std::endl;
        logfile.close();
    }
}

// Cross-platform permission check
bool check_permissions(const std::string& path) {
    #ifdef _WIN32
        DWORD attribs = GetFileAttributesA(path.c_str());
        return (attribs != INVALID_FILE_ATTRIBUTES);
    #else
        struct stat buffer;
        if (stat(path.c_str(), &buffer) != 0) return false;
        return (buffer.st_mode & S_IRUSR);
    #endif
}

// Validate source path
bool validate_source(const std::string& path) {
    if (!fs::exists(path)) {
        std::cout << "ERROR: Path does not exist: " << path << std::endl;
        log_action("ERROR: Path does not exist: " + path);
        return false;
    }
    if (!check_permissions(path)) {
        std::cout << "ERROR: No read permissions for: " << path << std::endl;
        log_action("ERROR: No read permissions for: " + path);
        return false;
    }
    return true;
}

// Validate destination path
bool validate_destination(const std::string& dest) {
    fs::path dest_path(dest);
    fs::path parent = dest_path.parent_path();
    if (!fs::exists(parent)) {
        std::cout << "ERROR: Destination parent directory does not exist: " << parent << std::endl;
        log_action("ERROR: Destination parent directory does not exist: " + parent.string());
        return false;
    }
    return true;
}

// Display file size
void show_file_size(const fs::path& path) {
    if (fs::is_regular_file(path)) {
        auto size = fs::file_size(path) / 1024.0; // Convert to KB
        std::cout << " (" << std::fixed << std::setprecision(2) << size << " KB)";
    }
}
// Get platform-specific info
std::string get_platform_info() {
    #ifdef _WIN32
        return "Windows";
    #elif __APPLE__
        return "macOS";
    #else
        return "Linux";
    #endif
}
std::vector<fs::path> list_directory_contents(const std::string& path) {
    std::vector<fs::path> entries;
    if(!fs::exists(path)) {
        std::cout << "ERROR: The specified path does not exist" << std::endl;
        log_action("ERROR: Path does not exist: " + path);
        return entries;
    }
    if (fs::is_empty(path)) {
        std::cout << "The directory is empty" << std::endl;
    } else {
        int count = 1;
        for (const auto& entry : fs::directory_iterator(path)) {
            std::cout << count << ". " << entry.path().filename();
            show_file_size(entry.path());
            std::cout << std::endl;
            entries.emplace_back(entry.path());
            count++;
        }
    }
    return entries;
}
