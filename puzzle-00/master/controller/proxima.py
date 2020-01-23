#!/usr/bin/etc python3

from modbus_tk.modbus import LOGGER
import modbus_tk.defines as cst
import time

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

            if sid not in (1, 4, 10):
                continue
            
            controller = self._controllers[sid]
            number_of_registers = len(controller.registers)
            
            for _ in range(2):
                time.sleep(delay)
                try: 
                    LOGGER.debug(self._master.execute(sid, cst.READ_HOLDING_REGISTERS, 0, number_of_registers))
                    break
                except Exception as excpt:
                    LOGGER.debug("SystemDataCollector error: %s", str(excpt))
            
            # Check registers whether are changed or not
            # if slave_registers == controller.registers:
            #     continue

            # Write to slaves's registers if change is happened
            for _ in range(2):
                time.sleep(delay)
                try: 
                    LOGGER.debug(self._master.execute(sid, cst.WRITE_MULTIPLE_REGISTERS, 0, output_value=controller.registers))
                    break
                except Exception as excpt:
                    LOGGER.debug("SystemDataCollector error: %s", str(excpt))
            
    
def cmp(a, b):
    print("a > b", a > b)
    print("a < b", a < b)
    print("a == b", a == b)
    return (a > b) - (a < b) 
