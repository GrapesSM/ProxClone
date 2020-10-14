#!/usr/bin/etc python3
from modbus_tk.modbus import LOGGER
import modbus_tk.defines as cst
from .base_controller import BaseController
from .constants import STATE, COMMAND, DS_REGISTER_INDEX, STATUS
from .helpers import time_now
from enum import Enum
import time

class DockedShipController(BaseController):
    def __init__(self, key_name, model, puzzle, master):
        super().__init__(key_name, model, puzzle, master)

    def write_ES(self, command, body = 0, delay=0.08):
        registers = None
        ok = False
        try:                
            registers = list(self.read())
            registers[DS_REGISTER_INDEX.REG_MASTER_ES_COMMAND] = command
            registers[DS_REGISTER_INDEX.REG_MASTER_ES_BODY] = body
            registers[DS_REGISTER_INDEX.REG_SLAVE_ES_CONFIRM] = 0
            if delay > 0: 
                time.sleep(delay)
            self._master.execute(self.getSlaveID(), cst.WRITE_MULTIPLE_REGISTERS, 11, output_value=self.registers[10:13])
            ok = True
        except Exception as excpt:
            self._failedCommand_ES = [command, body]
            _ = ""

        return ok
    
    def write_SP(self, command, body = 0, delay=0.08):
        registers = None
        ok = False
        try:                
            registers = list(self.read())
            registers[DS_REGISTER_INDEX.REG_MASTER_SP_COMMAND] = command
            registers[DS_REGISTER_INDEX.REG_MASTER_SP_BODY] = body
            registers[DS_REGISTER_INDEX.REG_SLAVE_SP_CONFIRM] = 0
            if delay > 0: 
                time.sleep(delay)
            self._master.execute(self.getSlaveID(), cst.WRITE_MULTIPLE_REGISTERS, 0, output_value=self.registers[0:5])
            ok = True
        except Exception as excpt:
            self._failedCommand_SP = [command, body]
            _ = ""

        return ok