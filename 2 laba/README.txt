For launch daemon controller
g++ daemon_control.cpp -o daemon_control
sudo ./daemon_control [start/stop] ./backupd
For compile the daemon
make
g++ config_creator.cpp
./a.out
For analyze the process
sudo ps aux | grep backupd
To kill process
sudo kill -9 [PID]
To change the permissions to file
sudo chmod -R 777 ..
Also to launch auto after Linux is launched
1.move to directory /etc/systemd/system/
2.make file backupd.service
3.write into file
[Unit]
Description=Backup Daemon
After=network.target
[Service]
ExecStart=/path/to/your/daemon/executable
Restart=always
User=your_username
[Install]
WantedBy=default.target
3.1.and change where it needed
4.enable start
sudo systemctl daemon-reload 
sudo systemctl enable backupd.service 
sudo systemctl start backupd.service
5.to check status 
sudo systemctl status backupd.service
