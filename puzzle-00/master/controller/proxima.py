#!/usr/bin/etc python3

from modbus_tk.modbus import LOGGER
import modbus_tk.defines as cst
import time
import copy
from lib.helpers import *
from lib.constants import *
from threading import Thread
import json

from database.db import Puzzle

class ProximaCommand(object):
    def __init__(self, controllers, master):
        self._controllers = controllers
        self._master = master
        self._step = 0
        self._batteryLevelMax = 13
        self._batteryLevelMin = 0
        self._batteryFull = False
        self._batteryEmpty = False
        self._gameStage = GAME_STAGE.NULL
        self._flag = False
        self._dsSyncroPoint = 0
        self._psSyncroPoint = 0
        self._syncroTimer = {
            'current': 0,
            'point': 0,
            'interval': 7000
        }

    def run(self):
        time.sleep(1)
        try:
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

            if self._gameStage == GAME_STAGE.ONGOING and \
               self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_STATE] == STATE.ENABLE and \
               self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.ENABLE:
                self._controlDockedShip()

            if self._gameStage == GAME_STAGE.ONGOING and \
               self._controllers['safeomatic'].registers[SC_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.ENABLE:
                self._controlSafeomatic()

            if self._gameStage == GAME_STAGE.ONGOING and \
               self._controllers['life_support'].registers[LS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.ENABLE:
                self._controlLifeSupport()
            
            if self._gameStage == GAME_STAGE.ONGOING:
                self._controlDatamatic()
            
            if self._gameStage == GAME_STAGE.ONGOING:
                self._controlLaserGridAndBar()

            if self._gameStage == GAME_STAGE.ONGOING:
                self._controlKeypad()
            
            if self._gameStage == GAME_STAGE.ONGOING and \
               self._controllers['status_board'].registers[SB_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.ENABLE:
                self._controlStatusBoard()

        except Exception as ex:
            LOGGER.debug("SystemDataCollector error: %s", str(ex))

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

        if self._controllers['power_control'].registers[PC_REGISTER_INDEX.REG_SLAVE_LIGHT_EFFECT_STATE] == STATE.FAILURE:
            self._controllers['datamatic'].addCommand(COMMAND.CMD_DISABLE)
            self._controllers['safeomatic'].addCommand(COMMAND.CMD_DISABLE)
            self._controllers['life_support'].addCommand(COMMAND.CMD_DISABLE)
            self._controllers['docked_ship'].addCommand_SP(COMMAND.CMD_DISABLE)
            self._controllers['docked_ship'].addCommand_ES(COMMAND.CMD_DISABLE)
            self._controllers['prep_status'].addCommand(COMMAND.CMD_DISABLE)
            self._controllers['lasergrid'].addCommand(COMMAND.CMD_DISABLE)
            self._controllers['laserbar'].addCommand(COMMAND.CMD_DISABLE)
            self._controllers['keypad'].addCommand(COMMAND.CMD_DISABLE)
        
        elif self._controllers['power_control'].registers[PC_REGISTER_INDEX.REG_SLAVE_BATTERY] == self._batteryLevelMin:
            self._batteryEmpty = True
            self._controllers['datamatic'].addCommand(COMMAND.CMD_DISABLE)
            self._controllers['safeomatic'].addCommand(COMMAND.CMD_DISABLE)
            self._controllers['life_support'].addCommand(COMMAND.CMD_DISABLE)
            self._controllers['docked_ship'].addCommand_SP(COMMAND.CMD_DISABLE)
            self._controllers['docked_ship'].addCommand_ES(COMMAND.CMD_DISABLE)
            self._controllers['prep_status'].addCommand(COMMAND.CMD_DISABLE)
            self._controllers['lasergrid'].addCommand(COMMAND.CMD_DISABLE)
            self._controllers['laserbar'].addCommand(COMMAND.CMD_DISABLE)
            self._controllers['keypad'].addCommand(COMMAND.CMD_DISABLE)
        
        elif self._controllers['power_control'].registers[PC_REGISTER_INDEX.REG_SLAVE_BATTERY] == self._batteryLevelMax:
            self._batteryFull = True
            if self._controllers['datamatic'].registers[DM_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.DISABLE:
                self._controllers['datamatic'].addCommand(COMMAND.CMD_ENABLE)
            if self._controllers['safeomatic'].registers[SC_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.DISABLE:
                self._controllers['safeomatic'].addCommand(COMMAND.CMD_ENABLE)
            if self._controllers['life_support'].registers[LS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.DISABLE:
                self._controllers['life_support'].addCommand(COMMAND.CMD_ENABLE)
            if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_STATE] == STATE.DISABLE:
                self._controllers['docked_ship'].addCommand_SP(COMMAND.CMD_ENABLE)
            if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_STATE] == STATE.DISABLE:
                self._controllers['docked_ship'].addCommand_ES(COMMAND.CMD_ENABLE)
            if self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.DISABLE:
                self._controllers['prep_status'].addCommand(COMMAND.CMD_ENABLE)
            if self._controllers['lasergrid'].registers[LG_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.DISABLE:
                self._controllers['lasergrid'].addCommand(COMMAND.CMD_ENABLE)
            if self._controllers['laserbar'].registers[LB_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.DISABLE:
                self._controllers['laserbar'].addCommand(COMMAND.CMD_ENABLE)


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
        _ = ""
        # print("Control Datamatic")
    
    def _controlSafeomatic(self):
        _ = ""
        # print("Control Safeomatic")
    
    def _controlLaserGridAndBar(self):
        if self._controllers['lasergrid'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.SOLVED:
            self._controllers['laserbar'].addCommand(COMMAND.CMD_DISABLE)     
        
    
    def _controlLifeSupport(self):
        _ = ""
        # print("Control Support")
    
    def _controlStatusBoard(self):
        #Update Status Board
        print(self._controllers['status_board'].model.changed)
        if self._controllers['status_board'].model.changed:
            changed_state = json.loads(self._controllers['status_board'].model.changed_state)
            self._controllers['status_board'].setCountdownTime(changed_state['count_down'])
            self._controllers['status_board'].addCommand(COMMAND.CMD_RESET_COUNTDOWN_TIME)
            Puzzle.update(changed=False).where(Puzzle.id == self._controllers['status_board'].model.id).execute()

        if self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.SOLVED:
            self._controllers['status_board'].addCommand(COMMAND.CMD_SET_DOCKED_SHIP_SOLVED)

        if self._controllers['lasergrid'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.SOLVED:
            self._controllers['status_board'].addCommand(COMMAND.CMD_SET_LASER_GRID_SOLVED)   

        if self._controllers['keypad'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.SOLVED:
            self._controllers['status_board'].addCommand(COMMAND.CMD_SET_KEYPAD_SOLVED)

        if self._controllers['life_support'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.SOLVED:
            self._controllers['status_board'].addCommand(COMMAND.CMD_SET_LIFE_SUPPORT_SOLVED)


    def _controlDockedShip(self):
        # Check the BatteryMatrix solved state and set to PrepStatus
        self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_BATTERY_MATRIX_STATE] = STATE.SOLVED
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
            
        if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_SYNCRO_READER_INPUT_KEY] == 4 and \
           self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_SYNCRO_READER_STATE] != STATE.SYNCRONIZED:
            self._dsSyncroPoint = time_now()

        if abs(self._dsSyncroPoint - self._psSyncroPoint) > 0 and abs(self._dsSyncroPoint - self._psSyncroPoint) < 1500:
            self._controllers['prep_status'].addCommand(COMMAND.CMD_SET_PS_SYNCRO_READER_SYNCRONIZED)
            self._controllers['docked_ship'].addCommand_ES(COMMAND.CMD_SET_DS_SYNCRO_READER_SYNCRONIZED)

        if (self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_SYNCRO_READER_STATE] == STATE.ENABLE and \
            self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_SYNCRO_READER_STATE] == STATE.ENABLE):
            self._flag = True
            
        if (self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_SYNCRO_READER_STATE] == STATE.DISABLE and \
            self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_SYNCRO_READER_STATE] == STATE.DISABLE):
            self._flag = True

        if (self._flag or \
            self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_SYNCRO_READER_STATE] == STATE.DONE and \
            self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_SYNCRO_READER_STATE] == STATE.DONE):
            self._flag = False
            self._startSyncroReaderTimer()
            
        
        self._syncroTimer['current'] = time_now()
        if (self._syncroTimer['current'] - self._syncroTimer['point']) > self._syncroTimer['interval'] and self._flag == False:
            self._syncroTimer['point'] = self._syncroTimer['current']
            self._flag = True
            

    def _controlKeypad(self):
        #Enable Keypad if Life support, ship prep and lasergrid is Solved

        if self._controllers['life_support'].registers[LS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.SOLVED and \
            self._controllers['lasergrid'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.SOLVED and \
            self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_STATE] == STATE.SOLVED and \
            self._controllers['keypad'].registers[KP_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.DISABLE:
            self._controllers['keypad'].addCommand(COMMAND.CMD_ENABLE)

    def _startSyncroReaderTimer(self, delayDS = 0, delayPS = 0):
        self._controllers['docked_ship']._busy = True
        registers = self._controllers['docked_ship'].registers
        registers[DS_REGISTER_INDEX.REG_MASTER_ES_COMMAND] = COMMAND.CMD_START_TIMER
        registers[DS_REGISTER_INDEX.REG_MASTER_ES_BODY] = delayDS
        registers[DS_REGISTER_INDEX.REG_SLAVE_ES_CONFIRM] = STATE.ACTIVE
        try: 
            self._master.execute(self._controllers['docked_ship'].getSlaveID(), cst.WRITE_MULTIPLE_REGISTERS, 11, output_value=registers[10:13])
        except Exception as excpt:
            print("ENER")

        registers = self._controllers['prep_status'].registers
        registers[PS_REGISTER_INDEX.REG_MASTER_COMMAND] = COMMAND.CMD_START_TIMER
        registers[PS_REGISTER_INDEX.REG_MASTER_BODY] = delayPS
        registers[PS_REGISTER_INDEX.REG_SLAVE_CONFIRM] = STATE.ACTIVE

        try: 
            self._master.execute(self._controllers['prep_status'].getSlaveID(), cst.WRITE_MULTIPLE_REGISTERS, 0, output_value=registers[0:5])
        except Exception as excpt:
            print("PREP")
        
        self._controllers['docked_ship']._busy = False

def cmp(a, b):
    return (a > b) - (a < b) 
