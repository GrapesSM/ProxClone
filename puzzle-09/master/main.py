from flask import Flask
from flask import render_template, request, redirect, url_for
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
    users = User.select()
    return render_template('index.html', users=users)

@app.route('/user/<string:username>')
def show_user_profile(username):
    user = User.get_or_none(User.username == username)
    return 'User %s' % user.username if user else 'User not found'

@app.route('/user', methods=['POST'])
def create_user():
    if (not request.form['username']):
        return 'Username is required'
    user = User(username = request.form['username'])
    user.save()
    return redirect(url_for('index'))

@app.route('/user/clear')
def clear_all():
    User.delete().execute()
    return redirect(url_for('index'))

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

@app.route('/led1', methods=['POST'])
def set_color1():
    if (not request.form['color']):
        return 'Pick color'
    
    color = request.form['color'].lstrip('#')
    colorRGB = tuple(int(color[i:i+2], 16) for i in (0, 2, 4))
    logger.info(colorRGB)

    try:
        master.execute(1, cst.READ_HOLDING_REGISTERS, 0, 4)
        master.execute(1, cst.WRITE_MULTIPLE_REGISTERS, 0, output_value=[1, colorRGB[0], colorRGB[1], colorRGB[2]])
    except modbus_tk.modbus.ModbusError as exc:
        logger.error("%s- Code=%d", exc, exc.get_exception_code())
    return redirect(url_for('index'))

@app.route('/about')
def about():
    return 'About Page'

@app.errorhandler(404)
def not_found(error):
    return '404 Not Found', 404

if __name__ == "__main__":
   app.run(host='0.0.0.0', port=5000, debug=True)