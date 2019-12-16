from flask import Flask
from flask import render_template, request, redirect, url_for
from flask import send_from_directory
from db import User
app = Flask(__name__)

import serial

import modbus_tk
import modbus_tk.defines as cst
from modbus_tk import modbus_rtu

PORT = '/dev/ttyUSB0'

logger = modbus_tk.utils.create_logger("console")
master = None

try:
    #Connect to the slave
    master = modbus_rtu.RtuMaster(
        serial.Serial(port=PORT, baudrate=19200, bytesize=8, parity='N', stopbits=1, xonxoff=0)
    )
    master.set_timeout(5.0)
    master.set_verbose(True)
    
except modbus_tk.modbus.ModbusError as exc:
    logger.error("%s- Code=%d", exc, exc.get_exception_code())

@app.route('/')
def index():
    try:
        a = master.execute(1, cst.READ_HOLDING_REGISTERS, 0, 20)
        a += master.execute(1, cst.READ_HOLDING_REGISTERS, 20, 20)
        a += master.execute(1, cst.READ_HOLDING_REGISTERS, 40, 20)
        a += master.execute(1, cst.READ_HOLDING_REGISTERS, 60, 10)
    except modbus_tk.modbus.ModbusError as exc:
        logger.error("%s- Code=%d", exc, exc.get_exception_code())
    return render_template('index.html', output=a)

@app.route('/led', methods=['POST'])
def set_color():
    if (not request.form['red'] or not request.form['green'] or not request.form['blue']):
        return 'Enter Red and Green and Blue colors'
    
    try:
        master.execute(1, cst.READ_HOLDING_REGISTERS, 0, 4)
        master.execute(1, cst.WRITE_MULTIPLE_REGISTERS, 0, output_value=[1, int(request.form['red']), int(request.form['green']), int(request.form['blue'])])
    except modbus_tk.modbus.ModbusError as exc:
        logger.error("%s- Code=%d", exc, exc.get_exception_code())
    return redirect(url_for('index'))

@app.route('/update', methods=['POST'])
def update():
    #color = request.form['color'].lstrip('#')
    #colorRGB = tuple(int(color[i:i+2], 16) for i in (0, 2, 4))
    inputs = request.form;
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
    return redirect(url_for('index'))

@app.route('/img/<path:path>')
def send_img(path):
    return send_from_directory('templates/img', path)

@app.route('/js/<path:path>')
def send_js(path):
    return send_from_directory('templates/js', path)

@app.route('/css/<path:path>')
def send_css(path):
    return send_from_directory('templates/css', path)

@app.errorhandler(404)
def not_found(error):
    return '404 Not Found', 404

if __name__ == "__main__":
   app.run(host='0.0.0.0', port=5000, debug=True)