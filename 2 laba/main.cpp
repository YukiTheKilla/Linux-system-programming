#include <cstdlib>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <unistd.h>
#include <limits.h>
#include <syslog.h>
#include <sys/stat.h>
#include <csignal>
#include <fstream>
#include <experimental/filesystem>
#include <thread>
#include <zip.h>
#include <sstream>
#include <map>

namespace fs = std::experimental::filesystem;

bool running = true;
class Config {
public:
    std::string sourceDir;
    std::string backupDir;
    int backupFrequency;

    void readConfig(const std::string& filename) {
        std::ifstream configFile(filename);
        if (!configFile.is_open()) {
            throw std::runtime_error("Unable to open config file: " + filename);
        }

        std::string line;
        while (std::getline(configFile, line)) {
            std::istringstream iss(line);
            std::string key, value;
            if (std::getline(iss, key, '=') && std::getline(iss, value)) {
                trim(key);
                trim(value);
                processConfigLine(key, value);
            }
        }

        configFile.close();
    }

private:
    void trim(std::string& str) {
        size_t first = str.find_first_not_of(' ');
        size_t last = str.find_last_not_of(' ');
        str = str.substr(first, (last - first + 1));
    }

    void processConfigLine(const std::string& key, const std::string& value) {
        if (key == "sourceDir") {
            sourceDir = value;
        } else if (key == "backupDir") {
            backupDir = value;
        } else if (key == "backupFrequency") {
            backupFrequency = std::stoi(value);
        }
        // Add more configurations if needed
    }
};
void signalHandler(int signum) {
    if (signum == SIGTERM || signum == SIGINT) {
        syslog(LOG_INFO, "Received termination signal. Stopping daemon.");
        running = false;
    }
}

std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time_t_now), "%Y%m%d_%H%M%S");

    return oss.str();
}

void zipDirectory(const std::string& sourceDir, const std::string& zipFilePath) {
    zip_t* zip = zip_open(zipFilePath.c_str(), ZIP_CREATE | ZIP_EXCL, nullptr);
    if (!zip) {
        throw std::runtime_error("Error creating zip file");
    }

    for (const auto& entry : fs::directory_iterator(sourceDir)) {
        if (fs::is_regular_file(entry.path())) {
            std::string fileName = entry.path().filename().string();
            std::string entryName = fileName;

            zip_source_t* source = zip_source_file(zip, entry.path().c_str(), 0, 0);
            if (!source) {
                throw std::runtime_error("Error adding file to zip");
            }

            if (zip_file_add(zip, entryName.c_str(), source, ZIP_FL_OVERWRITE) < 0) {
                zip_source_free(source);
                throw std::runtime_error("Error adding file to zip");
            }
        }
    }

    zip_close(zip);
}

void backupFiles(const std::string& sourceDir, const std::string& backupDir, std::ofstream& logFile) {
    std::string timestamp = getCurrentTimestamp();
    std::string backupTimestampDir = backupDir + "/" + timestamp;
    std::string zipFilePath = backupDir + "/" + timestamp + ".zip";

    try {
        // Create a directory with the current timestamp for backups
        fs::create_directory(backupTimestampDir);

        // Copy files to the timestamped directory
        for (const auto& entry : fs::directory_iterator(sourceDir)) {
            if (fs::is_regular_file(entry.path())) {
                std::string fileName = entry.path().filename().string();
                std::string backupFilePath = backupTimestampDir + "/" + fileName;

                fs::copy_file(entry.path(), backupFilePath, fs::copy_options::overwrite_existing);

                // Log the backup operation to the provided log file
                logFile << "Backup: " << entry.path().c_str() << " -> " << backupFilePath.c_str() << std::endl;
            }
        }

        // Zip the timestamped directory
        zipDirectory(backupTimestampDir, zipFilePath);

        fs::remove_all(backupTimestampDir);
    } catch (const std::exception& e) {
        syslog(LOG_ERR, "Error during backup: %s", e.what());
    }
}

int main() {
    openlog("backupd", LOG_PID, LOG_USER);

    // Load configuration from config.ini
    Config config;
    config.readConfig("config.ini");

    std::string sourceDir = config.sourceDir;
    std::string backupDir = config.backupDir;
    int backupFrequency = config.backupFrequency;


    pid_t pid = fork();

    if (pid < 0) {
        syslog(LOG_ERR, "Error forking process for daemon.");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        syslog(LOG_INFO, "Daemon started with PID: %d", pid);
        closelog();
        exit(EXIT_SUCCESS);
    }

    umask(S_IRWXU | S_IRWXG | S_IRWXO);

    setsid();

    signal(SIGTERM, signalHandler);
    signal(SIGINT, signalHandler);

    // Get the directory where the daemon is located
    char buffer[PATH_MAX];
    if (getcwd(buffer, sizeof(buffer)) == NULL) {
        syslog(LOG_ERR, "Error getting current working directory.");
        exit(EXIT_FAILURE);
    }

    std::string daemonDirectory(buffer);
    std::string logFilePath = daemonDirectory + "/daemon_log.txt";

    // Open the log file for writing or create it if it doesn't exist
    std::ofstream logFile(logFilePath, std::ios::app);
    if (!logFile.is_open()) {
        syslog(LOG_ERR, "Error opening log file for writing.");
        exit(EXIT_FAILURE);
    }

    logFile << "Daemon started at " << getCurrentTimestamp() << std::endl;
    logFile.close();

    while (running) {
        std::this_thread::sleep_for(std::chrono::minutes(backupFrequency));

        // Perform backup and zip
        backupFiles(sourceDir, backupDir, logFile);

        // Log operation to system log
        syslog(LOG_INFO, "Backup completed at %s", getCurrentTimestamp().c_str());
    }

    closelog();
    return 0;
}
