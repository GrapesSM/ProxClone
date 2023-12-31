/**
 *  Modbus master example 2:
 *  The purpose of this example is to query several sets of data
 *  from an external Modbus slave device. 
 *  The link media can be USB or RS232.
 *
 *  Recommended Modbus slave: 
 *  diagslave http://www.modbusdriver.com/diagslave.html
 *
 *  In a Linux box, run 
 *  "./diagslave /dev/ttyUSB0 -b 19200 -d 8 -s 1 -p none -m rtu -a 1"
 *   This is:
 *    serial port /dev/ttyUSB0 at 19200 baud 8N1
 *    RTU mode and address @1
 */
#define UBRR1H _SFR_MEM8(0xCD)
#include <ModbusRtu.h>

uint16_t au16data[16]; //!< data array for modbus network sharing
uint8_t u8state; //!< machine state
uint8_t u8query; //!< pointer to message query

/**
 *  Modbus object declaration
 *  u8id : node id = 0 for master, = 1..247 for slave
 *  u8serno : serial port (use 0 for Serial)
 *  u8txenpin : 0 for RS-232 and USB-FTDI 
 *               or any pin number > 1 for RS-485
 */
Modbus master(0,1,4); // this is master and RS-232 or USB-FTDI

/**
 * This is an structure which contains a query to an slave device
 */
modbus_t telegram[2];
int v;
uint16_t values[] = {0, 0, 0};

unsigned long u32wait;

void setup() {
  // telegram 0: read registers
  telegram[0].u8id = 1; // slave address
  telegram[0].u8fct = 3; // function code (this one is registers read)
  telegram[0].u16RegAdd = 0; // start address in slave
  telegram[0].u16CoilsNo = 3; // number of elements (coils or registers) to read
  telegram[0].au16reg = au16data; // pointer to a memory array in the Arduino
  
  // telegram 1: write a single register
  telegram[1].u8id = 1; // slave address
  telegram[1].u8fct = 16; // function code (this one is write a single register)
  telegram[1].u16RegAdd = 0; // start address in slave
  telegram[1].u16CoilsNo = 3; // number of elements (coils or registers) to read
  telegram[1].au16reg = au16data; // pointer to a memory array in the Arduino
  
  master.begin( 9600 ); // baud-rate at 19200
  Serial.begin(9600);
  master.setTimeOut( 5000 ); // if there is no answer in 5000 ms, roll over
  u32wait = millis() + 1000;
  u8state = u8query = 0; 
}

void loop() {
    Serial.print(u8state);
    Serial.print(',');
    Serial.print(u8query);
    Serial.print(',');
    Serial.print(au16data[0]);
    Serial.print(',');
    Serial.print(au16data[1]);
    Serial.print(',');
    Serial.print(au16data[2]);
   
    
    if (Serial.available() > 1) {
      v = Serial.parseInt();
      values[v/10 - 1] = v - (v / 10) * 10;
    } 
    
    switch( u8state ) {
      case 0: 
        if (millis() > u32wait) u8state++; // wait state
        break;
      case 1:            
        master.query( telegram[u8query] ); // send query (only once)
        u8state++;
        u8query++;
        if (u8query > 2) u8query = 0;
        break;
      case 2:
        master.poll(); // check incoming messages
        if (master.getState() == COM_IDLE) {
          u8state = 0;
          u32wait = millis() + 1000;
          for (int i = 0; i < 3; i++) {
            au16data[i] = values[i];
          }
        }
        break;
    }
    Serial.println();
  
}
