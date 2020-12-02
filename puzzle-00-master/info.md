
# Overview

Master puzzle has three components such as controller, interface, and database

# Controller Component
### Initialization
- read configurations
- check connection with SQLite database
- create ModBus connection
- check ModBus communication with every puzzle
### Control
- read states of all puzzles
- change states of puzzles depending on requirements
- write new states of puzzle to puzzles depending on requirements
- save new states of puzzles to the database
### Status
- notice initialization 
- notice states of puzzles
- notice problems/errors
- notice connections
### Error
- log everything whether happening on all puzzles or not
- log everything whether functioning on all puzzles or not

# Interface Component
### Initialization
- read configurations
- check database connection
### Running
- read database
- read requests
- authenticate/authorize user
- respond to requests
- change information in database

# Database Component
### Initialization
- read configurations
- create & initialize SQLite database
### ORM module
- provide models(connection between python object and table in database) for tables

# Tools
- [SQLite Browser for Windows](https://sqlitebrowser.org/dl/#windows)

# Proxima Controller Service Script
#### sudo nano /etc/systemd/system/proxima-controller.service
```
[Unit]
Description=Proxima Controller (Master) Code
After=network.target

[Service]
ExecStart=/home/pi/proxima/puzzle-00-master/master/proxima-controller.service.sh
WorkingDirectory=/home/pi/proxima/puzzle-00-master/master/
StandardOutput=inherit
StandardError=inherit
Restart=always
User=pi

[Install]
WantedBy=multi-user.target
```


# Proxima Database Service Script
#### sudo nano /etc/systemd/system/proxima-database.service
```
[Unit]
Description=Proxima Database (Master) Code
After=network.target

[Service]
ExecStart=/home/pi/proxima/puzzle-00-master/master/proxima-database.service.sh
WorkingDirectory=/home/pi/proxima/puzzle-00-master/master/
StandardOutput=inherit
StandardError=inherit
Restart=always
User=pi

[Install]
WantedBy=multi-user.target
```

# Proxima Interface Client Service Script
#### sudo nano /etc/systemd/system/proxima-interface-client.service
```
[Unit]
Description=Proxima Controller (Master) Code
After=network.target

[Service]
ExecStart=/home/pi/proxima/puzzle-00-master/master/proxima-interface-client.service.sh
WorkingDirectory=/home/pi/proxima/puzzle-00-master/master/
StandardOutput=inherit
StandardError=inherit
Restart=always
User=pi

[Install]
WantedBy=multi-user.target
```
# Proxima Interface Server Service Script
#### sudo nano /etc/systemd/system/proxima-interface-server.service
```
[Unit]
Description=Proxima Controller (Master) Code
After=network.target

[Service]
ExecStart=/home/pi/proxima/puzzle-00-master/master/proxima-interface-server.service.sh
WorkingDirectory=/home/pi/proxima/puzzle-00-master/master/
StandardOutput=inherit
StandardError=inherit
Restart=always
User=pi

[Install]
WantedBy=multi-user.target
```