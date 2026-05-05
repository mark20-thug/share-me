
#include<iostream>
#include<map>
#include<chrono>
#include "folder.h"

// Function to load config file
std::map<std::string, std::string> load_config() {
    std::map<std::string, std::string> config;
    std::ifstream configfile("config.txt");
    std::string line;
    while (std::getline(configfile, line)) {
        if (line.empty() || line[0] == '#') continue;
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            config[key] = value;
        }
    }
    return config;
}

// Function to save config file
void save_config(const std::map<std::string, std::string>& config) {
    std::ofstream configfile("config.txt");
    configfile << "# Saved network paths and credentials\n";
    for (const auto& [key, value] : config) {
        configfile << key << "=" << value << std::endl;
    }
}

// Function to create backup before copying
bool create_backup(const fs::path& source) {
    try {
        std::string backup_path = source.string() + ".backup";
        if (fs::is_directory(source)) {
            fs::copy(source, backup_path, fs::copy_options::recursive);
        } else {
            fs::copy(source, backup_path);
        }
        std::cout << "Backup created: " << backup_path << std::endl;
        log_action("Backup created: " + backup_path);
        return true;
    } catch (const std::exception& e) {
        std::cout << "WARNING: Could not create backup: " << e.what() << std::endl;
        return false;
    }
}

int main() {
    std::cout << "\n========== FILE TRANSFER APPLICATION v2.0 (Cross-Platform) ==========" << std::endl;
    std::cout << "Platform: " << get_platform_info() << std::endl;
    std::cout << "\n";
    log_action("Application started on " + get_platform_info());
    
    // Load saved configurations
    auto config = load_config();
    
    std::cout << "Enter the path to the directory you want to use: ";
    std::string path;
    std::getline(std::cin, path);

    // Validate source path
    if (!validate_source(path)) {
        log_action("Failed: Invalid source path");
        return 1;
    }
    
    std::cout << "\n--------Printing contents of the directory---------\n" << std::endl;
    auto entries = list_directory_contents(path);
    
    if (entries.empty()) {
        log_action("Directory is empty or inaccessible");
        return 0;
    }
    
    std::cout << "\nSelect a file/folder (enter number): ";
    int choice;
    std::cin >> choice;
    
    if (choice < 1 || choice > entries.size()) {
        std::cout << "ERROR: Invalid choice" << std::endl;
        log_action("ERROR: Invalid choice - " + std::to_string(choice));
        return 1;
    }
    
    fs::path selected = entries[choice - 1];
    std::cout << "\nYou selected: " << selected.filename() << std::endl;
    
    // Show file size
    if (fs::is_regular_file(selected)) {
        auto size_bytes = fs::file_size(selected);
        std::cout << "Size: " << (size_bytes / 1024.0 / 1024.0) << " MB" << std::endl;
    } else {
        std::cout << "Type: Directory" << std::endl;
    }
    
    // Ask for backup option
    std::cout << "\nCreate backup before copying? (y/n): ";
    char backup_choice;
    std::cin >> backup_choice;
    if (backup_choice == 'y' || backup_choice == 'Y') {
        create_backup(selected);
    }
    
    std::cout << "\n=== Destination Path ===";
    std::cout << "\nRecent saved paths:" << std::endl;
    int config_count = 1;
    std::vector<std::string> saved_paths;
    for (const auto& [key, value] : config) {
        if (key.find("path_") == 0) {
            std::cout << config_count << ". " << value << std::endl;
            saved_paths.push_back(value);
            config_count++;
        }
    }
    
    std::cout << "\nEnter destination path: ";
    std::cout << "\nFor neighbor's PC, use platform-specific paths:" << std::endl;
    #ifdef _WIN32
        std::cout << "  Windows: \\\\\\\\192.168.1.100\\\\shared_folder or C:\\\\Path\\\\To\\\\Folder" << std::endl;
    #elif __APPLE__
        std::cout << "  macOS: /Volumes/neighbor_share or /Volumes/SharedFolder/path" << std::endl;
    #else
        std::cout << "  Linux: /mnt/neighbor_share or /media/username/shared_folder" << std::endl;
    #endif
    std::cout << "Or enter 'new' for new path, or number for saved path" << std::endl;
    std::cout << "> ";
    std::string destination;
    std::cin.ignore();
    std::getline(std::cin, destination);
    
    // Handle saved paths
    if (!destination.empty() && destination != "new" && std::isdigit(destination[0])) {
        try {
            int path_choice = std::stoi(destination) - 1;
            if (path_choice >= 0 && path_choice < saved_paths.size()) {
                destination = saved_paths[path_choice];
                std::cout << "Using saved path: " << destination << std::endl;
            }
        } catch (...) {
            // Continue with entered path
        }
    }
    
    // Validate destination path
    if (!validate_destination(destination)) {
        log_action("Failed: Invalid destination path");
        return 1;
    }
    
    // Save destination path to config
    std::cout << "\nSave this path for future use? (y/n): ";
    char save_choice;
    std::cin >> save_choice;
    if (save_choice == 'y' || save_choice == 'Y') {
        std::cout << "Enter name for this path: ";
        std::string path_name;
        std::cin.ignore();
        std::getline(std::cin, path_name);
        config["path_" + path_name] = destination;
        save_config(config);
        std::cout << "Path saved!" << std::endl;
    }
    
    try {
        std::cout << "\n--- Starting copy operation ---" << std::endl;
        auto start = std::chrono::system_clock::now();
        
        if (fs::is_directory(selected)) {
            std::cout << "Copying directory recursively..." << std::endl;
            fs::copy(selected, destination, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
        } else {
            std::cout << "Copying file..." << std::endl;
            fs::copy(selected, destination, fs::copy_options::overwrite_existing);
        }
        
        auto end = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
        
        std::cout << "\n✓ Successfully copied to: " << destination << std::endl;
        std::cout << "Time taken: " << duration << " seconds" << std::endl;
        log_action("SUCCESS: Copied " + selected.string() + " to " + destination);
        
        // Ask about undo
        std::cout << "\nUndo this copy? (y/n): ";
        char undo_choice;
        std::cin >> undo_choice;
        if (undo_choice == 'y' || undo_choice == 'Y') {
            fs::remove_all(destination);
            std::cout << "Copy operation undone." << std::endl;
            log_action("Copy operation undone");
        }
        
    } catch (const std::exception& e) {
        std::cout << "\n✗ ERROR during copy: " << e.what() << std::endl;
        std::cout << "Possible reasons:" << std::endl;
        std::cout << "  - Destination path doesn't exist" << std::endl;
        std::cout << "  - No write permissions on destination" << std::endl;
        std::cout << "  - Network is unreachable" << std::endl;
        std::cout << "  - Not enough disk space" << std::endl;
        log_action("ERROR: Copy failed - " + std::string(e.what()));
        return 1;
    }

    log_action("Application closed successfully");
    return 0;


}
