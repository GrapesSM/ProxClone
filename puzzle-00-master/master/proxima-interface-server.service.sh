#!/bin/bash 

DIR_PATH="/home/pi/proxima/puzzle-00-master/master"
cd "$DIR_PATH/interface/server/"
export FLASK_APP=app.py
flask run
