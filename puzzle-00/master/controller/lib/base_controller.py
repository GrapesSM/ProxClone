#!/usr/bin/etc python3
from .constants import STATE

class BaseController:
    def __init__(self, key_name, model, puzzle):
        self._model=model
        self._puzzle=puzzle
        self._state= STATE.NODATA
        self._key_name = key_name
    
    @property
    def registers(self):
        return self._puzzle['registers']
    
    def sayHello(self):
        for i in range(len(self._puzzle['registers'])):
            self._puzzle['registers'][i] = 0

    def getState(self):
        return self._state

    def setState(self, state):
        self._state = state

    def getSlaveID(self):
        return self._puzzle['slave_id']

    def getKeyName(self):
        return self._key_name



    