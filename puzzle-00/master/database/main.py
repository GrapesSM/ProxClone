
# from config import config as cfg
from db import *

import os, sys
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import configuration as cfg

def initialize_db():
    db.connect()
    db.create_tables([Puzzle, User], safe=True)
    
    # Create user record of administrator
    admin = User(username=cfg.db['admin']['username'], password=cfg.db['admin']['password'], activated=True)
    admin.save()

    # Create records of puzzles
    for key_name in cfg.puzzles.keys():
        puzzle = Puzzle(name=cfg.puzzles[key_name]['name'], key_name=key_name, state=json.dumps(cfg.puzzles[key_name]))
        puzzle.save()

    db.close()

if __name__ == "__main__":
    initialize_db()
