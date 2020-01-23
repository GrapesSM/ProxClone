#!/usr/bin/etc python3

from modbus_tk.defines import HOLDING_REGISTERS
from modbus_tk import modbus_rtu

from modbus_tk.utils import to_data, PY2, PY3

class BaseController:
    def __init__(self, model, puzzle):
        self._model=model
        self._puzzle=puzzle
    
    @property
    def registers(self):
        return tuple(self._puzzle['registers'])

    def sayHello(self):
        print("Hello Base Controller")
