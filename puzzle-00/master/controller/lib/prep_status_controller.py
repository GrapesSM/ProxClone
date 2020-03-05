#!/usr/bin/etc python3
from .base_controller import BaseController
from .constants import STATE, COMMAND, PS_REGISTER_INDEX, STATUS
from .helpers import time_now
from enum import Enum

class PrepStatusController(BaseController):
    def __init__(self, key_name, model, puzzle):
        super().__init__(key_name, model, puzzle)
        self.batteryMatrixState = STATE.DISABLE
        self.generatorState = STATE.DISABLE
        self.energySuppMatrixState = STATE.DISABLE

    def update(self, registers):
        # controller register vs slave register

        if self.getCommand() == registers[PS_REGISTER_INDEX.REG_MASTER_COMMAND]:
            if registers[PS_REGISTER_INDEX.REG_SLAVE_CONFIRM] == STATE.DONE:
                registers[PS_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_NONE
                self._command = COMMAND.CMD_NONE
                self._commandStatus = STATUS.ST_CONFIRMED

        if self.getCommand() == COMMAND.CMD_ENABLE and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[PS_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_ENABLE
            registers[PS_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE
        print(self.getCommand())

        if self.getCommand() == COMMAND.CMD_SET_BATTERY_MATRIX_SOLVED and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[PS_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_SET_BATTERY_MATRIX_SOLVED
            registers[PS_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_SET_ENERGY_SUPP_SOLVED and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[PS_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_SET_ENERGY_SUPP_SOLVED
            registers[PS_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_SET_GENERATOR_SOLVED and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[PS_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_SET_GENERATOR_SOLVED
            registers[PS_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        self.setRegisters(registers)