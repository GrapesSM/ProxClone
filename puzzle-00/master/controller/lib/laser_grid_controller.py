#!/usr/bin/etc python3
from .base_controller import BaseController
from .constants import STATE, COMMAND, LG_REGISTER_INDEX, STATUS
from .helpers import time_now
from enum import Enum

class LaserGridController(BaseController):
    def __init__(self, key_name, model, puzzle):
        super().__init__(key_name, model, puzzle)

    def update(self, registers):
        # controller register vs slave register
        if  registers[LG_REGISTER_INDEX.REG_SLAVE_CONFIRM] == 0:
            registers[LG_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.DONE

        if self.getCommand() == registers[LG_REGISTER_INDEX.REG_MASTER_COMMAND]:
            if registers[LG_REGISTER_INDEX.REG_SLAVE_CONFIRM] == STATE.DONE:
                registers[LG_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_NONE
                self._command = COMMAND.CMD_NONE
                self._commandStatus = STATUS.ST_CONFIRMED

        if self.getCommand() == COMMAND.CMD_ENABLE and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[LG_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_ENABLE
            registers[LG_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_RESET and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[LG_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_RESET
            registers[LG_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_DISABLE and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[LG_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_DISABLE
            registers[LG_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE
        
        self.setRegisters(registers)