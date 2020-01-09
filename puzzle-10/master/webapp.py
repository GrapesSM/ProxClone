#!/usr/bin/etc python3
from flask import Flask
from flask import render_template, request, redirect, url_for
from flask import send_from_directory
import config as cfg
from db import *

app = Flask(__name__, 
            static_url_path='', 
            static_folder='static',
            template_folder='templates')

@app.route('/')
def index():
    users = User.select()
    # [user.username for user in query]
    return render_template('index.html', users=users)

@app.route('/update', methods=['POST'])
def update():
    #color = request.form['color'].lstrip('#')
    #colorRGB = tuple(int(color[i:i+2], 16) for i in (0, 2, 4))
    return redirect(url_for('index'))

@app.errorhandler(404)
def not_found(error):
    return '404 Not Found', 404