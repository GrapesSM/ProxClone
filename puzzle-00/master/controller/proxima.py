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
        self.update(delay=0)
        #Enabling the Puzzles if they are initialized
        if self._controllers['power_control'].registers[PC_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.INITIALIZED:
            self._controllers['power_control'].addCommand(COMMAND.CMD_ENABLE)

        if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_STATE] == STATE.INITIALIZED:
            self._controllers['docked_ship'].addCommand_ES(COMMAND.CMD_ENABLE)

        if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_STATE] == STATE.INITIALIZED:
            self._controllers['docked_ship'].addCommand_SP(COMMAND.CMD_ENABLE)
        
        if self._controllers['datamatic'].registers[DM_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.INITIALIZED:
            self._controllers['datamatic'].addCommand(COMMAND.CMD_ENABLE)

        if self._controllers['lasergrid'].registers[LG_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.INITIALIZED:
            self._controllers['lasergrid'].addCommand(COMMAND.CMD_ENABLE)

        if self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.INITIALIZED:
            self._controllers['prep_status'].addCommand(COMMAND.CMD_ENABLE)

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
        print("c demand: ", demand)
        print("r demand: ", self._controllers['power_control'].getDemand())

        if demand != int(self._controllers['power_control'].getDemand()):
            print("WHYY")

            self._controllers['power_control'].setDemand(demand)
            self._controllers['power_control'].addCommand(COMMAND.CMD_SET_DEMAND)

        #Check the BatteryMatirx, Generator and PowerAdjusment state Solvation State and set to PrepStatus
        if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_BATTERY_MATRIX_STATE] == STATE.SOLVED and self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_BATTERY_MATRIX_STATE] != STATE.SOLVED:
            self._controllers['prep_status'].addCommand(COMMAND.CMD_SET_BATTERY_MATRIX_SOLVED)

        if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_GENERATOR_STATE] == STATE.SOLVED and self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_ENERGY_SUPP_STATE] != STATE.SOLVED:
            self._controllers['prep_status'].addCommand(COMMAND.CMD_SET_GENERATOR_SOLVED)

        if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_POWER_ADJUSTER_STATE] == STATE.SOLVED and self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_GENERATOR_STATE]  != STATE.SOLVED:
            self._controllers['prep_status'].addCommand(COMMAND.CMD_SET_ENERGY_SUPP_SOLVED)
        
    def update(self, delay):   
        for key_name in self._controllers.keys():

            if key_name not in ( 
                 'power_control', 
                 'datamatic', 
                 'docked_ship', 
                 'lasergrid',
                # 'prep_status',
                # 'safeomatic',
                ):
                continue
            
            controller = self._controllers[key_name]
            registers = None
            for _ in range(2):
                try: 
                    registers = list(self._master.execute(controller.getSlaveID(), cst.READ_HOLDING_REGISTERS, 0, controller.getNumberOfRegisters()))
                except Exception as excpt:
                    print(controller.getKeyName(), end=" ")
                    LOGGER.debug("SystemDataCollector1 error: %s", str(excpt))                    
            
            if registers:
                print(controller.getKeyName() ,"Slave Registers (received):  ", registers)
                controller.update(registers)
                print(controller.getKeyName() ,"Slave Registers (modified):  ", controller.registers)
            
            
            if controller.getCommand() != COMMAND.CMD_NONE:
                print("COMMAND: ", controller.getCommand(), " to " , controller.getKeyName())
                try: 
                    self._master.execute(controller.getSlaveID(), cst.WRITE_MULTIPLE_REGISTERS, 0, output_value=controller.registers)
                except Exception as excpt:
                    print(controller.getKeyName(), end=" ")
                    LOGGER.debug("SystemDataCollector error: %s", str(excpt))

            controller.refreshCommand()
            time.sleep(delay)
            
    
def cmp(a, b):
    print("a > b", a > b)
    print("a < b", a < b)
    print("a == b", a == b)
    return (a > b) - (a < b) 
