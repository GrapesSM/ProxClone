#!/usr/bin/etc python3
from .base_controller import BaseController
from .constants import STATE, COMMAND, PS_REGISTER_INDEX, STATUS
from .helpers import time_now
from enum import Enum

class PrepStatusController(BaseController):
    def __init__(self, key_name, model, puzzle):
        super().__init__(key_name, model, puzzle)
        self._lighEffectPatternNumber = 0

    def update(self, registers):
        # controller register vs slave register
        if  registers[PS_REGISTER_INDEX.REG_SLAVE_CONFIRM] == 0:
            registers[PS_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.DONE

        if self.getCommand() == registers[PS_REGISTER_INDEX.REG_MASTER_COMMAND]:
            if registers[PS_REGISTER_INDEX.REG_SLAVE_CONFIRM] == STATE.DONE:
                registers[PS_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_NONE
                self._command = COMMAND.CMD_NONE
                self._commandStatus = STATUS.ST_CONFIRMED

        if self.getCommand() == COMMAND.CMD_ENABLE and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[PS_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_ENABLE
            registers[PS_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_RESET and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[PS_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_RESET
            registers[PS_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_PAUSE and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[PS_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_PAUSE
            registers[PS_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_DISABLE and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[PS_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_DISABLE
            registers[PS_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_SET_PS_BATTERY_MATRIX_SOLVED and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[PS_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_SET_PS_BATTERY_MATRIX_SOLVED
            registers[PS_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_SET_PS_ENERGY_SUPP_SOLVED and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[PS_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_SET_PS_ENERGY_SUPP_SOLVED
            registers[PS_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_SET_PS_GENERATOR_SOLVED and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[PS_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_SET_PS_GENERATOR_SOLVED
            registers[PS_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_SET_PS_SYNCRO_KEY_SOLVED and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[PS_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_SET_PS_SYNCRO_KEY_SOLVED
            registers[PS_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_SET_PS_SYNCRO_KEY_WRONG_SOLVED and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[PS_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_SET_PS_SYNCRO_KEY_WRONG_SOLVED
            registers[PS_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_ENABLE_PS_SYNCRO_KEY and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[PS_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_ENABLE_PS_SYNCRO_KEY
            registers[PS_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_START_TIMER and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[PS_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_START_TIMER
            registers[PS_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_SET_LIGHT_EFFECT_PATTERN_NUMBER and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[PS_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_SET_LIGHT_EFFECT_PATTERN_NUMBER
            registers[PS_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE
            registers[PS_REGISTER_INDEX.REG_SLAVE_LIGHT_EFFECT_PATTERN_NUMBER] = self._lighEffectPatternNumber


        self.setRegisters(registers)