#include <iostream>
#include <fstream>
#include <sstream>

class Config {
public:
    std::string sourceDir;
    std::string backupDir;
    int backupFrequency;
    bool startOnSystemStartup;

    Config() : sourceDir(""), backupDir(""), backupFrequency(1), startOnSystemStartup(false) {}

    void readConfig(const std::string& filename) {
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                std::string key, value;
                if (std::getline(iss, key, '=') && std::getline(iss, value)) {
                    if (key == "sourceDir") sourceDir = value;
                    else if (key == "backupDir") backupDir = value;
                    else if (key == "backupFrequency") backupFrequency = std::stoi(value);
                }
            }
            file.close();
        }
    }

    void writeConfig(const std::string& filename) {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << "sourceDir=" << sourceDir << std::endl;
            file << "backupDir=" << backupDir << std::endl;
            file << "backupFrequency=" << backupFrequency << std::endl;
            file.close();
        }
    }
};

int main() {
    Config config;

    // Read existing config or create a new one
    config.readConfig("config.ini");

    // Modify config parameters as needed
    config.sourceDir = "/home/kalisto/Desktop/Linuxoid/2laba/test";  // Replace with your source directory
    config.backupDir = "/home/kalisto/Desktop/Linuxoid/2laba/backup";  // Replace with your backup directory
    config.backupFrequency = 1;  // Replace with your desired backup frequency in minutes
    // Write updated config to file
    config.writeConfig("config.ini");

    return 0;
}
