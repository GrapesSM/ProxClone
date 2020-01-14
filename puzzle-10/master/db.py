from datetime import datetime as dt
from peewee import *
import config as cfg
import json

db = SqliteDatabase(cfg.db['sqlite']['file_path'])

class BaseModel(Model):
    class Meta:
        database = db

class User(BaseModel):
    username = CharField(unique=True)
    password = CharField()
    activated = BooleanField(default=False)
    disabled = BooleanField(default=False)
    updated_at = TimestampField(default=dt.now)
    created_at = TimestampField(constraints=[SQL('DEFAULT CURRENT_TIMESTAMP')])

class Puzzle(BaseModel):
    name = CharField(null=True)
    description = TextField(null=True)
    key_name = CharField(unique=True)
    state = TextField(null=True)
    disabled = BooleanField(constraints=[SQL('DEFAULT FALSE')])
    updated_at = TimestampField(default=dt.now)
    created_at = TimestampField(constraints=[SQL('DEFAULT CURRENT_TIMESTAMP')])
    
def initialize_db():
    db.connect()
    db.create_tables([Puzzle, User], safe=True)
    
    # Create user record of administrator
    admin = User(username=cfg.db['admin']['username'], password=cfg.db['admin']['password'], activated=True)
    admin.save()

    # Create records of puzzles
    for key_name in cfg.puzzles.keys():
        puzzle = Puzzle(key_name=key_name, state=json.dumps(cfg.puzzles[key_name]))
        puzzle.save()

    db.close()

if __name__ == "__main__":
    initialize_db()