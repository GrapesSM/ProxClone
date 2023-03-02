#!/bin/bash
FILE=/home/pi/proxima/logs
if [ -f "$FILE" ]; then
    echo > /home/pi/proxima/logs
fi


