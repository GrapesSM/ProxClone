#!/usr/bin/etc python3

from __future__ import print_function

from modbus_tk import defines
from modbus_tk.modbus import LOGGER
from modbus_tk.defines import HOLDING_REGISTERS
from modbus_tk.modbus_rtu import RtuServer, RtuMaster
from modbus_tk.simulator import Simulator
from modbus_tk.simulator_rpc_client import SimulatorRpcClient
from modbus_tk.utils import WorkerThread
import random
import time
from threading import Thread
import serial
import json

import os, sys
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from database.db import Puzzle
import configuration as cfg

from lib.power_control_controller import PowerControlController
from lib.datamatic_controller import DatamaticController
from lib.docked_ship_controller import DockedShipController
from lib.prep_status_controller import PrepStatusController
from lib.laser_grid_controller import LaserGridController
from lib.life_support_controller import LifeSupportController
from lib.safeomatic_controller import SafeomaticController
from lib.status_board_controller import StatusBoardController
from lib.keypad_controller import KeypadController
from lib.laserbar_controller import LaserbarController
from proxima import ProximaCommand

def createControllers(master):
    controllers = {}
    for key_name in cfg.puzzles.keys():
        if key_name == "power_control":
            model = Puzzle.get_or_none(key_name=key_name)
            controllers[key_name] = PowerControlController(key_name, model, cfg.puzzles[key_name], master)
        if key_name == "datamatic":
            model = Puzzle.get_or_none(key_name=key_name)
            controllers[key_name] =  DatamaticController(key_name, model, cfg.puzzles[key_name], master)
        if key_name == "docked_ship":
            model = Puzzle.get_or_none(key_name=key_name)
            controllers[key_name] = DockedShipController(key_name, model, cfg.puzzles[key_name], master)
        if key_name == "prep_status":
            model = Puzzle.get_or_none(key_name=key_name)
            controllers[key_name] = PrepStatusController(key_name,model, cfg.puzzles[key_name], master)
        if key_name == "lasergrid":
            model = Puzzle.get_or_none(key_name=key_name)
            controllers[key_name] = LaserGridController(key_name, model, cfg.puzzles[key_name], master)
        if key_name == "life_support":
            model = Puzzle.get_or_none(key_name=key_name)
            controllers[key_name] = LifeSupportController(key_name, model, cfg.puzzles[key_name], master)
        if key_name == "safeomatic":
            model = Puzzle.get_or_none(key_name=key_name)
            controllers[key_name] = SafeomaticController(key_name, model, cfg.puzzles[key_name], master)
        if key_name == "status_board":
            model = Puzzle.get_or_none(key_name=key_name)
            controllers[key_name] = StatusBoardController(key_name, model, cfg.puzzles[key_name], master)
        if key_name == "keypad":
            model = Puzzle.get_or_none(key_name=key_name)
            controllers[key_name] = KeypadController(key_name, model, cfg.puzzles[key_name], master)
        if key_name == "laserbar":
            model = Puzzle.get_or_none(key_name=key_name)
            controllers[key_name] = LaserbarController(key_name, model, cfg.puzzles[key_name], master)
    return controllers

def main():
    #create the modbus RTU simulator
    server = RtuServer(serial.Serial(port=None))
    simulator = Simulator(server)

    # create master to communicate to slaves
    master = RtuMaster(serial.Serial(
        port=cfg.modbus['port'], 
        baudrate=cfg.modbus['baudrate'], 
        bytesize=cfg.modbus['bytesize'], 
        parity=cfg.modbus['parity'], 
        stopbits=cfg.modbus['stopbits'], 
        xonxoff=cfg.modbus['xonxoff']))
    master.set_timeout(cfg.modbus['timeout'])
    master.set_verbose(cfg.modbus['verbose'])
    while not master:
        print("Hello")

    controllers = createControllers(master)
    proximaCommand = ProximaCommand(controllers, master)
    systemMonitor = WorkerThread(proximaCommand.run)
    slaveThreads = list()
    flagStopThreads = False
    for key_name in controllers.keys():
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
        slaveThreads.append(Thread(target = controllers[key_name].readAndWriteToSlave, args=(0.0, lambda : flagStopThreads), daemon = True))


    try:
        LOGGER.info("'quit' for closing the server")

        #start the system
        systemMonitor.start()

        for thread in slaveThreads:
            thread.start()

        #start the simulator! will block until quit command is received
        simulator.start()


    except Exception as excpt:
        print(excpt)

    finally:
        flagStopThreads = True
        #close the simulator
        simulator.close()
        #stop the data collect
        systemMonitor.stop()



if __name__ == "__main__":
    main()