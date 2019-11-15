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
from modbus_tk import modbus_rtu

#PORT = 1
PORT = '/dev/ttyUSB0'
data = []

def main():
    """main"""
    logger = modbus_tk.utils.create_logger("console")

    try:

        #Connect to the slave
        server = modbus_rtu.RtuServer(
            serial.Serial(port=PORT, baudrate=19200, bytesize=8, parity='N', stopbits=1, xonxoff=0), data
        )
        server.set_timeout(5.0)
        server.start();
        logger.info("connected")

    except modbus_tk.modbus.ModbusError as exc:
        logger.error("%s- Code=%d", exc, exc.get_exception_code())

if __name__ == "__main__":
    main()
