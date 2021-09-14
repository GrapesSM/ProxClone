# Proxima Command

## Global Registers (16-bit)
- Power Status for each puzzle (binary)
- Solved state for each puzzle (binary)
- Remaining game time
- Play message
- Recharge / Standby

## Puzzle and Component States
```cpp
enum STATE {
  OFF = 0,
  ON = 1,
  ACTIVE = 2,
  STANDBY = 3,
  SOLVED = 4,
  OFFLINE = 5,
  FAILURE = 6,
  SYSTEM_ERROR = 7,
  ERROR = 8,
  SYNCRONIZED = 9,
  INITIALIZING = 10,
  INITIALIZED = 11,
  START = 12,
  END = 13,
  READING = 14,
  PLAYING = 15,
  BALANCED = 16,
  SETUP = 17,
  FLASH = 18,
  UNBALANCED = 19,
  ALARM = 20,
  RESET = 21,
  UNSOLVED = 22,
  DONE = 23,
  DISABLE = 24,
  ENABLE = 25,
  PAUSE = 26,
  COUNTING = 27,
  START_TIMER = 28,
  CHARGING = 29,
  FULL = 30,
  EMPTY = 31,
  OPEN = 32,
  CLOSED = 33,
  NODATA = 34,
  TRANSMITTED = 35,
  KEY_ENTERED = 36,
  DETECTED = 37,
  OUT_OF_RANGE = 38
};
```

## Puzzle Registers
```cpp
typedef struct {
  uint8_t address = ADDR_SLAVE;
  STATE state;
  uint8_t numberOfRegisters = 20;
  uint16_t registers[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
} Puzzle;
```

## Puzzle Main Commands
```cpp
enum COMMAND {
  CMD_NONE = 0,
  CMD_ENABLE = 1,
  CMD_DISABLE = 2,
  CMD_RESET = 3,
  CMD_PAUSE = 4,
  // ... please look constants files in every puzzle folder for specific commands
};
```


## Sources
### Docs & Datasheets
- [Modicon Modbus Protocol Reference Guide](http://modbus.org/docs/PI_MBUS_300.pdf)
- [MODBUS over Serial Line Specification & Implementation guide V1.0 ](http://www.modbus.org/docs/Modbus_over_serial_line_V1.pdf)
### Libraries
- [Arduino Modbus](https://github.com/arduino-libraries/ArduinoModbus)
- [Speaker](https://www.xtronical.com/the-dacaudio-library-download-and-installation/)
- [Event Manager] https://github.com/igormiktor/arduino-EventManager
- [ESP32Encoder] https://github.com/madhephaestus/ESP32Encoder
