#!/usr/bin/etc python3
from .base_controller import BaseController
from .constants import STATE, COMMAND, PC_REGISTER_INDEX, STATUS
from .helpers import time_now


class PowerControlController(BaseController):
    def __init__(self, key_name, model, puzzle, master):
        super().__init__(key_name, model, puzzle, master)
        self._demand = 0
        self._lighEffectPatternNumber = 0
        self._baterryMaxValue = 7
        self._batteryChargingRate = 5
        self._failurePeriodValue = 3

    def update(self, registers):
        
        if self.getCommand() == COMMAND.CMD_SET_DEMAND and self.getCommandStatus() == STATUS.ST_CREATED:
            print("Command================== ", self.getCommand())
            registers[PC_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_SET_DEMAND
            registers[PC_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE
            registers[PC_REGISTER_INDEX.REG_SLAVE_DEMAND] = self._demand

        if self.getCommand() == COMMAND.CMD_ENABLE and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[PC_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_ENABLE
            registers[PC_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_PAUSE and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[PC_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_PAUSE
            registers[PC_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_RESET and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[PC_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_RESET
            registers[PC_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        if self.getCommand() == COMMAND.CMD_DISABLE and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[PC_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_DISABLE
            registers[PC_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE
            
        if self.getCommand() == COMMAND.CMD_SET_LIGHT_EFFECT_PATTERN_NUMBER and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[PC_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_SET_LIGHT_EFFECT_PATTERN_NUMBER
            registers[PC_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE
            registers[PC_REGISTER_INDEX.REG_SLAVE_LIGHT_EFFECT_PATTERN_NUMBER] = self._lighEffectPatternNumber

        if self.getCommand() == COMMAND.CMD_SET_BATTERY_MAX_VALUE and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[PC_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_SET_BATTERY_MAX_VALUE
            registers[PC_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE
            registers[PC_REGISTER_INDEX.REG_SLAVE_BATTERY_MAX_VALUE] = self._baterryMaxValue

        if self.getCommand() == COMMAND.CMD_SET_BATTERY_CHARGING_RATE and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[PC_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_SET_BATTERY_CHARGING_RATE
            registers[PC_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE
            registers[PC_REGISTER_INDEX.REG_SLAVE_BATTERY_CHARGING_RATE] = self._batteryChargingRate

        if self.getCommand() == COMMAND.CMD_SET_FAILURE_PERIOD_VALUE and self.getCommandStatus() == STATUS.ST_CREATED:
            registers[PC_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_SET_FAILURE_PERIOD_VALUE
            registers[PC_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE
            registers[PC_REGISTER_INDEX.REG_SLAVE_FAILURE_PERIOD_VALUE] = self._failurePeriodValue

        if  registers[PC_REGISTER_INDEX.REG_SLAVE_CONFIRM] == 0:
            registers[PC_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.DONE

        if self.getCommand() == registers[PC_REGISTER_INDEX.REG_MASTER_COMMAND]:
            if registers[PC_REGISTER_INDEX.REG_SLAVE_CONFIRM] == STATE.DONE:
                registers[PC_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_NONE
                self._command = COMMAND.CMD_NONE
                self._commandStatus = STATUS.ST_CONFIRMED
            
        self.setRegisters(registers)

    def setDemand(self, demand):
        self._demand = demand

    def getDemand(self):
        return self._demand
