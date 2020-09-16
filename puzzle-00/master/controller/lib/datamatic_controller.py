#!/usr/bin/etc python3
from .base_controller import BaseController
from .constants import STATE, COMMAND, DM_REGISTER_INDEX, STATUS
from .helpers import time_now
from enum import Enum


class DatamaticController(BaseController):
    
    def __init__(self, key_name,model, puzzle, master):
        super().__init__(key_name, model, puzzle, master)
        self._lighEffectPatternNumber = 0
        self._datamaticKey1 = 12345
        self._datamaticKey2 = 23456
        self._datamaticKey3 = 34567
        self._datamaticKey4 = 45678
        self._datamaticKey5 = 56789

    def update(self, registers):
        # controller register vs slave register
        if  registers[DM_REGISTER_INDEX.REG_SLAVE_CONFIRM] == 0:
            registers[DM_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.DONE

        if self.getCommand() == registers[DM_REGISTER_INDEX.REG_MASTER_COMMAND]:
            if registers[DM_REGISTER_INDEX.REG_SLAVE_CONFIRM] == STATE.DONE:
                registers[DM_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_NONE
                self._command = COMMAND.CMD_NONE
                self._commandStatus = STATUS.ST_CONFIRMED

        if self.getCommand() == COMMAND.CMD_ENABLE and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[DM_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_ENABLE
            registers[DM_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_RESET and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[DM_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_RESET
            registers[DM_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_DISABLE and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[DM_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_DISABLE
            registers[DM_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_PAUSE and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[DM_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_PAUSE
            registers[DM_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_SET_LIGHT_EFFECT_PATTERN_NUMBER and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[DM_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_SET_LIGHT_EFFECT_PATTERN_NUMBER
            registers[DM_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE
            registers[DM_REGISTER_INDEX.REG_SLAVE_LIGHT_EFFECT_PATTERN_NUMBER] = self._lighEffectPatternNumber

        if self.getCommand() == COMMAND.CMD_SET_DATAMATIC_KEY_1 and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[DM_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_SET_DATAMATIC_KEY_1
            registers[DM_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE
            registers[DM_REGISTER_INDEX.REG_SLAVE_KEY_1] = self._datamaticKey1

        if self.getCommand() == COMMAND.CMD_SET_DATAMATIC_KEY_2 and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[DM_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_SET_DATAMATIC_KEY_2
            registers[DM_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE
            registers[DM_REGISTER_INDEX.REG_SLAVE_KEY_2] = self._datamaticKey2

        if self.getCommand() == COMMAND.CMD_SET_DATAMATIC_KEY_3 and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[DM_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_SET_DATAMATIC_KEY_3
            registers[DM_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE
            registers[DM_REGISTER_INDEX.REG_SLAVE_KEY_3] = self._datamaticKey3

        if self.getCommand() == COMMAND.CMD_SET_DATAMATIC_KEY_4 and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[DM_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_SET_DATAMATIC_KEY_4
            registers[DM_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE
            registers[DM_REGISTER_INDEX.REG_SLAVE_KEY_4] = self._datamaticKey4

        if self.getCommand() == COMMAND.CMD_SET_DATAMATIC_KEY_5 and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[DM_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_SET_DATAMATIC_KEY_5
            registers[DM_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE
            registers[DM_REGISTER_INDEX.REG_SLAVE_KEY_5] = self._datamaticKey5

        
        self.setRegisters(registers)
