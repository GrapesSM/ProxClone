#!/usr/bin/etc python3
from modbus_tk.modbus import LOGGER
import modbus_tk.defines as cst
import time
import json
from .constants import STATE
from .constants import STATE, COMMAND, REGISTER_INDEX, STATUS
from queue import *
from database.db import Puzzle

class BaseController:
    busy = False
    
    def __init__(self, key_name, model, puzzle, master):
        self._model=model
        self._puzzle=puzzle
        self._key_name = key_name
        self._master = master
        self._failedCommand = None
        self._failedCommand_ES = None
        self._failedCommand_SP = None
        self._forced = False
        self._timer = {
            'current': 0,
            'point': 0,
            'interval': 5000 #milliseconds
        }

    @property
    def registers(self):
        return self._puzzle['registers']
    
    @property
    def model(self):
        return self._model

    def getSlaveID(self):
        return self._puzzle['slave_id']

    def getKeyName(self):
        return self._key_name

    def refresh(self, delay, stop):
        while True:
            if stop():
                break

            if self.busy:
                continue

            registers = None
            try:
                self.busy = True 
                registers = self.read(delay)
                self.busy = False
            except Exception as excpt:
                self.busy = False
            
            if registers:
                self.setRegisters(registers)               
            
            if self._failedCommand:
                self.busy = True
                if self.write(self._failedCommand[0], self._failedCommand[1]):
                    self._failedCommand = None
                self.busy = False
            
            if self._failedCommand_ES:
                self.busy = True
                if self.write(self._failedCommand_ES[0], self._failedCommand_ES[1]):
                    self._failedCommand_ES = None
                self.busy = False

            if self._failedCommand_SP:
                self.busy = True
                if self.write(self._failedCommand_SP[0], self._failedCommand_SP[1]):
                    self._failedCommand_SP = None
                self.busy = False


    def setRegisters(self, registers):
        for i in range(len(self._puzzle['registers'])):
            self._puzzle['registers'][i] = registers[i]
        
        self._timer['current'] = int(round(time.time() * 1000))
        if (self._timer['current'] - self._timer['point']) > self._timer['interval']:
            self._timer['point'] = int(round(time.time() * 1000))
            Puzzle.update(state=json.dumps(self._puzzle))\
                .where(Puzzle.id == self._model.id)\
                .execute()

    def getNumberOfRegisters(self):
        return len(self._puzzle['registers'])

    def read(self, delay=0.08):
        registers = None
        try:                
            registers = list(self._master.execute(self.getSlaveID(), cst.READ_HOLDING_REGISTERS, 0, self.getNumberOfRegisters()))
            if delay > 0: 
                time.sleep(delay)
        except Exception as excpt:
            _ = ""

        return registers

    def write(self, command, body = 0, delay=0.08):
        registers = None
        ok = False
        try:
            registers = list(self.read())
            registers[REGISTER_INDEX.REG_MASTER_COMMAND] = command
            registers[REGISTER_INDEX.REG_MASTER_BODY] = body
            registers[REGISTER_INDEX.REG_SLAVE_CONFIRM] = 0
            if delay > 0: 
                time.sleep(delay)
            self._master.execute(self.getSlaveID(), cst.WRITE_MULTIPLE_REGISTERS, 0, output_value=self.registers[0:5])
            ok = True
        except Exception as excpt:
            self._failedCommand = [command, body]
            _ = ""

        return ok

        