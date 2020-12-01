#!/bin/bash 

DIR_PATH="/home/pi/proxima/puzzle-00-master/master"
FILE="$DIR_PATH/database/local.db"
if [ ! -f "$FILE"]; then
  cd "$DIR_PATH/database"
  python3 "main.py"
fi

