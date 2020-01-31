#!/usr/bin/etc python3

from modbus_tk.modbus import LOGGER
import modbus_tk.defines as cst
import time
import copy

class ProximaCommand(object):
    def __init__(self, controllers, master):
        self._controllers = controllers
        self._master = master
    def run(self):
        self.update(delay=1)
        for sid in self._controllers.keys():
            self._controllers[sid].sayHello()
        time.sleep(0.1)
    
    def update(self, delay):         
        # TO-DO: Check registers to be copied and changed
        for sid in self._controllers.keys():

            if sid not in (10,):
                continue
            
            controller = self._controllers[sid]
            number_of_registers = len(controller.registers)
            controllerRegisters = controller.registers

            for _ in range(2):
                time.sleep(delay)
                try: 
                    slaveRegisters = list(self._master.execute(sid, cst.READ_HOLDING_REGISTERS, 0, number_of_registers))
                    break
                except Exception as excpt:
                    LOGGER.debug("SystemDataCollector error: %s", str(excpt))                    
            print("recived= ", slaveRegisters)
            
            #0 -> change check , 1 -> power state, 2to5 -> controller data, 6to9 -> slave data

            # Check slave registers whether are changed or not
            if  slaveRegisters[0] == 1: #check if changed
                slavePart = copy.deepcopy(slaveRegisters[6:10]) 
                controllerRegisters[6:10] = slavePart
                print("aaaaaaaaaaaaa = " + str(controller.registers))
                slaveRegisters[0] = 0
                try: 
                    LOGGER.debug(self._master.execute(sid, cst.WRITE_MULTIPLE_REGISTERS, 0, output_value=slaveRegisters))
                    break
                except Exception as excpt:
                    LOGGER.debug("SystemDataCollector error: %s", str(excpt))
                 
            # Check controller registers whether are changed or not
            if  controllerRegisters[0] == 1: #check if changed
                controllerPart = copy.deepcopy(slaveRegisters[2:6]) 
                slaveRegisters[2:6] = controllerPart
                #Write the change to slave
                try: 
                    LOGGER.debug(self._master.execute(sid, cst.WRITE_MULTIPLE_REGISTERS, 0, output_value=slaveRegisters))
                    break
                except Exception as excpt:
                    LOGGER.debug("SystemDataCollector error: %s", str(excpt))
                
                #Read the slave
                for _ in range(2):
                    time.sleep(delay)
                    try: 
                        slaveRegisters = list(self._master.execute(sid, cst.READ_HOLDING_REGISTERS, 0, number_of_registers))
                        break
                    except Exception as excpt:
                        LOGGER.debug("SystemDataCollector error: %s", str(excpt)) 

                if slaveRegisters[2:6] ==  controllerRegisters[2:6]:
                    controllerRegisters[0] = 1
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
