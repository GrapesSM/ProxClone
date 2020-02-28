#!/usr/bin/etc python3

from modbus_tk.modbus import LOGGER
import modbus_tk.defines as cst
import time
import copy
from lib.constants import STATE, COMMAND, REGISTER_INDEX


class ProximaCommand(object):
    def __init__(self, controllers, master):
        self._controllers = controllers
        self._master = master
    def run(self):
        self.update(delay=1)
        
        #Game - Level Controlling part
        #Battery Demand Level
        demand = 0
        if(self._controllers['datamatic'].getState() == STATE.ENABLE):
            demand += 3
        
        self._controllers['power_control'].setDemand(demand)


        
        time.sleep(0.1)
    
    def update(self, delay):   
        for key_name in self._controllers.keys():

            if key_name not in ('datamatic','power_control',):
                continue
            
            controller = self._controllers[key_name]
            for _ in range(2):
                time.sleep(delay)
                try: 
                    slaveRegisters = list(self._master.execute(controller.getSlaveID(), cst.READ_HOLDING_REGISTERS, 0, controller.getNumberOfRegisters()))
                    break
                except Exception as excpt:
                    LOGGER.debug("SystemDataCollector error: %s", str(excpt))                    
            print(controller.getKeyName() ,"Slave Registers:  ", slaveRegisters)
            
            controller.update(slaveRegisters)
            
            if controller.getCommand() != COMMAND.CMD_NONE:
                try: 
                    (self._master.execute(controller.getSlaveID(), cst.WRITE_MULTIPLE_REGISTERS, 0, output_value=controller.getRegisters()))
                    break
                except Exception as excpt:
                    LOGGER.debug("SystemDataCollector error: %s", str(excpt))
                

    
def cmp(a, b):
    print("a > b", a > b)
    print("a < b", a < b)
    print("a == b", a == b)
    return (a > b) - (a < b) 
