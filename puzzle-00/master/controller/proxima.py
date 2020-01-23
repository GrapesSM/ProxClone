#!/usr/bin/etc python3

from modbus_tk.modbus import LOGGER
import modbus_tk.defines as cst
import time

class ProximaCommand(object):
    def __init__(self, controllers, master):
        self._controllers = controllers
        self._master = master
        self._max_count = 5 * 10
        self._count = self._max_count - 1

    def run(self):
        try:
            # TO-DO:
            self._count += 1
            if self._count >= self._max_count:
                self._count = 0
                time.sleep(1)
                
                self.update()
                for sid in self._controllers.keys():
                    self._controllers[sid].sayHello()

        except Exception as excpt:
            LOGGER.debug("SystemDataCollector error: %s", str(excpt))
        time.sleep(0.1)
    
    def update(self):
        for sid in self._controllers.keys():
            if sid != 10:
                continue
            controller = self._controllers[sid]
            number_of_registers = len(controller.registers)
            slave_registers = self._master.execute(sid, cst.READ_HOLDING_REGISTERS, 0, number_of_registers)
            if slave_registers == controller.registers:
                continue
            
            # TO-DO: Check registers to be set and updated
    
def cmp(a, b):
    print("a > b", a > b)
    print("a < b", a < b)
    print("a == b", a == b)
    return (a > b) - (a < b) 
