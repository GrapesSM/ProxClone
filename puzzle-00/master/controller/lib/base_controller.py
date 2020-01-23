#!/usr/bin/etc python3

class BaseController:
    def __init__(self, model, puzzle):
        self._model=model
        self._puzzle=puzzle
    
    @property
    def registers(self):
        return tuple(self._puzzle['registers'])

    def sayHello(self):
        for i in range(len(self._puzzle['registers'])):
            self._puzzle['registers'][i] = 1
