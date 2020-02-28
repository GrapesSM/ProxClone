#!/usr/bin/etc python3
from .constants import STATE
from .constants import STATE, COMMAND
from enum import Enum

class  REGISTER_INDEX(Enum):
    REG_MASTER_MILLIS = 0
    REG_MASTER_COMMAND = 1
    REG_MASTER_FORCE = 2
    REG_SLAVE_MILLIS = 3
    REG_SLAVE_CONFIRM = 4
    REG_SLAVE_STATE = 5


class BaseController:
    def __init__(self, key_name, model, puzzle):
        self._model=model
        self._puzzle=puzzle
        self._key_name = key_name
        self._command = COMMAND.CMD_NONE
        self._forced = False
    
    @property
    def registers(self):
        return self._puzzle['registers']

    def getState(self):
        return self._puzzle['registers'][REGISTER_INDEX.REG_SLAVE_STATE]

    def setState(self, state):
        self._puzzle['registers'][REGISTER_INDEX.REG_SLAVE_STATE] = state

    def getSlaveID(self):
        return self._puzzle['slave_id']

    def getKeyName(self):
        return self._key_name

    def getCommand(self):
        return self._command

    def setCommand(self, command):
        self._command = command

    def setRegisters(self, registers):
        self._puzzle.registers = registers

    def combineRegisters(self, registers):
        registers[1:3] = [self._command, self._forced]
        self._puzzle.registers = registers
    