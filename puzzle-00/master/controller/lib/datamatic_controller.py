#!/usr/bin/etc python3
from .base_controller import BaseController
from .constants import STATE


class DatamaticController(BaseController):
    def __init__(self, key_name,model, puzzle):
        super().__init__(key_name, model, puzzle)


    def sayHello(self):
        print()
        print()
        
        #if self._puzzle['registers'][9] == 1 :
        #    print("enabled")
        #elif self._puzzle['registers'][9] == 2:
        #    print("disabled")
        #else:
        #    print("unknown")

    def update(self):
        if self._puzzle['registers'][1] == 1:
            print("check change to enables")
            self.setState(STATE.ENABLE)
        if self._puzzle['registers'][1] == 0:
            print("check change to disable")
            self.setState(STATE.DISABLE)
    
    


