#!/usr/bin/etc python3

import config as cfg
import json
import time

import os, sys
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from database.db import *

'''
Controller:
    initialize
'''

if __name__ == "__main__":
    timer=0
    counter=0
    while True:
        if (time.time() - timer) < 0.800:
            continue

        if counter > 1000000:
            break
        counter = counter + 1
        timer = time.time()
        # ModBus: Open ModBus connection ----------------------
        if not master:
            try:
                master = modbus_rtu.RtuMaster(
                    serial.Serial(port=cfg.modbus['port'], baudrate=cfg.modbus['baudrate'], bytesize=cfg.modbus['bytesize'], parity=cfg.modbus['parity'], stopbits=cfg.modbus['stopbits'], xonxoff=cfg.modbus['xonxoff'])
                )
                master.set_timeout(cfg.modbus['timeout'])
                master.set_verbose(cfg.modbus['verbose'])
            except:
                logger.error("Failure into opening the connection")
        # -----------------------------------------------------
        
        # DB: Select all puzzles ------------------------------
        puzzles = {}
        states = {}
        for p in Puzzle.select():
            puzzles[p.id] = p
            states[p.id] = json.loads(p.state)
        # -----------------------------------------------------

        # ModBus: Read all puzzles ----------------------------
        for p_id in puzzles.keys():
            states[p_id]['registers'] = queryPuzzle(states[p_id]['address'], states[p_id]['number_of_registers'])
        #------------------------------------------------------

        # DB: Update all puzzles ------------------------------
        for p_id in puzzles.keys():
            puzzles[p_id].state = json.dumps(states[p_id])
            puzzles[p_id].save()
        # -----------------------------------------------------

        # Count number of iterations
        # count = count + 1
        # if count > 30:
        #     break

    

'''
try:
    a = master.execute(1, cst.READ_HOLDING_REGISTERS, 0, 20)
    a += master.execute(1, cst.READ_HOLDING_REGISTERS, 20, 20)
    a += master.execute(1, cst.READ_HOLDING_REGISTERS, 40, 20)
    a += master.execute(1, cst.READ_HOLDING_REGISTERS, 60, 10)
except modbus_tk.modbus.ModbusError as exc:
    logger.error("%s- Code=%d", exc, exc.get_exception_code())


try:
    master.execute(1, cst.READ_HOLDING_REGISTERS, 0, 4)
    master.execute(1, cst.WRITE_MULTIPLE_REGISTERS, 0, output_value=[1, int(request.form['red']), int(request.form['green']), int(request.form['blue'])])
except modbus_tk.modbus.ModbusError as exc:
    logger.error("%s- Code=%d", exc, exc.get_exception_code())
*2
inputs = request.form
registers = []
for name in inputs:
    registers.append(int(inputs[name]))       
    
try:
    master.execute(1, cst.WRITE_MULTIPLE_REGISTERS, 0, output_value=registers[0:20])
    master.execute(1, cst.WRITE_MULTIPLE_REGISTERS, 20, output_value=registers[20:40])
    master.execute(1, cst.WRITE_MULTIPLE_REGISTERS, 40, output_value=registers[40:60])
    master.execute(1, cst.WRITE_MULTIPLE_REGISTERS, 60, output_value=registers[60:70])
    registers[0] = 1
    master.execute(1, cst.WRITE_MULTIPLE_REGISTERS, 0, output_value=registers[0:1])
except modbus_tk.modbus.ModbusError as exc:
    logger.error("%s- Code=%d", exc, exc.get_exception_code())

logger.info(registers[0:20])
logger.info(registers[20:40])
logger.info(registers[40:60])
logger.info(registers[60:70])
'''