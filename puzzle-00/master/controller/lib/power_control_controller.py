#!/usr/bin/etc python3
from .base_controller import BaseController
from .constants import STATE, COMMAND, PC_REGISTER_INDEX, STATUS
from .helpers import time_now


class PowerControlController(BaseController):
    def __init__(self, key_name, model, puzzle):
        super().__init__(key_name, model, puzzle)
        self._demand = 0

    def update(self, registers):
        # controller register vs slave register
        if  registers[PC_REGISTER_INDEX.REG_SLAVE_CONFIRM] == 0:
            registers[PC_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.DONE

        if self.getCommand() == registers[PC_REGISTER_INDEX.REG_MASTER_COMMAND]:
            if registers[PC_REGISTER_INDEX.REG_SLAVE_CONFIRM] == STATE.DONE:
                registers[PC_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_NONE
                self._command = COMMAND.CMD_NONE
                self._commandStatus = STATUS.ST_CONFIRMED

        if self.getCommand() == COMMAND.CMD_SET_DEMAND and self.getCommandStatus() == STATUS.ST_CREATED:
            print("fdfafd")
            registers[PC_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_SET_DEMAND
            registers[PC_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE
            registers[PC_REGISTER_INDEX.REG_SLAVE_DEMAND] = self._demand

        if self.getCommand() == COMMAND.CMD_ENABLE and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[PC_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_ENABLE
            registers[PC_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        self.setRegisters(registers)

    def setDemand(self, demand):
        self._demand = demand

    def getDemand(self):
        return self._demand
