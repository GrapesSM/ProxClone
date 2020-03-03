#!/usr/bin/etc python3
from .base_controller import BaseController
from .constants import STATE, COMMAND, PS_REGISTER_INDEX
from .helpers import time_now
from enum import Enum

class PrepStatusController(BaseController):
    def __init__(self, key_name, model, puzzle):
        super().__init__(key_name, model, puzzle)

    def update(self, registers):
        # controller register vs slave register

        if self.getCommand() == registers[PS_REGISTER_INDEX.REG_MASTER_COMMAND]:
            if registers[PS_REGISTER_INDEX.REG_SLAVE_CONFIRM] == STATE.DONE:
                registers[PS_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_NONE
                self._command = COMMAND.CMD_NONE
                self._commandStatus = COMMAND.STATUS_CONFIRMED

        if self.getCommand() == COMMAND.CMD_ENABLE and self.getCommandStatus() == COMMAND.STATUS_CREATED:
            registers[PS_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_ENABLE
            registers[PS_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_SET_BATERRY_MATRIX_SOLVED and self.getCommandStatus() == COMMAND.STATUS_CREATED:
            registers[PS_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_SET_BATERRY_MATRIX_SOLVED
            registers[PS_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE
            registers[PS_REGISTER_INDEX.REG_SLAVE_BATTERY_MATRIX_STATE] = STATE.SOLVED
        
        self.setRegisters(registers)