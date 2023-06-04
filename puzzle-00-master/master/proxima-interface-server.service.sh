#!/bin/bash 

DIR_PATH="/home/pi/proxima/puzzle-00-master/master"
cd "$DIR_PATH/interface/"
export FLASK_APP=app.py
flask run -h 0.0.0.0 -p 5000
