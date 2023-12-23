#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>

class DaemonController {
public:
    DaemonController(const std::string& daemonName) : daemonName(daemonName) {}

    void startDaemon() {
        std::string startCommand = daemonName + " &";
        int result = std::system(startCommand.c_str());

        if (result == 0) {
            std::cout << "Started " << daemonName << " daemon." << std::endl;
        } else {
            std::cerr << "Error starting " << daemonName << " daemon." << std::endl;
        }
    }

    void stopDaemon() {
        std::string stopCommand = "kill -f " + daemonName;
        int result = std::system(stopCommand.c_str());

        if (result == 0) {
            std::cout << "Stopped " << daemonName << " daemon." << std::endl;
        } else {
            std::cerr << "Error stopping " << daemonName << " daemon." << std::endl;
        }
    }

    void restartDaemon() {
        stopDaemon();
        std::this_thread::sleep_for(std::chrono::seconds(2));  // Wait for the daemon to stop
        startDaemon();
    }

private:
    std::string daemonName;
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " [start|stop|restart] backupd" << std::endl;
        return 1;
    }

    std::string action = argv[1];
    std::string daemonName = argv[2];

    DaemonController controller(daemonName);

    if (action == "start") {
        controller.startDaemon();
    } else if (action == "stop") {
        controller.stopDaemon();
    } else if (action == "restart") {
        controller.restartDaemon();
    } else {
        std::cerr << "Invalid action. Use [start|stop|restart]." << std::endl;
        return 1;
    }

    return 0;
}
