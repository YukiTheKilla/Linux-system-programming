For launch daemon controller
g++ daemon_control.cpp -o daemon_control
sudo ./daemon_control [start/stop] ./backupd
For compile the daemon
make
For analyze the process
sudo ps aux | grep backupd
To kill process
sudo kill -9 [PID]
To change the permissions to file
sudo chmod -R 777 ..
