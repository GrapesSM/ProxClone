#!/usr/bin/etc python3
from .constants import STATE
from .constants import STATE, COMMAND, REGISTER_INDEX

class BaseController:
    def __init__(self, key_name, model, puzzle):
        self._model=model
        self._puzzle=puzzle
        self._key_name = key_name
        self._command = COMMAND.CMD_NONE
        self._commandStatus = COMMAND.STATUS_NONE
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

    def getCommandStatus(self):
        return self._commandStatus

    def setCommand(self, command):
        self._command = command
        self._commandStatus = COMMAND.STATUS_CREATED

    def setRegisters(self, registers):
        self._puzzle['registers'] = registers

    def getNumberOfRegisters(self):
        return len(self._puzzle['registers'])
        