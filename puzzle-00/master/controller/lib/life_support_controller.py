#!/usr/bin/etc python3
from .base_controller import BaseController
from .constants import STATE, COMMAND, LS_REGISTER_INDEX, STATUS
from .helpers import time_now
from enum import Enum

class LifeSupportController(BaseController):
    def __init__(self, key_name, model, puzzle, master):
        super().__init__(key_name, model, puzzle, master)
        self._lighEffectPatternNumber = 0

    def update(self, registers):

        if  registers[LS_REGISTER_INDEX.REG_SLAVE_CONFIRM] == 0:
            registers[LS_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.DONE

        if self.getCommand() == registers[LS_REGISTER_INDEX.REG_MASTER_COMMAND]:
            if registers[LS_REGISTER_INDEX.REG_SLAVE_CONFIRM] == STATE.DONE:
                registers[LS_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_NONE
                self._command = COMMAND.CMD_NONE
                self._commandStatus = STATUS.ST_CONFIRMED

        if self.getCommand() == COMMAND.CMD_ENABLE and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[LS_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_ENABLE
            registers[LS_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_RESET and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[LS_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_RESET
            registers[LS_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_DISABLE and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[LS_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_DISABLE
            registers[LS_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_PAUSE and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[LS_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_PAUSE
            registers[LS_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_SET_LIGHT_EFFECT_PATTERN_NUMBER and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[LS_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_SET_LIGHT_EFFECT_PATTERN_NUMBER
            registers[LS_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE
            registers[LS_REGISTER_INDEX.REG_SLAVE_LIGHT_EFFECT_PATTERN_NUMBER] = self._lighEffectPatternNumber

            

        self.setRegisters(registers)
