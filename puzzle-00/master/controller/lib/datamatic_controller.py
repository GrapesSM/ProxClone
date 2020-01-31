#!/usr/bin/etc python3
from .base_controller import BaseController

class DatamaticController(BaseController):
    def __init__(self, model, puzzle):
        super().__init__(model, puzzle)


    def sayHello(self):
        print()
        print()

        if self._puzzle['registers'][9] == 1 :
            print("enabled")
        elif self._puzzle['registers'][9] == 2:
            print("disabled")
        else:
            print("unknown")
    


