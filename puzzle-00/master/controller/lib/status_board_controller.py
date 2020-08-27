#!/usr/bin/etc python3
from .base_controller import BaseController
from .constants import STATE, COMMAND, SB_REGISTER_INDEX, STATUS
from .helpers import time_now
from enum import Enum

class StatusBoardController(BaseController):
    def __init__(self, key_name, model, puzzle):
        super().__init__(key_name, model, puzzle)
        self._countdownTime = 3600

    def setCountdownTime(self, value):
        self._countdownTime = value

    def update(self, registers):
        if  registers[SB_REGISTER_INDEX.REG_SLAVE_CONFIRM] == 0:
            registers[SB_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.DONE

        if self.getCommand() == COMMAND.CMD_ENABLE and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[SB_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_ENABLE
            registers[SB_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_DISABLE and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[SB_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_DISABLE
            registers[SB_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_RESET and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[SB_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_RESET
            registers[SB_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_SET_DOCKED_SHIP_SOLVED and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[SB_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_SET_DOCKED_SHIP_SOLVED
            registers[SB_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_SET_LASER_GRID_SOLVED and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[SB_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_SET_LASER_GRID_SOLVED
            registers[SB_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_SET_KEYPAD_SOLVED and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[SB_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_SET_KEYPAD_SOLVED
            registers[SB_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_SET_LIFE_SUPPORT_SOLVED and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[SB_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_SET_LIFE_SUPPORT_SOLVED
            registers[SB_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE
        
        if self.getCommand() == COMMAND.CMD_RESET_COUNTDOWN_TIME and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[SB_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_RESET_COUNTDOWN_TIME
            registers[SB_REGISTER_INDEX.REG_SLAVE_COUNTDOWN] = self._countdownTime
            registers[SB_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE            
            
        self.setRegisters(registers)