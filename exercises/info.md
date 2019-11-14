## Example 1
Master (PC) communicates to slave (Arduino Mega) using ModBus protocol RS-485. Arduino is loaded with a program that lights up 3 LEDs depending on values received from the master.
MAX 485 chip wired with arduino based on [the circuit](https://github.com/Jamshedshoh/proxima/blob/master/exercises/sources/Ff8Vo.png).
[Modbus Tester software](https://github.com/Jamshedshoh/proxima/blob/master/tools/Tester.exe) is used to read from/write to registers, and there is a [screenshot](https://github.com/Jamshedshoh/proxima/blob/master/exercises/sources/Modbus%20tester%20app%20screenshot.PNG) of software used for the master.   

## Example 2
A master (Arduino Mega 1) communicates to a slave (Arduino Mega 2) using ModBus protocol RS-485. The slave is uploaded with a program that lights up LEDs depending on values received from the master. The master reads from the built-in Serial input and sends a read/write query to the slave's registors to control the state of the slave's LEDs. 
MAX 485 chip wired with arduino based on [the circuit](https://github.com/Jamshedshoh/proxima/blob/master/exercises/sources/Ff8Vo.png). Two chips connected with each other using A to A and B to B pins.

## Example 3
A master (Raspberry Pi) comminicates to a slave (Arduino Uno) using ModBus protocol RS-485. The slave is uploaded with a program that sequentially lights up NeoPixels depending on the RGB colors received from the master. 
TC485S chip (converter) is used with Arduino to communicate via ModBus protocol RS-485. [USB to RS485 Converter Adapter](https://potentiallabs.com/cart/image/cache/catalog/Latest%20components/usb%20to%20485%20converter_01-800x800.jpg) is used with Raspberry PI to communicate with the slave. The adapter is wired with the chip using A to A and B to B pins. 

### Libraries
- [Modbus Master Slave for Arduino](https://github.com/smarmengol/Modbus-Master-Slave-for-Arduino)
- [Modbus TX](https://github.com/ljean/modbus-tk)
