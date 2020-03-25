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
        'name': 'Power Control',
        'slave_id': 1,
        'registers' : [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        'status_code': ''},
    'status_board': {
        'name': 'Status Board',
        'slave_id': 2,
        'registers' : [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        'status_code': ''},
    'datamatic': {
        'name': 'Datamatic',
        'slave_id': 4,
        'registers' : [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],   
        'status_code': ''},
    'safeomatic': {
        'name': 'Safeomatic',
        'slave_id': 5,
        'registers' : [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        'status_code': ''},
    'rcp': {
        'name': 'RCP'},
    'life_support': {
        'name': 'Life Support',
        'slave_id': 8,
        'registers' : [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        'status_code': ''},
    'docked_ship': {
        'name': 'Docked Ship',
        'slave_id': 9,
        'registers' : [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        'status_code': ''},
    'prep_status': {
        'name': 'Prep Status',
        'slave_id': 10,
        'registers' : [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        'status_code': ''},
    'lasergrid': {
        'name': 'Laser Grid',
        'slave_id': 11,
        'registers' : [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        'status_code': ''},
    'keypad': {
        'name': 'Keypad',
        'slave_id': 12,
        'registers' : [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        'status_code': ''},
    'laserbar': {
        'name': 'Laser Bar',
        'slave_id': 13,
        'registers' : [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        'status_code': ''},
}

