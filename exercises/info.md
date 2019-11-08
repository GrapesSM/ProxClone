## Example 1
Master (PC) communicates to slave (Arduino Mega) using ModBus protocol RS-485. Arduino is loaded with a program that lights up 3 LEDs depending on values received from the master.
MAX 485 chip wired with arduino based on [the circuit](https://github.com/Jamshedshoh/proxima/blob/master/exercises/sources/Ff8Vo.png).
[Modbus Tester software](https://github.com/Jamshedshoh/proxima/blob/master/tools/Tester.exe) is used to read from/write to registers, and there is a [screenshot](https://github.com/Jamshedshoh/proxima/blob/master/exercises/sources/Modbus%20tester%20app%20screenshot.PNG) of software used for the master.   


### Libraries
- [Modbus Master Slave for Arduino](https://github.com/smarmengol/Modbus-Master-Slave-for-Arduino)
