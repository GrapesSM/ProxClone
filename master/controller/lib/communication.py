import serial
import modbus_tk
import modbus_tk.defines as cst
from modbus_tk import modbus_rtu

logger = modbus_tk.utils.create_logger("console")
master = None

def queryPuzzle(slaveAddress, numberOfRegisters, step=20):
    a = None
    if step > numberOfRegisters:
        step = numberOfRegisters
    numberOfQueries = (int)(numberOfRegisters / step)
    try:
        for i in range(numberOfQueries):
            if not a:
                a = master.execute(slaveAddress, cst.READ_HOLDING_REGISTERS, i * step, step)
            else:
                a += master.execute(slaveAddress, cst.READ_HOLDING_REGISTERS, i * step, step)
        if (numberOfRegisters - numberOfQueries * step) > 0:
            a += master.execute(slaveAddress, cst.READ_HOLDING_REGISTERS, numberOfQueries * step, numberOfRegisters - numberOfQueries * step)
    except:
        logger.error("Failure to execute query to the puzzle with slave address " + str(slaveAddress))
    return a