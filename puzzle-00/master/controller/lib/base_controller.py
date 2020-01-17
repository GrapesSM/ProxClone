#!/usr/bin/etc python3

class BaseController:
    def __init__(self, model, slave):
        self.model=model
        self.slave=slave
