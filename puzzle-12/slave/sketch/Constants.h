#define PIN_RX_1 9
#define PIN_TX_1 5
#define PIN_RX_2 10
#define PIN_TX_2 13
#define PIN_ENCODER_A 14
#define PIN_ENCODER_B 38
#define PIN_485_EN 15
#define PIN_INPUT_1 4
#define PIN_INPUT_2 33
#define PIN_INPUT_3 18
#define PIN_NEOPIXEL 26
#define PIN_SPEAKER 25
#define PIN_RELAY_1 23
#define PIN_RELAY_2 19
#define PIN_SWITCH_1 27
#define PIN_SWITCH_2 32
#define PIN_SWITCH_3 12
#define PIN_ANALOG_INPUT_1 34
#define PIN_ANALOG_INPUT_2 35
#define PIN_ANALOG_INPUT_3 37
#define PIN_AMPLIFIER 2
#define ADDR_BUTTON_1 4 // 0x24
#define ADDR_BUTTON_2 0 // 0x20
#define ADDR_SEVENSEGMENT 0x72
#define ADDR_SLAVE 12

#define SERIAL_BAUDRATE 9600
#define LED_COUNT 1
#define COLOR_SATURATION 128

#define PWM_SPEAKER_FREQUENCY 1500
#define PWM_SPEAKER_CHANNEL 0
#define PWM_SPEAKER_RESOLUTION 8
#define PWM_SPEAKER_DUTYCYCLE 200

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
  COUNTING = 24,
  DISABLE = 26,
  ENABLE = 27,
  START_TIMER = 28,
  CHARGING = 29,
  FULL = 30,
  EMPTY = 31,
  TRANSMITTED = 32,
  KEY_ENTERED = 33
};

extern String keyForCodeReader = "198362*";

#define NUMBER_OF_SOUNDS 2
enum {  // enumeration from 0, 1, 2 ...
  SOUND_POWER_UP,
  SOUND_POWER_DOWN
};

enum REGISTER_INDEX {
  REG_DATA_STATE = 0,
  REG_POWER_STATE = 1,
  REG_MASTER_FORCE = 2,
  REG_MASTER_COMMAND = 3,
  REG_MASTER_CONFIRM = 4,
  REG_SLAVE_STATE = 5,
  REG_SLAVE_CODE_READER_STATE = 6,
  REG_SLAVE_SPEAKER_STATE = 7
};

enum COMMAND {
  CMD_ENABLE_CODE_READER = 1,
  CMD_DISABLE_CODE_READER = 2,
  CMD_ENABLE_SPEAKER = 3,
  CMD_DISABLE_SPEAKER = 4
};

typedef struct {
  uint8_t address = ADDR_SLAVE;
  STATE state = INITIALIZED;
  bool forced = false;
  int totalPower = 10;
  uint8_t numberOfRegisters = 10;
  uint16_t registers[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  unsigned long startTime = 0;
  unsigned long endTime = 0;
  unsigned long timer = 0;
  unsigned long counter = 0;
} Puzzle;
