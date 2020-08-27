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
        self._batteryLevelMax = 13
        self._startTimeToFailure = 0
        self._waitTimeToReset = 5
        self._gameStage = GAME_STAGE.START

    def run_test_cycle(self):
        self.update(delay=0.15)
        
        if self._controllers['status_board'].registers[SB_REGISTER_INDEX.REG_SLAVE_COUNTDOWN_VALUE] < 3590:
            self._controllers['status_board'].setCountdownTime(3600)
            self._controllers['status_board'].addCommand(COMMAND.CMD_RESET_COUNTDOWN_TIME)

        if self._controllers['lasergrid'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.SOLVED:
            self._controllers['status_board'].addCommand(COMMAND.CMD_SET_LASER_GRID_SOLVED)
        
        # self._controllers['status_board'].addCommand(COMMAND.CMD_SET_DOCKED_SHIP_SOLVED)
        # self._controllers['status_board'].addCommand(COMMAND.CMD_SET_KEYPAD_SOLVED)
        # self._controllers['status_board'].addCommand(COMMAND.CMD_SET_LIFE_SUPPORT_SOLVED)


    def run(self):
        self.update(delay=0.15)

        if self._gameStage == GAME_STAGE.START:
            #Enabling Puzzle-01 (Power Control) if it's initialized
            if self._controllers['power_control'].registers[PC_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.INITIALIZED:
                self._controllers['power_control'].addCommand(COMMAND.CMD_ENABLE)
                        
            #Check if the battery level is full and then enable all other puzzles if they are initialized
            if self._controllers['power_control'].registers[PC_REGISTER_INDEX.REG_SLAVE_BATTERY] == self._batteryLevelMax:
                self.setSlaveStateCommandAllPuzzles(STATE.INITIALIZED, COMMAND.CMD_ENABLE)

            #Check if all enabled and then Change game stage to ongoing
            if self.checkAllSlaves(STATE.ENABLE):
                self._gameStage = GAME_STAGE.ONGOING
            
            # self._gameStage = GAME_STAGE.ONGOING

        if self._gameStage == GAME_STAGE.ONGOING:
            #Check the power state of the puzzles and calculate the demand
            demand = 0.0
            if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_POWER_SWITCH_STATE] == STATE.ON:
                demand += 4.5
            if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_POWER_SWITCH_STATE] == STATE.ON:
                demand += 3.5
            if self._controllers['datamatic'].registers[DM_REGISTER_INDEX.REG_SLAVE_POWER_SWITCH_STATE] == STATE.ON:
                demand += 1.5
            if self._controllers['lasergrid'].registers[DM_REGISTER_INDEX.REG_SLAVE_POWER_SWITCH_STATE] == STATE.ON:
                demand += 1.5
            if self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_POWER_SWITCH_STATE] == STATE.ON:
                demand += 1.5
            if self._controllers['life_support'].registers[PS_REGISTER_INDEX.REG_SLAVE_POWER_SWITCH_STATE] == STATE.ON:
                demand += 2.5
            if self._controllers['safeomatic'].registers[PS_REGISTER_INDEX.REG_SLAVE_POWER_SWITCH_STATE] == STATE.ON:
                demand += 1
            
            #Check the demand value and set to power control if changed
            demand = int(demand * 10)
            print("c demand: ", demand)
            print("r demand: ", self._controllers['power_control'].getDemand())
            print(int(self._controllers['power_control'].getDemand()))

            if demand != int(self._controllers['power_control'].getDemand()):
                self._controllers['power_control'].setDemand(demand)
                self._controllers['power_control'].addCommand(COMMAND.CMD_SET_DEMAND)

            #Check the BatteryMatrix, Generator and PowerAdjusment Solving State and set to PrepStatus
            if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_BATTERY_MATRIX_STATE] == STATE.SOLVED and \
                self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_BATTERY_MATRIX_STATE] != STATE.SOLVED:
                self._controllers['prep_status'].addCommand(COMMAND.CMD_SET_PS_BATTERY_MATRIX_SOLVED)

            if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_GENERATOR_STATE] == STATE.SOLVED and \
                self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_ENERGY_SUPP_STATE] != STATE.SOLVED:
                self._controllers['prep_status'].addCommand(COMMAND.CMD_SET_PS_GENERATOR_SOLVED)

            if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_POWER_ADJUSTER_STATE] == STATE.SOLVED and \
                self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_GENERATOR_STATE]  != STATE.SOLVED:
                self._controllers['prep_status'].addCommand(COMMAND.CMD_SET_PS_ENERGY_SUPP_SOLVED)

            #Check Docked Ship and Prep Status state and enable syncroKey if solved
            if self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_BATTERY_MATRIX_STATE]  == STATE.SOLVED  and \
                self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_ENERGY_SUPP_STATE]  == STATE.SOLVED and \
                self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_GENERATOR_STATE]  == STATE.SOLVED and \
                (self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_SYNCRO_READER_STATE] == STATE.ENABLE or \
                    self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_SYNCRO_READER_STATE] == STATE.ENABLE) :
                self._controllers['docked_ship'].addCommand(COMMAND.CMD_START_TIMER)
                self._controllers['prep_status'].addCommand(COMMAND.CMD_START_TIMER)

            if self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_SYNCRO_READER_STATE] == STATE.DONE and \
                self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_SYNCRO_READER_STATE] == STATE.DONE:
                if self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_SYNCRO_READER_INPUT_KEY] == 3 and \
                    self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_SYNCRO_READER_STATE] == 3:
                        self._controllers['docked_ship'].addCommand(COMMAND.CMD_SET_DS_SYNCRO_KEY_SOLVED)
                        self._controllers['prep_status'].addCommand(COMMAND.CMD_SET_PS_SYNCRO_KEY_SOLVED)
                elif self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_SYNCRO_READER_INPUT_KEY] == -1 and \
                    self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_SYNCRO_READER_STATE] == -1:
                        self._controllers['docked_ship'].addCommand(COMMAND.CMD_START_TIMER)
                        self._controllers['prep_status'].addCommand(COMMAND.CMD_START_TIMER)
                else:
                    self._controllers['docked_ship'].addCommand(COMMAND.CMD_SET_DS_SYNCRO_KEY_WRONG_SOLVED)
                    self._controllers['prep_status'].addCommand(COMMAND.CMD_SET_PS_SYNCRO_KEY_WRONG_SOLVED)


            #Check if the failure state in power control then disable all other puzzles if they are enable
            if self._controllers['power_control'].registers[PC_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.FAILURE:
                self._gameStage = GAME_STAGE.FAILURE

            #Update Status Board
            if self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.SOLVED:
                self._controllers['status_board'].addCommand(COMMAND.CMD_SET_DOCKED_SHIP_SOLVED)

            if self._controllers['lasergrid'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.SOLVED:
                self._controllers['status_board'].addCommand(COMMAND.CMD_SET_LASER_GRID_SOLVED)   

            if self._controllers['keypad'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.SOLVED:
                self._controllers['status_board'].addCommand(COMMAND.CMD_SET_KEYPAD_SOLVED)

            if self._controllers['life_support'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.SOLVED:
                self._controllers['status_board'].addCommand(COMMAND.CMD_SET_LIFE_SUPPORT_SOLVED)

            #Enable Keypad if Life support, ship prep and lasergrid is Solved

            if self._controllers['life_support'].registers[LS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.SOLVED and \
                self._controllers['lasergrid'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.SOLVED and \
                self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_STATE] == STATE.SOLVED and \
                self._controllers['keypad'].registers[KP_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.ENABLE:
                self._controllers['keypad'].addCommand(COMMAND.CMD_ENABLE)


        if self._gameStage == GAME_STAGE.FAILURE:
            self.setSlaveStateCommandAllPuzzles(STATE.ENABLE, COMMAND.CMD_DISABLE)
            self._controllers['power_control'].addCommand(COMMAND.CMD_RESET)
            time.sleep(5)
            self._gameStage = GAME_STAGE.RESET

        if self._gameStage == GAME_STAGE.RESET:
            self._controllers['power_control'].addCommand(COMMAND.CMD_RESET)
            self._controllers['docked_ship'].addCommand_ES(COMMAND.CMD_RESET)
            self._controllers['docked_ship'].addCommand_SP(COMMAND.CMD_RESET)
            self._controllers['datamatic'].addCommand(COMMAND.CMD_RESET)
            self._controllers['lasergrid'].addCommand(COMMAND.CMD_RESET)
            self._controllers['prep_status'].addCommand(COMMAND.CMD_RESET)
            self._controllers['life_support'].addCommand(COMMAND.CMD_RESET)
            self._controllers['safeomatic'].addCommand(COMMAND.CMD_RESET)
            self._controllers['status_board'].addCommand(COMMAND.CMD_RESET)
            self._controllers['keypad'].addCommand(COMMAND.CMD_RESET)
            self._gameStage = GAME_STAGE.START

    def update(self, delay):   
        for key_name in self._controllers.keys():

            if key_name not in ( 
                 'power_control', 
                 'datamatic', 
                 'docked_ship', 
                 'lasergrid',
                 'prep_status',
                 'safeomatic',
                 'life_support',
                 'keypad',
                 'laserbar',
                 'status_board'
                ):
                continue
            
            controller = self._controllers[key_name]
            registers = None
            readAgain = False
            for _ in range(1):
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
                readAgain = True
                print("COMMAND: ", controller.getCommand(), " to " , controller.getKeyName())
                for _ in range(1):
                    try: 
                        self._master.execute(controller.getSlaveID(), cst.WRITE_MULTIPLE_REGISTERS, 0, output_value=controller.registers)
                    except Exception as excpt:
                        print(controller.getKeyName(), end=" ")
                        LOGGER.debug("SystemDataCollector error: %s", str(excpt))
            
            if readAgain:
                registers = None
                for _ in range(1):
                    try: 
                        registers = list(self._master.execute(controller.getSlaveID(), cst.READ_HOLDING_REGISTERS, 0, controller.getNumberOfRegisters()))
                    except Exception as excpt:
                        print(controller.getKeyName(), end=" ")
                        LOGGER.debug("SystemDataCollector1 error: %s", str(excpt))
                
                if registers:
                    controller.setRegisters(registers)

            controller.refreshCommand()
            time.sleep(delay)
            
    def setSlaveStateCommandAllPuzzles(self, stateCondition, command): #except power control

        if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_STATE] == stateCondition:
            self._controllers['docked_ship'].addCommand_ES(command)

        if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_STATE] == stateCondition:
            self._controllers['docked_ship'].addCommand_SP(command)
        
        if self._controllers['datamatic'].registers[DM_REGISTER_INDEX.REG_SLAVE_STATE] == stateCondition:
            self._controllers['datamatic'].addCommand(command)

        if self._controllers['lasergrid'].registers[LG_REGISTER_INDEX.REG_SLAVE_STATE] == stateCondition:
            self._controllers['lasergrid'].addCommand(command)

        if self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] == stateCondition:
            self._controllers['prep_status'].addCommand(command)

        if self._controllers['life_support'].registers[LS_REGISTER_INDEX.REG_SLAVE_STATE] == stateCondition:
            self._controllers['life_support'].addCommand(command)

        if self._controllers['safeomatic'].registers[SC_REGISTER_INDEX.REG_SLAVE_STATE] == stateCondition:
            self._controllers['safeomatic'].addCommand(command)

        if self._controllers['status_board'].registers[SB_REGISTER_INDEX.REG_SLAVE_STATE] == stateCondition:
            self._controllers['status_board'].addCommand(command)

        if self._controllers['keypad'].registers[KP_REGISTER_INDEX.REG_SLAVE_STATE] == stateCondition:
            self._controllers['keypad'].addCommand(command)

    def checkAllSlaves(self, condition):     #except power control   
        if self._controllers['power_control'].registers[PC_REGISTER_INDEX.REG_SLAVE_STATE] == condition and \
            self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] == condition and \
            self._controllers['life_support'].registers[LS_REGISTER_INDEX.REG_SLAVE_STATE] == condition and \
            self._controllers['lasergrid'].registers[LG_REGISTER_INDEX.REG_SLAVE_STATE] == condition and \
            self._controllers['safeomatic'].registers[SC_REGISTER_INDEX.REG_SLAVE_STATE] == condition:
            # and \
            # self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_STATE] == condition and \
            # self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_STATE] == condition and \
            # self._controllers['datamatic'].registers[DM_REGISTER_INDEX.REG_SLAVE_STATE] == condition and \
            # self._controllers['status_board'].registers[SB_REGISTER_INDEX.REG_SLAVE_STATE] == condition and \
            # self._controllers['keypad'].registers[KP_REGISTER_INDEX.REG_SLAVE_STATE] == condition :
            return True
        return False



def cmp(a, b):
    return (a > b) - (a < b) 
