# Proxima Command

## Global Registers (16-bit)
- Power Status for each puzzle (binary)
- Solved state for each puzzle (binary)
- Remaining game time
- Play message
- Recharge / Standby

## Global/Game Variables
| Name  |  Type  | Value |
| ----  |  ----  | ----- |
| STAGE |  Array of String | |
| PUZZLE | Structure | |
| ACTIVE | | 1 |
| STANDBY | | 2 |
| SOLVED | | 3 |
| OFFLINE | | 4 |
| FAILURE | | 5 |
| SYSTEM_ERROR | | 6 |
| ERROR | | 7 |
| SYNCRONIZED | | 8 |
| ON | | 9 |
| OFF | | 10 |

```cpp
struct Puzzle {
    int address;
    int status;
    bool forced; // default is false
    int totalPower; // Watts
    int startTime;
    int endTime;
    int * messages;
}

```

## Sources
### Docs & Datasheets
- [Modicon Modbus Protocol Reference Guide](http://modbus.org/docs/PI_MBUS_300.pdf)
- [MODBUS over Serial Line Specification & Implementation guide V1.0 ](http://www.modbus.org/docs/Modbus_over_serial_line_V1.pdf)
### Libraries
- [Arduino Modbus](https://github.com/arduino-libraries/ArduinoModbus)
- [Speaker](https://www.xtronical.com/the-dacaudio-library-download-and-installation/)
- [Event Manager] https://github.com/igormiktor/arduino-EventManager
