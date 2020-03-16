#!/usr/bin/etc python3
from .base_controller import BaseController
from .constants import STATE, COMMAND, KP_REGISTER_INDEX, STATUS
from .helpers import time_now
from enum import Enum

class KeypadController(BaseController):
    def __init__(self, key_name,model, puzzle):
        super().__init__(key_name,model, puzzle)

    def update(self, registers):

        if  registers[KP_REGISTER_INDEX.REG_SLAVE_CONFIRM] == 0:
            registers[KP_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.DONE

        if self.getCommand() == registers[KP_REGISTER_INDEX.REG_MASTER_COMMAND]:
            if registers[KP_REGISTER_INDEX.REG_SLAVE_CONFIRM] == STATE.DONE:
                registers[KP_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_NONE
                self._command = COMMAND.CMD_NONE
                self._commandStatus = STATUS.ST_CONFIRMED

        if self.getCommand() == COMMAND.CMD_ENABLE and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[KP_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_ENABLE
            registers[KP_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_DISABLE and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[KP_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_DISABLE
            registers[KP_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_RESET and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[KP_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_RESET
            registers[KP_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE


        print(self.getCommand())

        self.setRegisters(registers)

