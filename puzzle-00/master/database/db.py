from datetime import datetime as dt
from peewee import *
import json

import os, sys
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import configuration as cfg

file_path = "".join((os.path.dirname(os.path.abspath(__file__)), "\\" if os.name == "nt" else "/", cfg.db['sqlite']['file']))

db = SqliteDatabase(file_path)

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