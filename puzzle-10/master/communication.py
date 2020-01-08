import serial
import modbus_tk
import modbus_tk.defines as cst
from modbus_tk import modbus_rtu
import config as cfg

logger = modbus_tk.utils.create_logger("console")
master = None

try:
    #Connect to the slave
    master = modbus_rtu.RtuMaster(
        serial.Serial(port=cfg.modbus['port'], baudrate=cfg.modbus['baudrate'], bytesize=cfg.modbus['bytesize'], parity=cfg.modbus['parity'], stopbits=cfg.modbus['stopbits'], xonxoff=cfg.modbus['xonxoff'])
    )
    master.set_timeout(cfg.modbus['timeout'])
    master.set_verbose(cfg.modbus['verbose'])
    
except modbus_tk.modbus.ModbusError as exc:
    logger.error("%s- Code=%d", exc, exc.get_exception_code())

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