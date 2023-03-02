from flask import *
  
from data import *
import os, sys, json
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from database.db import *


app = Flask(__name__)
false = False
null = None


puzzles = []
for puzzle in Puzzle.select().dicts():
    puzzle['state'] = json.loads(puzzle['state'])
    puzzles.append(puzzle)


@app.route('/')
def index():
    # create a list to hold the name and registers data
    data = []
    for d in puzzles:
        # extract the name and registers values from each dictionary
        if d["key_name"] == "rcp":
          continue
        name = d['name']
        key_name = d['key_name']
        registers = d['state']['registers']
        # add the name and registers data to the list
        data.append((name, key_name, registers))
    # pass the name and registers data to the HTML template
    return render_template('index.html', data=data, puzzle_registers_meta=puzzle_registers_meta, puzzle_states=puzzle_states)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)