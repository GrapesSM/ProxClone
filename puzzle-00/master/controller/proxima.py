#!/usr/bin/etc python3

from modbus_tk.modbus import LOGGER
import modbus_tk.defines as cst
import time
import copy
from lib.constants import *


class ProximaCommand(object):
    def __init__(self, controllers, master):
        self._controllers = controllers
        self._master = master
        self._step = 0

    def run(self):
        self.update(delay=1)
        time.sleep(0.1)

        if self._controllers['power_control'].registers[DM_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.INITIALIZED:
            self._controllers['power_control'].setCommand(COMMAND.CMD_ENABLE)
        
        if self._controllers['datamatic'].registers[DM_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.INITIALIZED:
            self._controllers['datamatic'].setCommand(COMMAND.CMD_ENABLE)

        demand = 0
        if self._controllers['datamatic'].registers[DM_REGISTER_INDEX.REG_SLAVE_POWER_SWITCH_STATE] == STATE.ON:
            demand += 3
        
        
        self._controllers['power_control'].setDemand(demand)
        self._controllers['power_control'].setCommand(COMMAND.CMD_SET_DEMAND)
        
    
    def update(self, delay):   
        for key_name in self._controllers.keys():

            if key_name not in ('datamatic', 'power_control'):
                continue
            
            controller = self._controllers[key_name]

            registers = None
            for _ in range(2):
                time.sleep(2)
                try: 
                    registers = list(self._master.execute(controller.getSlaveID(), cst.READ_HOLDING_REGISTERS, 0, controller.getNumberOfRegisters()))
                except Exception as excpt:
                    LOGGER.debug("SystemDataCollector error: %s", str(excpt))                    
            
            if registers:
                print(controller.getKeyName() ,"Slave Registers (received):  ", registers)
                if  registers[REGISTER_INDEX.REG_SLAVE_CONFIRM] == 0:
                    registers[REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.DONE
                controller.update(registers)
                print(controller.getKeyName() ,"Slave Registers (modified):  ", controller.registers)
            
            
            if controller.getCommand() != COMMAND.CMD_NONE:
                print("COMMAND: ", controller.getCommand())
                try: 
                    self._master.execute(controller.getSlaveID(), cst.WRITE_MULTIPLE_REGISTERS, 0, output_value=controller.registers)
                except Exception as excpt:
                    LOGGER.debug("SystemDataCollector error: %s", str(excpt))
                

    
def cmp(a, b):
    print("a > b", a > b)
    print("a < b", a < b)
    print("a == b", a == b)
    return (a > b) - (a < b) 
