#!/usr/bin/etc python3
from modbus_tk.modbus import LOGGER
import modbus_tk.defines as cst
import time
import json
from .constants import STATE, COMMAND, REGISTER_INDEX, STATUS
from database.db import Puzzle

class BaseController:
    busy = False
    
    def __init__(self, key_name, model, puzzle, master):
        self._model=model
        self._puzzle=puzzle
        self._key_name = key_name
        self._master = master
        self._writeCommand = None
        self._writeCommand_ES = None
        self._writeCommand_SP = None
        self._failedCommand = None
        self._failedCount = 0
        self._failedCommand_ES = None
        self._failedCount_ES = 0
        self._failedCommand_SP = None
        self._failedCount_SP = 0
        self._forced = False
        self._lastRefresh = 0
        self._currentRefresh = 0
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

    def refresh(self, delay):
        registers = None
        try:
            registers = self.read()
        except Exception as excpt:
            _ = ""

        if registers:
            self.setRegisters(registers)
            # print(self.getKeyName(), registers)
            if self._failedCommand:
                self._failedCount = self._failedCount + 1
                if self.write(self._failedCommand[0], self._failedCommand[1]):
                    self._failedCommand = None
                elif self._failedCount > 5:
                    self._failedCount = 0
                    self._failedCommand = None
            
            if self._failedCommand_ES:
                self._failedCount_ES = self._failedCount_ES + 1
                if self.write(self._failedCommand_ES[0], self._failedCommand_ES[1]):
                    self._failedCommand_ES = None
                elif self._failedCount_ES > 5:
                    self._failedCount_ES = 0
                    self._failedCommand_ES = None
            
            if self._failedCommand_SP:
                self._failedCount_SP = self._failedCount_SP + 1
                if self.write(self._failedCommand_SP[0], self._failedCommand_SP[1]):
                    self._failedCommand_SP = None
                elif self._failedCount_SP > 5:
                    self._failedCount_SP = 0
                    self._failedCommand_SP = None
            
        if registers == None:
            print(self.getKeyName())
        

    def setRegisters(self, registers):
        for i in range(len(registers)):
            self._puzzle['registers'][i] = registers[i]
        
        self._timer['current'] = int(round(time.time() * 1000))
        if (self._timer['current'] - self._timer['point']) > self._timer['interval']:
            self._timer['point'] = int(round(time.time() * 1000))
            Puzzle.update(state=json.dumps(self._puzzle))\
                .where(Puzzle.id == self._model.id)\
                .execute()
        

    def getNumberOfRegisters(self):
        return len(self._puzzle['registers'])

    def read(self):
        registers = None
        try:                
            registers = list(self._master.execute(self.getSlaveID(), cst.READ_HOLDING_REGISTERS, 0, self.getNumberOfRegisters()))
        except Exception as excpt:
            registers = None
            _ = ""

        return registers

    def write(self, command, body = 0):

        if self._failedCommand:
            return

        registers = self.registers
        ok = False
        try:
            registers[REGISTER_INDEX.REG_MASTER_COMMAND] = command            
            registers[REGISTER_INDEX.REG_MASTER_BODY] = body
            registers[REGISTER_INDEX.REG_SLAVE_CONFIRM] = 0
            self._master.execute(self.getSlaveID(), cst.WRITE_MULTIPLE_REGISTERS, 0, output_value=registers[0:5])
            # self._master.execute(self.getSlaveID(), cst.WRITE_MULTIPLE_REGISTERS, 4, output_value=registers[4:5])
            ok = True
        except Exception as excpt:
            self._failedCommand = [command, body]
            _ = ""

        return ok


        