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

namespace fs = std::experimental::filesystem;

bool running = true;

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

                // Copy file
                fs::copy_file(entry.path(), backupFilePath, fs::copy_options::overwrite_existing);

                // Log the backup operation to the provided log file
                logFile << "Backup: " << entry.path().c_str() << " -> " << backupFilePath.c_str() << std::endl;
            }
        }

        // Zip the timestamped directory
        zipDirectory(backupTimestampDir, zipFilePath);

        // Remove the timestamped directory after zipping (optional)
        fs::remove_all(backupTimestampDir);
    } catch (const std::exception& e) {
        syslog(LOG_ERR, "Error during backup: %s", e.what());
    }
}

int main() {
    openlog("backupd", LOG_PID, LOG_USER);

    // Hardcoded configuration parameters
    std::string sourceDir = "/home/kalisto/Desktop/Linuxoid/2laba/test";  // Replace with your source directory
    std::string backupDir = "/home/kalisto/Desktop/Linuxoid/2laba/backup";  // Replace with your backup directory
    int backupFrequency = 1;  // Replace with your desired backup frequency in minutes

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
