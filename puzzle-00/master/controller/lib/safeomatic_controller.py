#!/usr/bin/etc python3
from .base_controller import BaseController
from .constants import STATE, COMMAND, SC_REGISTER_INDEX, STATUS
from .helpers import time_now

class SafeomaticController(BaseController):
    def __init__(self, key_name, model, puzzle, master):
        super().__init__(key_name, model, puzzle, master)

    def update(self, registers):
        # controller register vs slave register
        if  registers[SC_REGISTER_INDEX.REG_SLAVE_CONFIRM] == 0:
            registers[SC_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.DONE

        if self.getCommand() == registers[SC_REGISTER_INDEX.REG_MASTER_COMMAND]:
            if registers[SC_REGISTER_INDEX.REG_SLAVE_CONFIRM] == STATE.DONE:
                registers[SC_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_NONE
                self._command = COMMAND.CMD_NONE
                self._commandStatus = STATUS.ST_CONFIRMED

        if self.getCommand() == COMMAND.CMD_ENABLE and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[SC_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_ENABLE
            registers[SC_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_RESET and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[SC_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_RESET
            registers[SC_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_DISABLE and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[SC_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_DISABLE
            registers[SC_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_PAUSE and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[SC_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_PAUSE
            registers[SC_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_SET_SOLVED and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[SC_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_SET_SOLVED
            registers[SC_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        self.setRegisters(registers)