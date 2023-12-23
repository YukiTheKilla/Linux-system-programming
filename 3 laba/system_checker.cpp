#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <thread>
#include <xlsxwriter.h>
#include <csignal>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>

namespace fs = std::filesystem;

lxw_workbook *workbook = nullptr;
// (number-1) should be / by 7 (size of array) to make a better look
const int MAX_HISTORY_ENTRIES = 70001;

void signalHandler(int signum) {
    std::cout << "Interrupt signal (" << signum << ") received.\n";

    if (workbook != nullptr) {
        workbook_close(workbook);
    }

    exit(signum);
}

std::string getProcessOwner(const std::string &pid) {
    struct stat statBuf;
    std::string procPath = "/proc/" + pid;

    if (stat(procPath.c_str(), &statBuf) == 0) {
        struct passwd *pw = getpwuid(statBuf.st_uid);
        if (pw != nullptr) {
            return pw->pw_name;
        }
    }

    return "Unknown";
}

void logProcessHistoryJSON(const std::string &pid, const std::string &status) {
    std::ifstream historyFile("process_history.json");
    std::vector<std::string> historyEntries;
    std::string line;

    while (std::getline(historyFile, line)) {
        historyEntries.push_back(line);
    }

    while (historyEntries.size() >= MAX_HISTORY_ENTRIES) {
        historyEntries.erase(historyEntries.begin());
    }

    std::ifstream cmdlineFile("/proc/" + pid + "/cmdline");
    std::string processDescription;
    if (cmdlineFile.is_open()) {
        std::getline(cmdlineFile, processDescription, '\0');
    }

    std::string processOwner = getProcessOwner(pid);

    historyEntries.push_back("{\n\"pid\": \"" + pid + "\",\n\"status\": \"" + status + "\",\n\"description\": \""
                             + processDescription + "\",\n\"owner\": \"" + processOwner + "\",\n\"timestamp\": \""
                             + std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) + "\"\n}");

    std::ofstream outputFile("process_history.json");
    for (const auto &entry : historyEntries) {
        outputFile << entry << '\n';
    }

    // Check if the processDescription contains the specified message
    if (processDescription.find("/snap/telegram-desktop/5305/usr/bin/telegram-desktop") != std::string::npos) {
        std::cout << ">>>>>>>>>>>>>>>>>>>>Notification: Telegram Desktop detected!<<<<<<<<<<<<<<<<<<<<<<<<<\n";
    }
}

void logProcessHistoryXLSX(const std::string &pid, const std::string &status, lxw_worksheet *worksheet, int row) {
    lxw_format *bold = workbook_add_format(workbook);
    format_set_bold(bold);

    std::ifstream cmdlineFile("/proc/" + pid + "/cmdline");
    std::string processDescription;
    if (cmdlineFile.is_open()) {
        std::getline(cmdlineFile, processDescription, '\0');
    }

    std::string processOwner = getProcessOwner(pid);

    worksheet_write_string(worksheet, row, 0, pid.c_str(), nullptr);
    worksheet_write_string(worksheet, row, 1, status.c_str(), bold);
    worksheet_write_string(worksheet, row, 2, processDescription.c_str(), nullptr);
    worksheet_write_string(worksheet, row, 3, processOwner.c_str(), nullptr);
    worksheet_write_number(worksheet, row, 4, std::chrono::system_clock::now().time_since_epoch().count(), nullptr);
}

std::unordered_set<std::string> getRunningProcesses() {
    std::unordered_set<std::string> processes;

    for (const auto &entry : fs::directory_iterator("/proc")) {
        const std::string entryName = entry.path().filename().string();

        if (std::all_of(entryName.begin(), entryName.end(), isdigit)) {
            processes.insert(entryName);
        }
    }

    return processes;
}

void monitorProcesses() {
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, nullptr);

    worksheet_write_string(worksheet, 0, 0, "PID", nullptr);
    worksheet_write_string(worksheet, 0, 1, "Status", nullptr);
    worksheet_write_string(worksheet, 0, 2, "Description", nullptr);
    worksheet_write_string(worksheet, 0, 3, "Owner", nullptr);
    worksheet_write_string(worksheet, 0, 4, "Timestamp", nullptr);

    std::unordered_set<std::string> runningProcesses = getRunningProcesses();
    int row = 1;

    for (const auto &pid : runningProcesses) {
        std::cout << "Working: " << pid << std::endl;
        logProcessHistoryJSON(pid, "working");
        logProcessHistoryXLSX(pid, "working", worksheet, row++);
    }

    std::unordered_set<std::string> currentProcesses;

    while (true) {
        currentProcesses = getRunningProcesses();

        for (const auto &pid : currentProcesses) {
            if (runningProcesses.find(pid) == runningProcesses.end()) {
                std::cout << "Started " << pid << std::endl;
                logProcessHistoryJSON(pid, "started");
                logProcessHistoryXLSX(pid, "started", worksheet, row++);
            }
        }

        for (const auto &pid : runningProcesses) {
            if (currentProcesses.find(pid) == currentProcesses.end()) {
                std::cout << "Terminated: " << pid << std::endl;
                logProcessHistoryJSON(pid, "terminated");
                logProcessHistoryXLSX(pid, "terminated", worksheet, row++);
            }
        }

        runningProcesses = currentProcesses;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    signal(SIGINT, signalHandler);

    std::cout << "Monitoring processes in /proc directory. Press Ctrl+C to exit.\n";

    workbook = workbook_new("process_history.xlsx");
    if (workbook == nullptr) {
        std::cerr << "Error creating XLSX workbook." << std::endl;
        return 1;
    }

    monitorProcesses();

    workbook_close(workbook);

    return 0;
}
