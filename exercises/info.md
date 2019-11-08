## Example 1
Master (PC) to slave (Arduino Mega) communication using ModBus protocol RS-485
Arduino is loaded with a program that lights up 3 LEDs depending on information received from master.
MAX 485 chip wired with arduino based on [the circuit](https://github.com/Jamshedshoh/proxima/blob/master/exercises/sources/Ff8Vo.png).
[Modbus Tester software](https://github.com/Jamshedshoh/proxima/blob/master/tools/Tester.exe) is used to read from/write to registers  


### Libraries
- [Modbus Master Slave for Arduino](https://github.com/smarmengol/Modbus-Master-Slave-for-Arduino)
