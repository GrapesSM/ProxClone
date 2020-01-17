#!/bin/bash 

echo "Hello, Proxima Command"

nohup python ./database/main.py &
nohup python ./interface/main.py &
nohup python ./controller/main.py &
