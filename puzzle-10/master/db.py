from datetime import datetime as dt
from peewee import *
import config as cfg

db = SqliteDatabase(cfg.db['sqlite']['file_path'])

class BaseModel(Model):
    class Meta:
        database = db

class User(BaseModel):
    username = CharField(unique=True)
    password = CharField()
    activated = BooleanField(default=True)
    disabled = BooleanField(default=False)
    updated_at = TimestampField(default=dt.now)
    created_at = TimestampField(constraints=[SQL('DEFAULT CURRENT_TIMESTAMP')])

class Puzzle(BaseModel):
    name = CharField()
    description = TextField()
    state = TextField()
    updated_at = TimestampField(default=dt.now)
    created_at = TimestampField(constraints=[SQL('DEFAULT CURRENT_TIMESTAMP')])
    
def initialize_db():
    db.connect()
    db.create_tables([Puzzle, User], safe=True)
    db.close()

if __name__ == "__main__":
    initialize_db()