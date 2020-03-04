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
        #Enabling the Puzzles if they are initialized
        if self._controllers['power_control'].registers[PC_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.INITIALIZED:
            self._controllers['power_control'].setCommand(COMMAND.CMD_ENABLE)

        if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_STATE] == STATE.INITIALIZED:
            self._controllers['docked_ship'].setCommand_ES(COMMAND.CMD_ENABLE)

        if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_STATE] == STATE.INITIALIZED:
            self._controllers['docked_ship'].setCommand_SP(COMMAND.CMD_ENABLE)
        
        if self._controllers['datamatic'].registers[DM_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.INITIALIZED:
            self._controllers['datamatic'].setCommand(COMMAND.CMD_ENABLE)

        if self._controllers['lasergrid'].registers[LG_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.INITIALIZED:
            self._controllers['lasergrid'].setCommand(COMMAND.CMD_ENABLE)

        if self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.INITIALIZED:
            self._controllers['prep_status'].setCommand(COMMAND.CMD_ENABLE)

        #Check the power state of the puzzles and calculate the demand
        demand = 0
        if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_POWER_SWITCH_STATE] == STATE.ON:
            demand += 4
        if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_POWER_SWITCH_STATE] == STATE.ON:
            demand += 3
        if self._controllers['datamatic'].registers[DM_REGISTER_INDEX.REG_SLAVE_POWER_SWITCH_STATE] == STATE.ON:
            demand += 1
        if self._controllers['lasergrid'].registers[DM_REGISTER_INDEX.REG_SLAVE_POWER_SWITCH_STATE] == STATE.ON:
            demand += 1
        if self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_POWER_SWITCH_STATE] == STATE.ON:
            demand += 1
        
        #Check the demand value and set to power control if changed
        if demand != self._controllers['power_control'].getDemand():
            self._controllers['power_control'].setDemand(demand)
            self._controllers['power_control'].setCommand(COMMAND.CMD_SET_DEMAND)

        #Check the BatteryMatirx, Generator and PowerAdjusment state Solvation State and set to PrepStatus
        if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_BATTERY_MATRIX_STATE] == STATE.SOLVED:
            self._controllers['prep_status'].setCommand(COMMAND.CMD_SET_BATERRY_MATRIX_SOLVED)
        
    
    def update(self, delay):   
        for key_name in self._controllers.keys():

            if key_name not in ( 
                # 'power_control', 
                # 'datamatic', 
                #'docked_ship', 
                # 'lasergrid',
                # 'prep_status',
                # 'safeomatic',
                ):
                continue
            
            controller = self._controllers[key_name]

            registers = None
            for _ in range(1):
                try: 
                    registers = list(self._master.execute(controller.getSlaveID(), cst.READ_HOLDING_REGISTERS, 0, controller.getNumberOfRegisters()))
                except Exception as excpt:
                    LOGGER.debug("SystemDataCollector1 error: %s", str(excpt))                    
            
            if registers:
                print(controller.getKeyName() ,"Slave Registers (received):  ", registers)
                controller.update(registers)
                print(controller.getKeyName() ,"Slave Registers (modified):  ", controller.registers)
            
            
            if controller.getCommand() != COMMAND.CMD_NONE:
                print("COMMAND: ", controller.getCommand())
                try: 
                    self._master.execute(controller.getSlaveID(), cst.WRITE_MULTIPLE_REGISTERS, 0, output_value=controller.registers)
                except Exception as excpt:
                    LOGGER.debug("SystemDataCollector error: %s", str(excpt))
            
            time.sleep(delay)

    
def cmp(a, b):
    print("a > b", a > b)
    print("a < b", a < b)
    print("a == b", a == b)
    return (a > b) - (a < b) 
