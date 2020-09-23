#!/usr/bin/etc python3
from modbus_tk.modbus import LOGGER
import modbus_tk.defines as cst
from .base_controller import BaseController
from .constants import STATE, COMMAND, DS_REGISTER_INDEX, STATUS
from .helpers import time_now
from enum import Enum
import time

class DockedShipController(BaseController):
    def __init__(self, key_name, model, puzzle, master):
        super().__init__(key_name, model, puzzle, master)
        self._command_ES = COMMAND.CMD_NONE
        self._commandStatus_ES = STATUS.ST_NONE
        self._command_SP = COMMAND.CMD_NONE
        self._commandStatus_SP = COMMAND.CMD_NONE
        self._commandQueue_ES = []
        self._commandQueue_SP = []
        self._powerAdjusterKey = 650
        self._baterryMatrixKey = 12345
        self._generatorKey = 12345
        self._startTimerCmdWritten = False


    def update(self, registers):
        # controller register vs slave register
        if  registers[DS_REGISTER_INDEX.REG_SLAVE_SP_CONFIRM] == 0:
            registers[DS_REGISTER_INDEX.REG_SLAVE_SP_CONFIRM] = STATE.DONE

        if  registers[DS_REGISTER_INDEX.REG_SLAVE_ES_CONFIRM] == 0:
            registers[DS_REGISTER_INDEX.REG_SLAVE_ES_CONFIRM] = STATE.DONE

        if self.getCommand_ES() == registers[DS_REGISTER_INDEX.REG_MASTER_ES_COMMAND]:
            if registers[DS_REGISTER_INDEX.REG_SLAVE_ES_CONFIRM] == STATE.DONE:
                registers[DS_REGISTER_INDEX.REG_MASTER_ES_COMMAND] = COMMAND.CMD_NONE
                self._command_ES = COMMAND.CMD_NONE
                self._commandStatus_ES = STATUS.ST_CONFIRMED
        
        if self.getCommand_SP() == registers[DS_REGISTER_INDEX.REG_MASTER_SP_COMMAND]:
            if registers[DS_REGISTER_INDEX.REG_SLAVE_SP_CONFIRM] == STATE.DONE:
                registers[DS_REGISTER_INDEX.REG_MASTER_SP_COMMAND] = COMMAND.CMD_NONE
                self._command_SP = COMMAND.CMD_NONE
                self._commandStatus_SP = STATUS.ST_CONFIRMED

        if self.getCommand_ES() == COMMAND.CMD_ENABLE and self.getCommandStatus_ES() == STATUS.ST_CREATED:
            registers[DS_REGISTER_INDEX.REG_MASTER_ES_COMMAND] = COMMAND.CMD_ENABLE
            registers[DS_REGISTER_INDEX.REG_SLAVE_ES_CONFIRM] = STATE.ACTIVE

        if self.getCommand_ES() == COMMAND.CMD_RESET and self.getCommandStatus_ES() == STATUS.ST_CREATED:
            registers[DS_REGISTER_INDEX.REG_MASTER_ES_COMMAND] = COMMAND.CMD_RESET
            registers[DS_REGISTER_INDEX.REG_SLAVE_ES_CONFIRM] = STATE.ACTIVE

        if self.getCommand_ES() == COMMAND.CMD_DISABLE and self.getCommandStatus_ES() == STATUS.ST_CREATED:
            registers[DS_REGISTER_INDEX.REG_MASTER_ES_COMMAND] = COMMAND.CMD_DISABLE
            registers[DS_REGISTER_INDEX.REG_SLAVE_ES_CONFIRM] = STATE.ACTIVE

        if self.getCommand_ES() == COMMAND.CMD_PAUSE and self.getCommandStatus_ES() == STATUS.ST_CREATED:
            registers[DS_REGISTER_INDEX.REG_MASTER_ES_COMMAND] = COMMAND.CMD_PAUSE
            registers[DS_REGISTER_INDEX.REG_SLAVE_ES_CONFIRM] = STATE.ACTIVE

        if self.getCommand_ES() == COMMAND.CMD_SET_SOLVED and self.getCommandStatus_ES() == STATUS.ST_CREATED:
            registers[DS_REGISTER_INDEX.REG_MASTER_ES_COMMAND] = COMMAND.CMD_SET_SOLVED
            registers[DS_REGISTER_INDEX.REG_SLAVE_ES_CONFIRM] = STATE.ACTIVE

        if self.getCommand_ES() == COMMAND.CMD_ENABLE_DS_SYNCRO_READER and self.getCommandStatus_ES() == STATUS.ST_CREATED:
            registers[DS_REGISTER_INDEX.REG_MASTER_ES_COMMAND] = COMMAND.CMD_ENABLE_DS_SYNCRO_READER
            registers[DS_REGISTER_INDEX.REG_SLAVE_ES_CONFIRM] = STATE.ACTIVE

        if self.getCommand_ES() == COMMAND.CMD_SET_DS_SYNCRO_READER_SYNCRONIZED and self.getCommandStatus_ES() == STATUS.ST_CREATED:
            registers[DS_REGISTER_INDEX.REG_MASTER_ES_COMMAND] = COMMAND.CMD_SET_DS_SYNCRO_READER_SYNCRONIZED
            registers[DS_REGISTER_INDEX.REG_SLAVE_ES_CONFIRM] = STATE.ACTIVE

        if self.getCommand_ES() == COMMAND.CMD_SET_DS_SYNCRO_READER_WRONG_SOLVED and self.getCommandStatus_ES() == STATUS.ST_CREATED:
            registers[DS_REGISTER_INDEX.REG_MASTER_ES_COMMAND] = COMMAND.CMD_SET_DS_SYNCRO_READER_WRONG_SOLVED
            registers[DS_REGISTER_INDEX.REG_SLAVE_ES_CONFIRM] = STATE.ACTIVE

        if self.getCommand_ES() == COMMAND.CMD_START_TIMER and self.getCommandStatus_ES() == STATUS.ST_CREATED:
            registers[DS_REGISTER_INDEX.REG_MASTER_ES_COMMAND] = COMMAND.CMD_START_TIMER
            registers[DS_REGISTER_INDEX.REG_SLAVE_ES_CONFIRM] = STATE.ACTIVE






        if self.getCommand_SP() == COMMAND.CMD_ENABLE and self.getCommandStatus_SP() == STATUS.ST_CREATED:
            registers[DS_REGISTER_INDEX.REG_MASTER_SP_COMMAND] = COMMAND.CMD_ENABLE
            registers[DS_REGISTER_INDEX.REG_SLAVE_SP_CONFIRM] = STATE.ACTIVE
        

        if self.getCommand_SP() == COMMAND.CMD_RESET and self.getCommandStatus_SP() == STATUS.ST_CREATED:
            registers[DS_REGISTER_INDEX.REG_MASTER_SP_COMMAND] = COMMAND.CMD_RESET
            registers[DS_REGISTER_INDEX.REG_SLAVE_SP_CONFIRM] = STATE.ACTIVE

        if self.getCommand_SP() == COMMAND.CMD_DISABLE and self.getCommandStatus_SP() == STATUS.ST_CREATED:
            registers[DS_REGISTER_INDEX.REG_MASTER_SP_COMMAND] = COMMAND.CMD_DISABLE
            registers[DS_REGISTER_INDEX.REG_SLAVE_SP_CONFIRM] = STATE.ACTIVE

        if self.getCommand_SP() == COMMAND.CMD_PAUSE and self.getCommandStatus_SP() == STATUS.ST_CREATED:
            registers[DS_REGISTER_INDEX.REG_MASTER_SP_COMMAND] = COMMAND.CMD_PAUSE
            registers[DS_REGISTER_INDEX.REG_SLAVE_SP_CONFIRM] = STATE.ACTIVE

        if self.getCommand_SP() == COMMAND.CMD_SET_SOLVED and self.getCommandStatus_SP() == STATUS.ST_CREATED:
            registers[DS_REGISTER_INDEX.REG_MASTER_SP_COMMAND] = COMMAND.CMD_SET_SOLVED
            registers[DS_REGISTER_INDEX.REG_SLAVE_SP_CONFIRM] = STATE.ACTIVE

        if self.getCommand_SP() == COMMAND.CMD_SET_DS_BATTERY_MATRIX_SOLVED and self.getCommandStatus_SP() == STATUS.ST_CREATED:
            registers[DS_REGISTER_INDEX.REG_MASTER_SP_COMMAND] = COMMAND.CMD_SET_DS_BATTERY_MATRIX_SOLVED
            registers[DS_REGISTER_INDEX.REG_SLAVE_SP_CONFIRM] = STATE.ACTIVE

        if self.getCommand_SP() == COMMAND.CMD_SET_DS_GENERATOR_SOLVED and self.getCommandStatus_SP() == STATUS.ST_CREATED:
            registers[DS_REGISTER_INDEX.REG_MASTER_SP_COMMAND] = COMMAND.CMD_SET_DS_GENERATOR_SOLVED
            registers[DS_REGISTER_INDEX.REG_SLAVE_SP_CONFIRM] = STATE.ACTIVE

        self.setRegisters(registers)

    def getCommand(self):
        if (self._command_ES == COMMAND.CMD_NONE and self._command_SP == COMMAND.CMD_NONE):
            return COMMAND.CMD_NONE
        return self._command_ES if self._command_ES != COMMAND.CMD_NONE else self._command_SP

    def getCommand_ES(self):
        return self._command_ES

    def getCommandStatus_ES(self):
        return self._commandStatus_ES

    def getCommand_SP(self):
        return self._command_SP

    def getCommandStatus_SP(self):
        return self._commandStatus_SP
    
    def setCommand_ES(self, command):
        self._command_ES = command
        self._commandStatus_ES = STATUS.ST_CREATED
    
    def setCommand_SP(self, command):
        self._command_SP = command
        self._commandStatus_SP = STATUS.ST_CREATED

    def addCommand_ES(self, command):
        if command not in self._commandQueue_ES:
            self._commandQueue_ES.append(command)

    def addCommand_SP(self, command):
        if command not in self._commandQueue_SP:
            self._commandQueue_SP.append(command)

    def refreshCommand(self):
        if self._command_ES == COMMAND.CMD_NONE:
            if len(self._commandQueue_ES) != 0:
                self._command_ES = self._commandQueue_ES.pop(0)
                self._commandStatus_ES = STATUS.ST_CREATED

        if self._command_SP == COMMAND.CMD_NONE:
            if len(self._commandQueue_SP) != 0:
                self._command_SP = self._commandQueue_SP.pop(0)
                self._commandStatus_SP = STATUS.ST_CREATED
    
    def startTimer(self):
        # print("DOCKED_SHIP START TIMER")    

        registers = self.registers
        registers[DS_REGISTER_INDEX.REG_MASTER_ES_COMMAND] = 7
        registers[DS_REGISTER_INDEX.REG_SLAVE_ES_CONFIRM] = 0  

        # print(registers)      
        
        for _ in range(1):
            try: 
                # readAgain = True
                self._master.execute(self.getSlaveID(), cst.WRITE_MULTIPLE_REGISTERS, 0, output_value=registers)
                # print(self.getKeyName(), self.getSlaveID(), "write")
                # time.sleep(0.001)
            except Exception as excpt:
                # print(self.getKeyName(), end=" ")
                _ = ""
                # LOGGER.debug("SystemDataCollector error: %s", str(excpt))