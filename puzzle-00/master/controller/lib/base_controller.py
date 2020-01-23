#!/usr/bin/etc python3

class BaseController:
    def __init__(self, model, puzzle):
        self._model=model
        self._puzzle=puzzle
    
    @property
    def registers(self):
        return tuple(self._puzzle['registers'])

    def sayHello(self):
        print("Hello Base Controller")
