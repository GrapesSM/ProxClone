#!/usr/bin/env python3

server = {
    'host': '127.0.0.1',
    'port': 5000,
    'debug': True,
    'static_path': 'static/' }

modbus = {
    # 'port': '/dev/ttyUSB0',
    'port': 'COM5',
    'baudrate': 38400,
    'bytesize': 8,
    'parity': 'N',
    'stopbits': 1,
    'xonxoff': 0,
    'timeout': 5.0,
    'verbose': False }

db = {
    'sqlite': {
        'file': 'local.db' },
    'admin': {
        'username': 'administrator',
        'password': 'proxima123'} }

puzzles = {
    'power_control': {
        'slave_id': 1,
        'registers' : [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        'status_code': ''},
    'datamatic': {
        'slave_id': 4,
        'registers' : [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],   
        'status_code': ''},
    'docked_ship': {
        'slave_id': 9,
        'registers' : [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        'status_code': ''},
    'prep_status': {
        'slave_id': 10,
        'registers' : [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        'status_code': ''},
    'lasergrid': {
        'slave_id': 11,
        'registers' : [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        'status_code': ''},
    'life_support': {
        'slave_id': 8,
        'registers' : [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        'status_code': ''},
    'safeomatic': {
        'slave_id': 5,
        'registers' : [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        'status_code': ''},
    'status_board': {
        'slave_id': 2,
        'registers' : [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        'status_code': ''},
    'keypad': {
        'slave_id': 12,
        'registers' : [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        'status_code': ''}
}

