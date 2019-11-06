# Proxima Command

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
    int startTime;
    int endTime;
    int * messages;
}

```

## Sources
### Docs & Datasheets
- [Modicon Modbus Protocol Reference Guide](http://modbus.org/docs/PI_MBUS_300.pdf)
### Libraries
- [Arduino Modbus](https://github.com/arduino-libraries/ArduinoModbus)