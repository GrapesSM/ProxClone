import datetime
from peewee import *

db = SqliteDatabase("example.db")

class BaseModel(Model):
    class Meta:
        database = db

class User(BaseModel):
    username = CharField(unique=True)
    
def initialize_db():
    db.connect()
    db.create_tables([User], safe=True)
    db.close()

if __name__ == "__main__":
    initialize_db()