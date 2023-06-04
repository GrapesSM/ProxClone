#!/usr/bin/etc python3

from modbus_tk.modbus import LOGGER
import modbus_tk.defines as cst
import time
import asyncio
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
        self._dsSyncroKeyInserted = False
        self._dsSyncroPoint = 0
        self._psSyncroKeyInserted = False
        self._psSyncroPoint = 0
        self._syncroTimer = {
            'current': 0,
            'point': 0,
            'interval': 5500
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

    async def refresh(self, key_name, delay=0.0):
        try:
            await self._controllers[key_name].refresh(delay)
            if delay > 0.0:
                time.sleep(delay)
            #await self._controllers[key_name].refresh(delay)
        except Exception as ex:
            _ = ""

    async def refreshAll(self, delay):
        #print(self._controllers.keys())
        for key_name in self._controllers.keys():
            if key_name in ['docked_ship'] or key_name in ['prep_status']:
                await self.refresh('docked_ship', 0.00)
                await self.refresh('prep_status', 0.00)
                await self.refresh('prep_status', 0.00)
            if key_name in ['power_control']:
                await self.refresh(key_name, 0.00)
            if key_name in ['status_board']:
                await self.refresh(key_name, 0.00)
                await self.refresh(key_name, 0.00)
            if key_name in ['datamatic']:
                await self.refresh(key_name, 0.00)
            if key_name in ['safeomatic']:
                await self.refresh(key_name, 0.00)
            if key_name in ['life_support']:
                await self.refresh(key_name, 0.00)
            if key_name in ['laserbar']:
                await self.refresh(key_name, 0.00)
            if key_name in ['keypad']:
                await self.refresh(key_name, 0.00)
                await self.refresh(key_name, 0.00)
            if key_name in ['lasergrid']:
                await self.refresh(key_name, 0.00)
                await self.refresh(key_name, 0.00)
        #await asyncio.gather(
        #     self.refresh('prep_status'),
        #     self.refresh('docked_ship'),
        #     self.refresh('power_control'),
        #     self.refresh('status_board'),
        #     self.refresh('datamatic'),
        #     self.refresh('safeomatic'),
        #     self.refresh('lasergrid'),
        #     self.refresh('life_support'),
        #     self.refresh('laserbar'),
        #     self.refresh('keypad'),
        #)

    def update(self, delay, stop):
        while True:
            if stop():
                break
            # s = time.perf_counter()
            asyncio.run(self.refreshAll(delay))
            # elapsed = time.perf_counter() - s

    def run(self):
        self._runTimer['current'] = int(time.perf_counter() * 1000)
        if self._runTimer['current'] - self._runTimer['point'] > self._runTimer['interval']:
            self._runTimer['point'] = int(time.perf_counter() * 1000)
            try:
                _ = ""
                if self._controllers['status_board'].registers[SB_REGISTER_INDEX.REG_SLAVE_GAME_POWER_SWITCH_STATE] == STATE.ON:
                    if self._gameStage != GAME_STAGE.ONGOING:
                        self._gameStage = GAME_STAGE.START 

                if self._controllers['status_board'].registers[SB_REGISTER_INDEX.REG_SLAVE_GAME_POWER_SWITCH_STATE] == STATE.RESET:
                    self._gameStage = GAME_STAGE.RESET
                
                if self._controllers['status_board'].registers[SB_REGISTER_INDEX.REG_SLAVE_GAME_POWER_SWITCH_STATE] != STATE.ON and \
                    self._controllers['status_board'].registers[SB_REGISTER_INDEX.REG_SLAVE_GAME_POWER_SWITCH_STATE] != STATE.RESET:
                    self._gameStage = GAME_STAGE.PAUSE 

                if self._gameStage == GAME_STAGE.START:                    
                    if self._controllers['power_control'].registers[PC_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.DISABLE:
                        self._controllers['power_control'].write(COMMAND.CMD_ENABLE)
                    if self._controllers['laserbar'].registers[LB_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.DISABLE:
                        self._controllers['laserbar'].write(COMMAND.CMD_ENABLE)
                    
                    if self._controllers['power_control'].registers[PC_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.ENABLE:
                        self._gameStage = GAME_STAGE.ONGOING


                if self._gameStage == GAME_STAGE.RESET:
                    self._controllers['status_board'].write(COMMAND.CMD_RESET)    
                    self._controllers['power_control'].write(COMMAND.CMD_RESET)
                    self._controllers['laserbar'].write(COMMAND.CMD_RESET)
                    self._controllers['lasergrid'].write(COMMAND.CMD_RESET)
                    self._controllers['datamatic'].write(COMMAND.CMD_RESET)               
                    self._controllers['life_support'].write(COMMAND.CMD_RESET)
                    self._controllers['safeomatic'].write(COMMAND.CMD_RESET)
                    self._controllers['prep_status'].write(COMMAND.CMD_DISABLE)
                    self._controllers['docked_ship'].write_SP(COMMAND.CMD_RESET)
                    self._controllers['docked_ship'].write_ES(COMMAND.CMD_RESET)
                    self._controllers['keypad'].write(COMMAND.CMD_RESET)
                
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
                
                if self._gameStage == GAME_STAGE.ONGOING:
                    if self._controllers['status_board'].registers[SB_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.DISABLE:
                        self._controllers['status_board'].write(COMMAND.CMD_ENABLE)
                    if self._controllers['status_board'].registers[SB_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.ENABLE:
                        self._controlStatusBoard()

            except Exception as ex:
                LOGGER.debug("SystemDataCollector error: %s", str(ex))

    def _controlPowerControl(self):

        # #Check if the battery level is full and then enable all other puzzles if they are initialized
        if self._controllers['power_control'].registers[PC_REGISTER_INDEX.REG_SLAVE_LIGHT_EFFECT_STATE] == STATE.FAILURE:
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
            demand += 4.5
        if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_POWER_SWITCH_STATE] == STATE.ON:
            demand += 3.5
        if self._controllers['datamatic'].registers[DM_REGISTER_INDEX.REG_SLAVE_POWER_SWITCH_STATE] == STATE.ON:
            demand += 1.5
        if self._controllers['lasergrid'].registers[DM_REGISTER_INDEX.REG_SLAVE_POWER_SWITCH_STATE] == STATE.ON:
            demand += 1.5
        if self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_POWER_SWITCH_STATE] == STATE.ON:
            demand += 1.5
        if self._controllers['life_support'].registers[LS_REGISTER_INDEX.REG_SLAVE_POWER_SWITCH_STATE] == STATE.ON:
            demand += 2.5
        if self._controllers['safeomatic'].registers[SM_REGISTER_INDEX.REG_SLAVE_POWER_SWITCH_STATE] == STATE.ON:
            demand += 1.0
            
        #Check the demand value and set to power control if changed
        demand = int(demand * 10)
        if demand !=  self._controllers['power_control'].registers[PC_REGISTER_INDEX.REG_SLAVE_DEMAND]:
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
        elif self._controllers['lasergrid'].registers[PS_REGISTER_INDEX.REG_SLAVE_STATE] != STATE.SOLVED and \
             self._controllers['laserbar'].registers[LB_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.DISABLE:
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
        
        if self._controllers['status_board'].registers[SB_REGISTER_INDEX.REG_SLAVE_COUNTDOWN_VALUE] == 0:    
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

    def _controlDockedShip(self):
        # Check the BatteryMatrix solved state and set to PrepStatus
        if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_SP_BATTERY_MATRIX_STATE] == STATE.SOLVED and \
            self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_BATTERY_MATRIX_STATE] != STATE.SOLVED:
            # print("BM_SOLVED")
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
            if self._psSyncroKeyInserted == False:
                self._psSyncroPoint = int(time.perf_counter() * 1000)
                self._psSyncroKeyInserted = True

            if abs(int(time.perf_counter() * 1000) - self._psSyncroPoint) > 3000:
                self._controllers['prep_status'].write(COMMAND.CMD_SET_PS_SYNCRO_READER_WRONG_SOLVED)
                self._controllers['docked_ship'].write_ES(COMMAND.CMD_SET_DS_SYNCRO_READER_WRONG_SOLVED)

        if self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_SYNCRO_READER_INPUT_KEY] != 4:
            self._psSyncroKeyInserted = False
            
        
        if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_SYNCRO_READER_INPUT_KEY] == 4 and \
           self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_SYNCRO_READER_STATE] != STATE.SYNCRONIZED:
            if self._dsSyncroKeyInserted == False:
                self._dsSyncroPoint = int(time.perf_counter() * 1000)
                self._dsSyncroKeyInserted = True
            
            if abs(int(time.perf_counter() * 1000) - self._dsSyncroPoint) > 5000:
                self._controllers['prep_status'].write(COMMAND.CMD_SET_PS_SYNCRO_READER_WRONG_SOLVED)
                self._controllers['docked_ship'].write_ES(COMMAND.CMD_SET_DS_SYNCRO_READER_WRONG_SOLVED)
            
        if self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_SYNCRO_READER_INPUT_KEY] != 4:
            self._dsSyncroKeyInserted = False

        if self._psSyncroKeyInserted and self._dsSyncroKeyInserted and abs(self._dsSyncroPoint - self._psSyncroPoint) < 5000:
            self._controllers['prep_status'].write(COMMAND.CMD_SET_PS_SYNCRO_READER_SYNCRONIZED)
            self._controllers['docked_ship'].write_ES(COMMAND.CMD_SET_DS_SYNCRO_READER_SYNCRONIZED)
        
        if (self._controllers['docked_ship'].registers[DS_REGISTER_INDEX.REG_SLAVE_ES_SYNCRO_READER_STATE] == STATE.DONE and \
            self._controllers['prep_status'].registers[PS_REGISTER_INDEX.REG_SLAVE_SYNCRO_READER_STATE] == STATE.DONE):
            self._controllers['docked_ship'].write_ES(COMMAND.CMD_START_TIMER, 2000)
            self._controllers['prep_status'].write(COMMAND.CMD_START_TIMER, 0)
  
    def _controlKeypad(self):
        if self._controllers['keypad'].registers[KP_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.ENABLE:
            _ = ""
        

        if self._controllers['status_board'].registers[SB_REGISTER_INDEX.REG_SLAVE_LIFE_SUPPORT_STATUS_STATE] == STATE.SOLVED and \
            self._controllers['status_board'].registers[SB_REGISTER_INDEX.REG_SLAVE_LASER_GRID_STATUS_STATE] == STATE.SOLVED and \
            self._controllers['status_board'].registers[SB_REGISTER_INDEX.REG_SLAVE_SHIP_PREP_STATUS_STATE] == STATE.SOLVED and \
            self._controllers['keypad'].registers[KP_REGISTER_INDEX.REG_SLAVE_STATE] == STATE.DISABLE:
            self._controllers['keypad'].write(COMMAND.CMD_ENABLE)
            


def cmp(a, b):
    return (a > b) - (a < b) 
