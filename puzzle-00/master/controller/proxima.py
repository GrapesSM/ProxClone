#!/usr/bin/etc python3

from modbus_tk.modbus import LOGGER
import modbus_tk.defines as cst
import time
import copy
from lib.helpers import *
from lib.constants import *
from threading import Thread

class ProximaCommand(object):
    def __init__(self, controllers, master):
        self._controllers = controllers
        self._master = master
        self._step = 0
        self._batteryLevelMax = 13
        self._batteryLevelMin = 0
        self._batteryFull = False
        self._batteryEmpty = False
        self._startTimeToFailure = 0
        self._waitTimeToReset = 5
        self._startSyncroReaderTimer = 0
        self._gameStage = GAME_STAGE.NULL
        self._flag = False
        self._dsSyncroPoint = 0
        self._psSyncroPoint = 0

    def run(self):
        time.sleep(1)

        if self._controllers['status_board'].registers[SB_REGISTER_INDEX.REG_SLAVE_GAME_POWER_SWITCH_STATE] == STATE.ON:
            self._gameStage = GAME_STAGE.START 

        if self._controllers['status_board'].registers[SB_REGISTER_INDEX.REG_SLAVE_GAME_POWER_SWITCH_STATE] == STATE.RESET:
            self._gameStage = GAME_STAGE.RESET
        
        if self._controllers['status_board'].registers[SB_REGISTER_INDEX.REG_SLAVE_GAME_POWER_SWITCH_STATE] == STATE.OFF:
            self._gameStage = GAME_STAGE.PAUSE 

        if self._gameStage == GAME_STAGE.START:
            self._controllers['status_board'].addCommand(COMMAND.CMD_ENABLE)
            self._controllers['power_control'].addCommand(COMMAND.CMD_ENABLE)

        if self._gameStage == GAME_STAGE.RESET:    
            self._controllers['power_control'].addCommand(COMMAND.CMD_RESET)
            self._controllers['datamatic'].addCommand(COMMAND.CMD_RESET)
            self._controllers['safeomatic'].addCommand(COMMAND.CMD_RESET)
            self._controllers['life_support'].addCommand(COMMAND.CMD_RESET)
            self._controllers['docked_ship'].addCommand_SP(COMMAND.CMD_RESET)
            self._controllers['docked_ship'].addCommand_ES(COMMAND.CMD_RESET)
            self._controllers['prep_status'].addCommand(COMMAND.CMD_RESET)
            self._controllers['lasergrid'].addCommand(COMMAND.CMD_RESET)
            self._controllers['laserbar'].addCommand(COMMAND.CMD_RESET)
            self._controllers['status_board'].addCommand(COMMAND.CMD_RESET)
        
        if self._gameStage == GAME_STAGE.PAUSE: 
            self._controllers['power_control'].addCommand(COMMAND.CMD_DISABLE)
            self._controllers['datamatic'].addCommand(COMMAND.CMD_DISABLE)
            self._controllers['safeomatic'].addCommand(COMMAND.CMD_DISABLE)
            self._controllers['life_support'].addCommand(COMMAND.CMD_DISABLE)
            self._controllers['docked_ship'].addCommand_SP(COMMAND.CMD_DISABLE)
            self._controllers['docked_ship'].addCommand_ES(COMMAND.CMD_DISABLE)
            self._controllers['prep_status'].addCommand(COMMAND.CMD_DISABLE)
            self._controllers['lasergrid'].addCommand(COMMAND.CMD_DISABLE)
            self._controllers['laserbar'].addCommand(COMMAND.CMD_DISABLE)
            self._controllers['status_board'].addCommand(COMMAND.CMD_DISABLE)

        if self._gameStage == GAME_STAGE.START \
           and self._controllers['power_control'].registers[PC_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.ENABLE \
           and self._controllers['status_board'].registers[SB_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.ENABLE:
            self._gameStage = GAME_STAGE.ONGOING             

        if self._gameStage == GAME_STAGE.ONGOING:
            self._controlPowerControl()

        # if self._gameStage == GAME_STAGE.ONGOING:
        #     self._controlDockedShip()

        # if self._gameStage == GAME_STAGE.ONGOING:
        #     self._controlSafeomatic()

        # if self._gameStage == GAME_STAGE.ONGOING:
        #     self._controlLifeSupport()
        
        # if self._gameStage == GAME_STAGE.ONGOING:
        #     self._controlDatamatic()
        
        # if self._gameStage == GAME_STAGE.ONGOING:
        #     self._controlLaserGridAndBar()

        # if self._gameStage == GAME_STAGE.ONGOING:
        #     self._controlKeypad()
        
        # if self._gameStage == GAME_STAGE.ONGOING:
        #     self._controlStatusBoard()

        # #Update Status Board
        # if self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.SOLVED:
        #     self._controllers['status_board'].addCommand(COMMAND.CMD_SET_DOCKED_SHIP_SOLVED)

        # if self._controllers['lasergrid'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.SOLVED:
        #     self._controllers['status_board'].addCommand(COMMAND.CMD_SET_LASER_GRID_SOLVED)   

        # if self._controllers['keypad'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.SOLVED:
        #     self._controllers['status_board'].addCommand(COMMAND.CMD_SET_KEYPAD_SOLVED)

        # if self._controllers['life_support'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.SOLVED:
        #     self._controllers['status_board'].addCommand(COMMAND.CMD_SET_LIFE_SUPPORT_SOLVED)


    def checkAllSlaves(self, condition):
        if self._controllers['power_control'].registers[PC_REGISTER_INDEX.REG_SLAVE_STATE] == condition and \
            self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] == condition and \
            self._controllers['life_support'].registers[LS_REGISTER_INDEX.REG_SLAVE_STATE] == condition and \
            self._controllers['lasergrid'].registers[LG_REGISTER_INDEX.REG_SLAVE_STATE] == condition and \
            self._controllers['safeomatic'].registers[SC_REGISTER_INDEX.REG_SLAVE_STATE] == condition and \
            self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_STATE] == condition and \
            self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_STATE] == condition and \
            self._controllers['datamatic'].registers[DM_REGISTER_INDEX.REG_SLAVE_STATE] == condition and \
            self._controllers['status_board'].registers[SB_REGISTER_INDEX.REG_SLAVE_STATE] == condition and \
            self._controllers['keypad'].registers[KP_REGISTER_INDEX.REG_SLAVE_STATE] == condition :
            return True
        return False

    def _controlPowerControl(self):
                        
        #Check if the battery level is full and then enable all other puzzles if they are initialized
        
        if self._controllers['power_control'].registers[PC_REGISTER_INDEX.REG_SLAVE_BATTERY] == self._batteryLevelMin \
            and self._batteryFull:
            self._batteryFull = False
            self._batteryEmpty = True

        if self._batteryEmpty:
            self._controllers['datamatic'].addCommand(COMMAND.CMD_DISABLE)
            self._controllers['safeomatic'].addCommand(COMMAND.CMD_DISABLE)
            self._controllers['life_support'].addCommand(COMMAND.CMD_DISABLE)
            self._controllers['docked_ship'].addCommand_SP(COMMAND.CMD_DISABLE)
            self._controllers['docked_ship'].addCommand_ES(COMMAND.CMD_DISABLE)
            self._controllers['prep_status'].addCommand(COMMAND.CMD_DISABLE)
            self._controllers['lasergrid'].addCommand(COMMAND.CMD_DISABLE)
            self._controllers['laserbar'].addCommand(COMMAND.CMD_DISABLE)
            self._controllers['keypad'].addCommand(COMMAND.CMD_DISABLE)
        
        if self._controllers['power_control'].registers[PC_REGISTER_INDEX.REG_SLAVE_LIGHT_EFFECT_STATE] == STATE.FAILURE:
            self._batteryFull = False
            self._controllers['datamatic'].addCommand(COMMAND.CMD_DISABLE)
            self._controllers['safeomatic'].addCommand(COMMAND.CMD_DISABLE)
            self._controllers['life_support'].addCommand(COMMAND.CMD_DISABLE)
            self._controllers['docked_ship'].addCommand_SP(COMMAND.CMD_DISABLE)
            self._controllers['docked_ship'].addCommand_ES(COMMAND.CMD_DISABLE)
            self._controllers['prep_status'].addCommand(COMMAND.CMD_DISABLE)
            self._controllers['lasergrid'].addCommand(COMMAND.CMD_DISABLE)
            self._controllers['laserbar'].addCommand(COMMAND.CMD_DISABLE)
            self._controllers['keypad'].addCommand(COMMAND.CMD_DISABLE)
        else:
            if self._controllers['power_control'].registers[PC_REGISTER_INDEX.REG_SLAVE_BATTERY] == self._batteryLevelMax \
                and self._batteryFull == False:
                self._batteryFull = True
                self._batteryEmpty = False
                self._controllers['datamatic'].addCommand(COMMAND.CMD_ENABLE)
                self._controllers['safeomatic'].addCommand(COMMAND.CMD_ENABLE)
                self._controllers['life_support'].addCommand(COMMAND.CMD_ENABLE)
                self._controllers['docked_ship'].addCommand_SP(COMMAND.CMD_ENABLE)
                self._controllers['docked_ship'].addCommand_ES(COMMAND.CMD_ENABLE)
                self._controllers['prep_status'].addCommand(COMMAND.CMD_ENABLE)
                self._controllers['lasergrid'].addCommand(COMMAND.CMD_ENABLE)
                self._controllers['laserbar'].addCommand(COMMAND.CMD_ENABLE)
                self._controllers['keypad'].addCommand(COMMAND.CMD_ENABLE)


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
        if demand !=  self._controllers['power_control'].registers[PC_REGISTER_INDEX.REG_SLAVE_DEMAND]:
            self._controllers['power_control'].setDemand(demand)
            self._controllers['power_control'].addCommand(COMMAND.CMD_SET_DEMAND)

    def _controlDatamatic(self):
        print("Control Datamatic")
    
    def _controlSafeomatic(self):
        print("Control Safeomatic")
    
    def _controlLaserGridAndBar(self):
        print("Control Laser Grid and Laser Bar")
    
    def _controlLifeSupport(self):
        print("Control Support")
    
    def _controlStatusBoard(self):
        print("Control StatusBoard")

    def _controlDockedShip(self):
        # Enable a puzzle if it is initialized
        if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_STATE] == STATE.INITIALIZED:
            self._controllers['docked_ship'].addCommand_SP(COMMAND.CMD_ENABLE)
        
        # Enable a puzzle if it is initialized
        if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_STATE] == STATE.INITIALIZED:
            self._controllers['docked_ship'].addCommand_ES(COMMAND.CMD_ENABLE)

        # Enable a puzzle if it is initialized
        if self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.INITIALIZED:
            self._controllers['prep_status'].addCommand(COMMAND.CMD_ENABLE)

        # Check the BatteryMatrix solved state and set to PrepStatus
        if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_BATTERY_MATRIX_STATE] == STATE.SOLVED and \
            self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_BATTERY_MATRIX_STATE] != STATE.SOLVED:
            self._controllers['prep_status'].addCommand(COMMAND.CMD_SET_PS_BATTERY_MATRIX_SOLVED)

        # Check the Generator solved state and set to PrepStatus
        if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_GENERATOR_STATE] == STATE.SOLVED and \
            self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_GENERATOR_STATE] != STATE.SOLVED:
            self._controllers['prep_status'].addCommand(COMMAND.CMD_SET_PS_GENERATOR_SOLVED)

        # Check the PowerAdjusment solved state and set to PrepStatus
        if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_POWER_ADJUSTER_STATE] == STATE.SOLVED and \
            self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_ENERGY_SUPP_STATE]  != STATE.SOLVED:
            self._controllers['prep_status'].addCommand(COMMAND.CMD_SET_PS_ENERGY_SUPP_SOLVED)

        #Check Docked Ship and Prep Status state and enable syncroKey if solved
        if self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_BATTERY_MATRIX_STATE]  == STATE.SOLVED  and \
        self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_ENERGY_SUPP_STATE]  == STATE.SOLVED and \
        self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_GENERATOR_STATE]  == STATE.SOLVED and \
        self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_SYNCRO_READER_STATE] == STATE.DISABLE:
            self._controllers['prep_status'].addCommand(COMMAND.CMD_ENABLE_PS_SYNCRO_READER)

        if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_BATTERY_MATRIX_STATE]  == STATE.SOLVED  and \
        self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_GENERATOR_STATE]  == STATE.SOLVED and \
        self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_POWER_ADJUSTER_STATE]  == STATE.SOLVED and \
        self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_SYNCRO_READER_STATE] == STATE.DISABLE:
            self._controllers['docked_ship'].addCommand_ES(COMMAND.CMD_ENABLE_DS_SYNCRO_READER)
        
        if self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_SYNCRO_READER_INPUT_KEY] == 4 and \
        self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_SYNCRO_READER_STATE] != STATE.SYNCRONIZED:
            self._psSyncroPoint = time_now()
            time.sleep(0.05)
            
        if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_SYNCRO_READER_INPUT_KEY] == 4 and \
        self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_SYNCRO_READER_STATE] != STATE.SYNCRONIZED:
            self._dsSyncroPoint = time_now()

        if abs(self._dsSyncroPoint - self._psSyncroPoint) > 0 and abs(self._dsSyncroPoint - self._psSyncroPoint) < 2000:
            self._controllers['prep_status'].addCommand(COMMAND.CMD_SET_PS_SYNCRO_READER_SYNCRONIZED)
            self._controllers['docked_ship'].addCommand_ES(COMMAND.CMD_SET_DS_SYNCRO_READER_SYNCRONIZED)

        if (self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_SYNCRO_READER_STATE] == STATE.ENABLE and \
            self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_SYNCRO_READER_STATE] == STATE.ENABLE):
            self._flag = True
            
        if (self._flag) or \
        (self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_SYNCRO_READER_STATE] == STATE.DONE and \
            self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_SYNCRO_READER_STATE] == STATE.DONE):
            self._controllers['prep_status'].startTimer() 
            time.sleep(0.0045)
            self._controllers['docked_ship'].startTimer()
            time.sleep(5)

    def _controlKeypad(self):
        #Enable Keypad if Life support, ship prep and lasergrid is Solved

        if self._controllers['life_support'].registers[LS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.SOLVED and \
            self._controllers['lasergrid'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.SOLVED and \
            self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_STATE] == STATE.SOLVED and \
            self._controllers['keypad'].registers[KP_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.ENABLE:
            self._controllers['keypad'].addCommand(COMMAND.CMD_ENABLE)

def cmp(a, b):
    return (a > b) - (a < b) 
