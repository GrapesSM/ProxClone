#!/bin/bash 

echo "Hello World"

nohup python ./main.py &
# nohup python ./controller.py &
nohup python ./communication.py &