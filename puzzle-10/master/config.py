#!/usr/bin/env python3

server = {
    'host': '127.0.0.1',
    'port': 5000,
    'debug': True,
    'static_path': 'public/' }

modbus = {
    # 'port': '/dev/ttyUSB0',
    'port': 'COM7',
    'baudrate': 19200,
    'bytesize': 8,
    'parity': 'N',
    'stopbits': 1,
    'xonxoff': 0,
    'timeout': 5.0,
    'verbose': True }

db = {
    'sqlite': {
        'file_path': 'local.db' },
    'admin': {
        'username': 'administrator',
        'password': 'proxima123'} }

puzzles = {
    'power_panel': {
        'address': 3,
        'registers' : [],
        'number_of_registers': 10,
        'status_code': ''},
    'ship_prep_aux': {
        'address': 9,
        'registers' : [],
        'number_of_registers': 10,
        'status_code': ''},
    'prep_status': {
        'address': 10,
        'registers' : [],
        'number_of_registers': 10,
        'status_code': ''},
    'datamatic': {
        'address': 4,
        'registers' : [],
        'number_of_registers': 10,
        'status_code': ''},
    'lasergrid': {
        'address': 11,
        'registers' : [],
        'number_of_registers': 10,
        'status_code': ''},
    'life_support': {
        'address': 8,
        'registers' : [],
        'number_of_registers': 10,
        'status_code': ''},
    'safeomatic': {
        'address': 5,
        'registers' : [],
        'number_of_registers': 10,
        'status_code': ''},
    'status_board': {
        'address': 2,
        'registers' : [],
        'number_of_registers': 10,
        'status_code': ''}
}

