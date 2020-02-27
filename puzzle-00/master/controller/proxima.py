#!/usr/bin/etc python3

from modbus_tk.modbus import LOGGER
import modbus_tk.defines as cst
import time
import copy
from lib.constants import STATE




class ProximaCommand(object):
    def __init__(self, controllers, master):
        self._controllers = controllers
        self._master = master
    def run(self):
        self.update(delay=1)
        self._controllers['datamatic'].update()
        self._controllers['power_panel'].update()

        #Game - Level Controlling part
        #Battery Demand Level
        demand = 0
        if(self._controllers['datamatic'].getState() == STATE.ENABLE):
            demand += 3
        
        self._controllers['power_panel'].setDemand(demand)

        
        time.sleep(0.1)
    
    def update(self, delay):   
        # TO-DO: Check registers to be copied and changed
        for key_name in self._controllers.keys():

            if key_name not in ('datamatic','power_panel',):
                continue
            
            controller = self._controllers[key_name]
            number_of_registers = len(controller.registers)
            controllerRegisters = controller.registers
            for _ in range(2):
                time.sleep(delay)
                try: 
                    slaveRegisters = list(self._master.execute(controller.getSlaveID(), cst.READ_HOLDING_REGISTERS, 0, number_of_registers))
                    break
                except Exception as excpt:
                    LOGGER.debug("SystemDataCollector error: %s", str(excpt))                    
            print(controller.getKeyName() ,"Slave Registers:  ", slaveRegisters)
            
            #0 -> change check , 1 -> power state, 2to5 -> controller data, 6to9 -> slave data

            # Check slave registers whether are changed or not
            if  slaveRegisters[0] == 1: #check if changed
                print(controller.getKeyName(), " Slave Changed")
                slavePart = copy.deepcopy(slaveRegisters[5:number_of_registers]) 
                controllerRegisters[5:number_of_registers] = slavePart
                controllerRegisters[1] = slaveRegisters[1]
                print(controller.getKeyName(), "Controller Registers" , str(controller.registers))
                #controller.setRegisters(controllerRegisters)
                slaveRegisters[0] = 0
                try: 
                    LOGGER.debug(self._master.execute(controller.getSlaveID(), cst.WRITE_MULTIPLE_REGISTERS, 0, output_value=slaveRegisters))
                    break
                except Exception as excpt:
                    LOGGER.debug("SystemDataCollector error: %s", str(excpt))
                
                



            # Check controller registers whether are changed or not
            if  controllerRegisters[0] == 1: #check if changed
                print(controller.getKeyName(), " Controller Register Changed")
                controllerPart = copy.deepcopy(slaveRegisters[2:5]) 
                slaveRegisters[2:5] = controllerPart

                #Write the change to slave
                try: 
                    LOGGER.debug(self._master.execute(controller.getSlaveID(), cst.WRITE_MULTIPLE_REGISTERS, 0, output_value=slaveRegisters))
                    break
                except Exception as excpt:
                    LOGGER.debug("SystemDataCollector error: %s", str(excpt))
                
                #Read the slave
                for _ in range(2):
                    time.sleep(delay)
                    try: 
                        slaveRegisters = list(self._master.execute(controller.getSlaveID(), cst.READ_HOLDING_REGISTERS, 0, number_of_registers))
                        break
                    except Exception as excpt:
                        LOGGER.debug("SystemDataCollector error: %s", str(excpt)) 

                if slaveRegisters[2:5] ==  controllerRegisters[2:5]:
                    controllerRegisters[0] = 0
                    print(controller.getKeyName(), "Controller Registers:" , str(controller.registers))
                else: print("FAILED!")
            
"""
            # Write to slaves's registers if change is happened
            for _ in range(2):
                time.sleep(delay)
                try: 
                    LOGGER.debug(self._master.execute(sid, cst.WRITE_MULTIPLE_REGISTERS, 0, output_value=controller.registers))
                    break
                except Exception as excpt:
                    LOGGER.debug("SystemDataCollector error: %s", str(excpt))
"""

    
def cmp(a, b):
    print("a > b", a > b)
    print("a < b", a < b)
    print("a == b", a == b)
    return (a > b) - (a < b) 
