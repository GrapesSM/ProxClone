#!/usr/bin/etc python3
from .base_controller import BaseController
from .constants import STATE

class PowerPanelController(BaseController):
    def __init__(self, key_name, model, puzzle):
        super().__init__(key_name, model, puzzle)
        self._demand = 0

    def update(self):
        return

    def setDemand(self, value):
        if(self._demand != value):
            self._demand = value
            self._puzzle['registers'][0] = 1
            self._puzzle['registers'][13] = 5 
            print("allliii :" , self._puzzle['registers'])
        return