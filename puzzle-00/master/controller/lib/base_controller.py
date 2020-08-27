#!/usr/bin/etc python3
from .constants import STATE
from .constants import STATE, COMMAND, REGISTER_INDEX, STATUS
from queue import *


class BaseController:
    def __init__(self, key_name, model, puzzle):
        self._model=model
        self._puzzle=puzzle
        self._key_name = key_name
        self._command = COMMAND.CMD_NONE
        self._commandQueue = []
        self._commandStatus = STATUS.ST_NONE
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

    def addCommand(self, command):
        if command not in self._commandQueue:
            self._commandQueue.append(command)

    def refreshCommand(self):
        if self._command == COMMAND.CMD_NONE:
            if len(self._commandQueue) != 0:
                self._command = self._commandQueue.pop(0)
                self._commandStatus = STATUS.ST_CREATED
        else:
            if self._puzzle['registers'][REGISTER_INDEX.REG_SLAVE_CONFIRM] == STATE.DONE:
                self._command = COMMAND.CMD_NONE
                self._commandStatus = STATUS.ST_CREATED



    def getCommandStatus(self):
        return self._commandStatus

    def setCommand(self, command):
        self._command = command
        self._commandStatus = STATUS.ST_CREATED

    def setRegisters(self, registers):
        for i in range(len(self._puzzle['registers'])):
            self._puzzle['registers'][i] = registers[i]

    def getNumberOfRegisters(self):
        return len(self._puzzle['registers'])
        