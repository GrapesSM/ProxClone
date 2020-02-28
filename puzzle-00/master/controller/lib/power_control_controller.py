#!/usr/bin/etc python3
from .base_controller import BaseController
from .constants import STATE, COMMAND

class PowerControlController(BaseController):
    def __init__(self, key_name, model, puzzle):
        super().__init__(key_name, model, puzzle)
        self._demand = 0

    def update(self, registers):
        if controller.getCommand() != COMMAND.CMD_NONE:
            if slaveRegisters[REGISTER_INDEX.REG_SLAVE_CONFIRM] == STATE.DONE:
                controller.setCommand(COMMAND.CMD_NONE)
        
        if controller.getCommand() != COMMAND.CMD_NONE:
            controller.combineRegisters(slaveRegisters)
        else:
            controller.setRegisters(slaveRegisters)


    def setDemand(self, demand):
        if(self._demand != demand):
            self._demand = demand
            self.setCommand(COMMAND.CMD_SET_DEMAND)
        return

    def combineRegisters(self, registers):
        if (self._command == CMD_SET_DEMAND):
            registers[PC_REGISTER_INDEX.REG_SLAVE_DEMAND] = self._demand

        super().combineRegisters(registers)


