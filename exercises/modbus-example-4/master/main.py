#!/usr/bin/env python3
# -*- coding: utf_8 -*-
"""
 Modbus TestKit: Implementation of Modbus protocol in python
 (C)2009 - Luc Jean - luc.jean@gmail.com
 (C)2009 - Apidev - http://www.apidev.fr
 This is distributed under GNU LGPL license, see license.txt
"""

import serial

import modbus_tk
import modbus_tk.defines as cst
from modbus_tk.modbus_rtu import RtuServer
from modbus_tk.simulator import Simulator
from modbus_tk.modbus import Databank

#PORT = 1
PORT = '/dev/ttyUSB0'
# general test comment POB
def main():
    """main"""
    logger = modbus_tk.utils.create_logger("console")
    server = RtuServer(
        serial.Serial(port=PORT, baudrate=19200, bytesize=8, parity='N', stopbits=1, xonxoff=0)
    )
    server.set_timeout(5.0)
    server.set_verbose(True)
    server.add_slave(1)
    
    simu = Simulator(server)
    try:
        logger.info("'quit' for closing the server")
        simu.start()
    except Exception as exc:
        logger.error("%s- Code=%d", exc, exc.get_exception_code())
    finally:
        simu.close()
if __name__ == "__main__":
    main()
