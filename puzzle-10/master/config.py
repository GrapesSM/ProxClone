#!/usr/bin/env python3

server = {
    'host': '127.0.0.1',
    'port': 5000,
    'debug': True,
    'static_path': 'public/' }

modbus = {
    'port': '/dev/ttyUSB0',
    'baudrate': 19200,
    'bytesize': 8,
    'parity': 'N',
    'stopbits': 1,
    'xonxoff': 0,
    'timeout': 5.0,
    'verbose': True }

db = {
    'sqlite': {
        'file_path': 'local.db' } }


