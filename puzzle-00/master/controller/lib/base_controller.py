#!/usr/bin/etc python3
from modbus_tk.modbus import LOGGER
import modbus_tk.defines as cst
import time
import json
from .constants import STATE
from .constants import STATE, COMMAND, REGISTER_INDEX, STATUS
from queue import *


class BaseController:
    _busy = False
    
    def __init__(self, key_name, model, puzzle, master):
        self._model=model
        self._puzzle=puzzle
        self._key_name = key_name
        self._command = COMMAND.CMD_NONE
        self._commandQueue = []
        self._commandStatus = STATUS.ST_NONE
        self._master = master
        self._forced = False
        self._timer = {
            'current': 0,
            'point': 0,
            'interval': 5000 #milliseconds
        }

    @property
    def registers(self):
        return self._puzzle['registers']

    def getState(self):
        return self._puzzle['registers'][REGISTER_INDEX.REG_SLAVE_STATE]

    def setState(self, state):
        self._puzzle['registers'][REGISTER_INDEX.REG_SLAVE_STATE] = state

    def getSlaveID(self):
        return self._puzzle['slave_id']

    def getKeyName(self):
        return self._key_name

    def getCommand(self):
        return self._command

    def addCommand(self, command):
        if command not in self._commandQueue:
            self._commandQueue.append(command)

    def refreshCommand(self):
        if self._command == COMMAND.CMD_NONE:
            if len(self._commandQueue) != 0:
                self._command = self._commandQueue.pop(0)
                self._commandStatus = STATUS.ST_CREATED
        else:
            if self._puzzle['registers'][REGISTER_INDEX.REG_SLAVE_CONFIRM] == STATE.DONE:
                self._command = COMMAND.CMD_NONE
                self._commandStatus = STATUS.ST_CREATED

    def readAndWriteToSlave(self, delay, stop):
        while True:
            if stop():
                break

            if self._busy:
                continue

            self._busy = True

            registers = None
            for _ in range(1):
                try: 
                    registers = list(self._master.execute(self.getSlaveID(), cst.READ_HOLDING_REGISTERS, 0, self.getNumberOfRegisters()))
                    if delay > 0: 
                        time.sleep(delay)
                except Exception as excpt:
                    _ = ""
                    LOGGER.debug("SystemDataCollector1 error: %s", str(excpt))

            if registers:
                # print(self.getKeyName() ,"Slave Registers (received):  ", registers)
                self.update(registers)
                # print(self.getKeyName() ,"Slave Registers (modified):  ", self.registers)

            if self.getCommand() != COMMAND.CMD_NONE:
                readAgain = False
                for _ in range(1):
                    try: 
                        readAgain = True
                        self._master.execute(self.getSlaveID(), cst.WRITE_MULTIPLE_REGISTERS, 0, output_value=self.registers)
                        if delay > 0: 
                            time.sleep(delay)
                        self._command = COMMAND.CMD_NONE
                    except Exception as excpt:
                        _ = ""
                        LOGGER.debug("SystemDataCollector error: %s", str(excpt))
                
                # if readAgain:
                #     registers = None
                #     for _ in range(1):
                #         try:
                #             registers = list(self._master.execute(self.getSlaveID(), cst.READ_HOLDING_REGISTERS, 0, self.getNumberOfRegisters()))
                #             if delay > 0: 
                #                 time.sleep(delay)
                #             registers = self.registers
                #         except Exception as excpt:
                #             _ = ""
                #             # LOGGER.debug("SystemDataCollector1 error: %s", str(excpt))
                #     if registers:
                #         print(self.getKeyName() ,"Slave Registers (received):  ", registers)
                #         self.update(registers)
                #         print(self.getKeyName() ,"Slave Registers (modified):  ", self.registers)
            self._busy = False
            self.refreshCommand()


    def getCommandStatus(self):
        return self._commandStatus

    def setCommand(self, command):
        self._command = command
        self._commandStatus = STATUS.ST_CREATED

    def setRegisters(self, registers):
        for i in range(len(self._puzzle['registers'])):
            self._puzzle['registers'][i] = registers[i]
        
        self._timer['current'] = int(round(time.time() * 1000))
        if (self._timer['current'] - self._timer['point']) > self._timer['interval']:
            self._timer['point'] = int(round(time.time() * 1000))
            self._model.state = json.dumps(self._puzzle)
            self._model.save()

    def getNumberOfRegisters(self):
        return len(self._puzzle['registers'])
        