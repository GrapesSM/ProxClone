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
        self.firstCall = 0
        self.lastCall = 0
        self._refreshTimer = {
            'current': 0,
            'point': 0,
            'interval': 250
        }
        self._runTimer = {
            'current': 0,
            'point': 0,
            'interval': 500
        }
        self._PCTimer = {
            'current': 0,
            'point': 0,
            'interval': 500
        }
    
    def update(self, delay, stop):
        while True:
            if stop():
                break
            
            for key_name in self._controllers.keys():
                if key_name not in [
                    'docked_ship',
                    'prep_status',
                    'power_control',
                    'status_board',
                    'datamatic',
                    'safeomatic',
                    'life_support',
                    'lasergrid',
                    'laserbar',
                    'keypad',
                    ]:
                    continue

                try: 
                    self._refreshTimer['current'] = int(time.perf_counter() * 1000)
                    if self._refreshTimer['current'] - self._refreshTimer['point'] > self._refreshTimer['interval']:
                        self._refreshTimer['point'] = int(time.perf_counter() * 1000)
                        slave = self._master.execute(self._controllers[key_name].getSlaveID(), cst.READ_HOLDING_REGISTERS, 3, 1)
                        if slave and slave[0] > 0:
                            self._controllers[key_name].refresh(delay)
                except Exception as ex:
                    _ = ""

    def run(self):
        self._runTimer['current'] = int(time.perf_counter() * 1000)
        if self._runTimer['current'] - self._runTimer['point'] > self._runTimer['interval']:
            self._runTimer['point'] = int(time.perf_counter() * 1000)
            try:
                _ = ""
                if self._controllers['status_board'].registers[SB_REGISTER_INDEX.REG_SLAVE_GAME_POWER_SWITCH_STATE] == STATE.ON:
                    if self._gameStage != GAME_STAGE.ONGOING:
                        print("START")
                        self._gameStage = GAME_STAGE.START 

                if self._controllers['status_board'].registers[SB_REGISTER_INDEX.REG_SLAVE_GAME_POWER_SWITCH_STATE] == STATE.RESET:
                    print("RESET")
                    self._gameStage = GAME_STAGE.RESET
                
                if self._controllers['status_board'].registers[SB_REGISTER_INDEX.REG_SLAVE_GAME_POWER_SWITCH_STATE] != STATE.ON and \
                    self._controllers['status_board'].registers[SB_REGISTER_INDEX.REG_SLAVE_GAME_POWER_SWITCH_STATE] != STATE.RESET:
                    print("PAUSE")
                    self._gameStage = GAME_STAGE.PAUSE 

                if self._gameStage == GAME_STAGE.START:
                    print(self._controllers['power_control'].registers[PC_REGISTER_INDEX.REG_SLAVE_STATE])
                    
                    if self._controllers['status_board'].registers[SB_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.DISABLE:
                        self._controllers['status_board'].write(COMMAND.CMD_ENABLE)
                    if self._controllers['power_control'].registers[PC_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.DISABLE:
                        self._controllers['power_control'].write(COMMAND.CMD_ENABLE)
                    if self._controllers['laserbar'].registers[LB_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.DISABLE:
                        self._controllers['laserbar'].write(COMMAND.CMD_ENABLE)
                    
                    if self._controllers['power_control'].registers[PC_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.ENABLE:
                        self._gameStage = GAME_STAGE.ONGOING


                if self._gameStage == GAME_STAGE.RESET:
                    if self._controllers['status_board'].registers[SB_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.DISABLE:
                        self._controllers['status_board'].write(COMMAND.CMD_RESET)    
                    if self._controllers['power_control'].registers[PC_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.DISABLE:
                        self._controllers['power_control'].write(COMMAND.CMD_RESET)
                    if self._controllers['laserbar'].registers[LB_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.DISABLE:
                        self._controllers['laserbar'].write(COMMAND.CMD_RESET)
                    if self._controllers['lasergrid'].registers[LG_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.DISABLE:
                        self._controllers['lasergrid'].write(COMMAND.CMD_RESET)
                    if self._controllers['datamatic'].registers[DM_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.DISABLE:
                        self._controllers['datamatic'].write(COMMAND.CMD_RESET)               
                    if self._controllers['life_support'].registers[LS_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.DISABLE:
                        self._controllers['life_support'].write(COMMAND.CMD_RESET)
                    if self._controllers['safeomatic'].registers[SM_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.DISABLE:
                        self._controllers['safeomatic'].write(COMMAND.CMD_RESET)
                    if self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.DISABLE:
                        self._controllers['prep_status'].write(COMMAND.CMD_DISABLE)
                    if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_STATE] != STATE.DISABLE:
                        self._controllers['docked_ship'].write_SP(COMMAND.CMD_RESET)
                    if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_STATE] != STATE.DISABLE:
                        self._controllers['docked_ship'].write_ES(COMMAND.CMD_RESET)
                    if self._controllers['keypad'].registers[KP_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.DISABLE:
                        self._controllers['keypad'].write(COMMAND.CMD_DISABLE)
                
                if self._gameStage == GAME_STAGE.PAUSE: 
                    if self._controllers['status_board'].registers[SB_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.DISABLE:
                        self._controllers['status_board'].write(COMMAND.CMD_DISABLE)    
                    if self._controllers['power_control'].registers[PC_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.DISABLE:
                        self._controllers['power_control'].write(COMMAND.CMD_DISABLE)
                    if self._controllers['laserbar'].registers[LB_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.DISABLE:
                        self._controllers['laserbar'].write(COMMAND.CMD_DISABLE)
                    if self._controllers['lasergrid'].registers[LG_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.DISABLE:
                        self._controllers['lasergrid'].write(COMMAND.CMD_DISABLE)
                    if self._controllers['datamatic'].registers[DM_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.DISABLE:
                        self._controllers['datamatic'].write(COMMAND.CMD_DISABLE)               
                    if self._controllers['life_support'].registers[LS_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.DISABLE:
                        self._controllers['life_support'].write(COMMAND.CMD_DISABLE)
                    if self._controllers['safeomatic'].registers[SM_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.DISABLE:
                        self._controllers['safeomatic'].write(COMMAND.CMD_DISABLE)
                    if self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.DISABLE:
                        self._controllers['prep_status'].write(COMMAND.CMD_DISABLE)
                    if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_STATE] != STATE.DISABLE:
                        self._controllers['docked_ship'].write_SP(COMMAND.CMD_DISABLE)
                    if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_STATE] != STATE.DISABLE:
                        self._controllers['docked_ship'].write_ES(COMMAND.CMD_DISABLE)
                    if self._controllers['keypad'].registers[KP_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.DISABLE:
                        self._controllers['keypad'].write(COMMAND.CMD_DISABLE)                        
                
                
                if self._gameStage == GAME_STAGE.ONGOING:
                    print("ONGOING")
                    self._controlPowerControl()

                if self._gameStage == GAME_STAGE.ONGOING:
                    self._controlDockedShip()

                if self._gameStage == GAME_STAGE.ONGOING:
                    self._controlSafeomatic()

                if self._gameStage == GAME_STAGE.ONGOING:
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

    def _controlPowerControl(self):

        # #Check if the battery level is full and then enable all other puzzles if they are initialized
        if self._controllers['power_control'].registers[PC_REGISTER_INDEX.REG_SLAVE_LIGHT_EFFECT_STATE] == STATE.FAILURE:
            print("FAILURE")
            if self._controllers['laserbar'].registers[LB_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.DISABLE:
                self._controllers['laserbar'].write(COMMAND.CMD_DISABLE)
            if self._controllers['lasergrid'].registers[LG_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.DISABLE:
                self._controllers['lasergrid'].write(COMMAND.CMD_DISABLE)
            if self._controllers['datamatic'].registers[DM_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.DISABLE:
                self._controllers['datamatic'].write(COMMAND.CMD_DISABLE)               
            if self._controllers['life_support'].registers[LS_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.DISABLE:
                self._controllers['life_support'].write(COMMAND.CMD_DISABLE)
            if self._controllers['safeomatic'].registers[SM_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.DISABLE:
                self._controllers['safeomatic'].write(COMMAND.CMD_DISABLE)
            if self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.DISABLE:
                self._controllers['prep_status'].write(COMMAND.CMD_DISABLE)
            if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_STATE] != STATE.DISABLE:
                self._controllers['docked_ship'].write_SP(COMMAND.CMD_DISABLE)
            if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_STATE] != STATE.DISABLE:
                self._controllers['docked_ship'].write_ES(COMMAND.CMD_DISABLE)
        
        elif self._controllers['power_control'].registers[PC_REGISTER_INDEX.REG_SLAVE_BATTERY] == self._batteryLevelMin:
            self._batteryFull = False
            print("LOW BATTERY LEVEL")
            if self._controllers['laserbar'].registers[LB_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.DISABLE:
                self._controllers['laserbar'].write(COMMAND.CMD_DISABLE)
            if self._controllers['lasergrid'].registers[LG_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.DISABLE:
                self._controllers['lasergrid'].write(COMMAND.CMD_DISABLE)
            if self._controllers['datamatic'].registers[DM_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.DISABLE:
                self._controllers['datamatic'].write(COMMAND.CMD_DISABLE)               
            if self._controllers['life_support'].registers[LS_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.DISABLE:
                self._controllers['life_support'].write(COMMAND.CMD_DISABLE)
            if self._controllers['safeomatic'].registers[SM_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.DISABLE:
                self._controllers['safeomatic'].write(COMMAND.CMD_DISABLE)
            if self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.DISABLE:
                self._controllers['prep_status'].write(COMMAND.CMD_DISABLE)
            if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_STATE] != STATE.DISABLE:
                self._controllers['docked_ship'].write_SP(COMMAND.CMD_DISABLE)
            if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_STATE] != STATE.DISABLE:
                self._controllers['docked_ship'].write_ES(COMMAND.CMD_DISABLE)
        
        elif self._controllers['power_control'].registers[PC_REGISTER_INDEX.REG_SLAVE_BATTERY] > self._batteryLevelMin:
            print("FULL BATTERY FULL")
            if self._controllers['datamatic'].registers[DM_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.DISABLE:
                self._controllers['datamatic'].write(COMMAND.CMD_ENABLE)
            if self._controllers['safeomatic'].registers[SM_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.DISABLE:
                self._controllers['safeomatic'].write(COMMAND.CMD_ENABLE)
            if self._controllers['life_support'].registers[LS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.DISABLE:
                self._controllers['life_support'].write(COMMAND.CMD_ENABLE)
            if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_STATE] == STATE.DISABLE:
                self._controllers['docked_ship'].write_SP(COMMAND.CMD_ENABLE)
            if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_STATE] == STATE.DISABLE:
                self._controllers['docked_ship'].write_ES(COMMAND.CMD_ENABLE)
            if self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.DISABLE:
                self._controllers['prep_status'].write(COMMAND.CMD_ENABLE)
            if self._controllers['lasergrid'].registers[LG_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.DISABLE:
                self._controllers['lasergrid'].write(COMMAND.CMD_ENABLE)

        #Check the power state of the puzzles and calculate the demand
        demand = 0.0
        if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_POWER_SWITCH_STATE] == STATE.ON:
            print("DOCKED_SHIP")
            demand += 4.5
        if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_POWER_SWITCH_STATE] == STATE.ON:
            print("DOCKED_SHIP")
            demand += 3.5
        if self._controllers['datamatic'].registers[DM_REGISTER_INDEX.REG_SLAVE_POWER_SWITCH_STATE] == STATE.ON:
            print("DATAMATIC")
            demand += 1.5
        if self._controllers['lasergrid'].registers[DM_REGISTER_INDEX.REG_SLAVE_POWER_SWITCH_STATE] == STATE.ON:
            print("LASER")
            demand += 1.5
        if self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_POWER_SWITCH_STATE] == STATE.ON:
            print("PREP")
            demand += 1.5
        if self._controllers['life_support'].registers[LS_REGISTER_INDEX.REG_SLAVE_POWER_SWITCH_STATE] == STATE.ON:
            print("LIFE_SUPPORT")
            demand += 2.5
        if self._controllers['safeomatic'].registers[SM_REGISTER_INDEX.REG_SLAVE_POWER_SWITCH_STATE] == STATE.ON:
            print("SAFEOMATIC")
            demand += 1.0
            
        #Check the demand value and set to power control if changed
        demand = int(demand * 10)
        print(demand, self._controllers['power_control'].registers[PC_REGISTER_INDEX.REG_SLAVE_DEMAND])
        if demand !=  self._controllers['power_control'].registers[PC_REGISTER_INDEX.REG_SLAVE_DEMAND]:
            print ("SET DEMAND")
            self._controllers['power_control'].write(COMMAND.CMD_SET_DEMAND, demand)

    def _controlDatamatic(self):
        if self._controllers['datamatic'].registers[DM_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.ENABLE:
            _ = ""
    
    def _controlSafeomatic(self):
        if self._controllers['safeomatic'].registers[SM_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.ENABLE:
            _ = ""
    
    def _controlLaserGridAndBar(self):
        if self._controllers['lasergrid'].registers[LG_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.ENABLE:
            _ = ""
        

        if self._controllers['lasergrid'].registers[LG_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.SOLVED and \
           self._controllers['laserbar'].registers[LB_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.DISABLE:
            self._controllers['laserbar'].write(COMMAND.CMD_DISABLE)
            print("------------------------------")
        elif self._controllers['lasergrid'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.SOLVED and \
             self._controllers['laserbar'].registers[LB_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.DISABLE:
            print("+++++++++++++++++++++++++++++")
            self._controllers['laserbar'].write(COMMAND.CMD_ENABLE)
        
    
    def _controlLifeSupport(self):
        if self._controllers['life_support'].registers[LS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.ENABLE:
            _ = ""
        
    def _controlStatusBoard(self):
        if self._controllers['status_board'].model.changed:
            changed_state = json.loads(self._controllers['status_board'].model.changed_state)
            self._controllers['status_board'].write(COMMAND.CMD_RESET_COUNTDOWN_TIME, changed_state['count_down'])
            Puzzle.update(changed=False).where(Puzzle.id == self._controllers['status_board'].model.id).execute()

        if self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.SOLVED and \
           self._controllers['status_board'].registers[SB_REGISTER_INDEX.REG_SLAVE_SHIP_PREP_STATUS_STATE] != STATE.SOLVED:
            self._controllers['status_board'].write(COMMAND.CMD_SET_DOCKED_SHIP_SOLVED)

        if self._controllers['lasergrid'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.SOLVED and \
           self._controllers['status_board'].registers[SB_REGISTER_INDEX.REG_SLAVE_LASER_GRID_STATUS_STATE] != STATE.SOLVED:
            self._controllers['status_board'].write(COMMAND.CMD_SET_LASER_GRID_SOLVED)   

        if self._controllers['keypad'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.SOLVED and \
           self._controllers['status_board'].registers[SB_REGISTER_INDEX.REG_SLAVE_BLAST_DOOR_STATUS_STATE] != STATE.SOLVED:
            self._controllers['status_board'].write(COMMAND.CMD_SET_KEYPAD_SOLVED)

        if self._controllers['life_support'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.SOLVED and \
           self._controllers['status_board'].registers[SB_REGISTER_INDEX.REG_SLAVE_LIFE_SUPPORT_STATUS_STATE] != STATE.SOLVED:
            self._controllers['status_board'].write(COMMAND.CMD_SET_LIFE_SUPPORT_SOLVED)

    def _controlDockedShip(self):
        # Check the BatteryMatrix solved state and set to PrepStatus
        if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_BATTERY_MATRIX_STATE] == STATE.SOLVED and \
            self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_BATTERY_MATRIX_STATE] != STATE.SOLVED:
            print("BM_SOLVED")
            self._controllers['prep_status'].write(COMMAND.CMD_SET_PS_BATTERY_MATRIX_SOLVED)

        # Check the Generator solved state and set to PrepStatus
        if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_GENERATOR_STATE] == STATE.SOLVED and \
            self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_GENERATOR_STATE] != STATE.SOLVED:
            self._controllers['prep_status'].write(COMMAND.CMD_SET_PS_GENERATOR_SOLVED)

        # Check the PowerAdjusment solved state and set to PrepStatus
        if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_POWER_ADJUSTER_STATE] == STATE.SOLVED and \
            self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_ENERGY_SUPP_STATE]  != STATE.SOLVED:
            self._controllers['prep_status'].write(COMMAND.CMD_SET_PS_ENERGY_SUPP_SOLVED)

        #Check Docked Ship and Prep Status state and enable syncroKey if solved
        if self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_BATTERY_MATRIX_STATE]  == STATE.SOLVED  and \
           self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_ENERGY_SUPP_STATE]  == STATE.SOLVED and \
           self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_GENERATOR_STATE]  == STATE.SOLVED and \
           self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_SYNCRO_READER_STATE] == STATE.DISABLE:
            self._controllers['prep_status'].write(COMMAND.CMD_ENABLE_PS_SYNCRO_READER)

        if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_BATTERY_MATRIX_STATE]  == STATE.SOLVED  and \
           self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_GENERATOR_STATE]  == STATE.SOLVED and \
           self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_POWER_ADJUSTER_STATE]  == STATE.SOLVED and \
           self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_SYNCRO_READER_STATE] == STATE.DISABLE:
            self._controllers['docked_ship'].write_ES(COMMAND.CMD_ENABLE_DS_SYNCRO_READER)
        
        if self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_SYNCRO_READER_INPUT_KEY] == 4 and \
            self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_SYNCRO_READER_STATE] != STATE.SYNCRONIZED:
            self._psSyncroPoint = time_now()    
            if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_SYNCRO_READER_INPUT_KEY] == 4 and \
                self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_SYNCRO_READER_STATE] != STATE.SYNCRONIZED:
                self._dsSyncroPoint = time_now()
                if abs(self._dsSyncroPoint - self._psSyncroPoint) >= 0 and abs(self._dsSyncroPoint - self._psSyncroPoint) < 5000:
                    self._controllers['prep_status'].write(COMMAND.CMD_SET_PS_SYNCRO_READER_SYNCRONIZED)
                    self._controllers['docked_ship'].write_ES(COMMAND.CMD_SET_DS_SYNCRO_READER_SYNCRONIZED)

        if (self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_SYNCRO_READER_STATE] == STATE.DONE and \
            self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_SYNCRO_READER_STATE] == STATE.DONE):
            self._controllers['docked_ship'].write_ES(COMMAND.CMD_START_TIMER, 2000)
            self._controllers['prep_status'].write(COMMAND.CMD_START_TIMER, 0)
  

        self._syncroTimer['current'] = time_now()
        if (self._syncroTimer['current'] - self._syncroTimer['point']) > self._syncroTimer['interval'] and self._flag == False:
            self._syncroTimer['point'] = self._syncroTimer['current']
            self._flag = True

    def _controlKeypad(self):
        if self._controllers['keypad'].registers[KP_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.ENABLE:
            _ = ""
        

        if self._controllers['life_support'].registers[LS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.SOLVED and \
            self._controllers['lasergrid'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.SOLVED and \
            self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_STATE] == STATE.SOLVED and \
            self._controllers['keypad'].registers[KP_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.DISABLE:
            self._controllers['keypad'].write(COMMAND.CMD_ENABLE)
            


def cmp(a, b):
    return (a > b) - (a < b) 