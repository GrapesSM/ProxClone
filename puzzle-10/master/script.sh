#!/bin/bash 

echo "Hello World"

nohup python ./webapp.py &
nohup python ./controller.py &
nohup python ./communication.py &