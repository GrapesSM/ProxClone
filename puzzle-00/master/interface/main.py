#!/usr/bin/etc python3
import os, sys
from flask import Flask
from flask import render_template, request, redirect, url_for
from flask import send_from_directory
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import configuration as cfg
from database.db import *

app = Flask(__name__, 
            static_url_path='', 
            static_folder='static', 
            template_folder='templates') 

@app.route('/')
def index():
    puzzles = Puzzle.select().where(Puzzle.disabled == False).dicts()
    return render_template('index.html', puzzles=puzzles)

@app.route('/p/<int:id>')
def puzzle(id):
    puzzle = Puzzle.get(Puzzle.id == id)
    puzzles = Puzzle.select().where(Puzzle.disabled == False).dicts()
    return render_template('puzzle.html', puzzles=puzzles, puzzle=puzzle)

@app.route('/update', methods=['POST'])
def update():
    #color = request.form['color'].lstrip('#')
    #colorRGB = tuple(int(color[i:i+2], 16) for i in (0, 2, 4))
    return redirect(url_for('index'))

@app.errorhandler(404)
def not_found(error):
    return '404 Not Found', 404

if __name__ == "__main__":
    app.run(host=cfg.server['host'], port=cfg.server['port'], debug=cfg.server['debug'])