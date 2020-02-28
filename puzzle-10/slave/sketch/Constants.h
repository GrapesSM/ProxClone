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
#define ADDR_SLAVE 10

#define SERIAL_BAUDRATE 38400
#define LED_COUNT 31
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
  DISABLE = 24,
  ENABLE = 25,
  PAUSE = 26,
  COUNTING = 27,
  START_TIMER = 28,
  CHARGING = 29,
  FULL = 30,
  EMPTY = 31,
  OPEN = 32,
  CLOSED = 33
};

// Number of Lights and Pin Numbers
extern int lightPinForPowerSwitch = 0;

#define NUMBER_OF_LIGHTS_FOR_SYNCRO_READER 3
extern int lightPinsForSyncroReader[] = {12, 11, 10};

#define NUMBER_OF_LIGHTS_FOR_BATTERY_MATRIX 3
extern int lightPinsForBatteryMatrix[] = {7, 4, 1};

#define NUMBER_OF_LIGHTS_FOR_ENERGY_SUPP 3
extern int lightPinsForEnergySupp[] = {8, 5, 2};

#define NUMBER_OF_LIGHTS_FOR_GENERATOR 3
extern int lightPinsForGenerator[] = {9, 6, 3};

#define NUMBER_OF_LIGHTS_FOR_LIGHT_EFFECT 18
extern int lightPinsForLightEffect[] = {13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30};

#define NUMBER_OF_SOUNDS 3
enum {  // enumeration from 0, 1, 2 ...
  SOUND_POWER_UP,
  SOUND_POWER_DOWN,
  SOUND_KEY_INSERT
};

enum REGISTER_INDEX {
  REG_DATA_STATE = 0,
  REG_POWER_STATE = 1,
  REG_MASTER_FORCE = 2,
  REG_MASTER_COMMAND = 3,
  REG_MASTER_CONFIRM = 4,
  REG_SLAVE_STATE = 5,
  REG_SLAVE_POWER_SWITCH_STATE = 6,
  REG_SLAVE_BATTERY_MATRIX_STATE = 7,
  REG_SLAVE_ENERGY_SUPP_STATE = 8,
  REG_SLAVE_GENERATOR_STATE = 9,
  REG_SLAVE_SYNCRO_READER_STATE = 10,
  REG_SLAVE_SPEAKER_STATE = 11,
  REG_SLAVE_LIGHT_EFFECT_STATE = 12,
  REG_SLAVE_SYNCRO_READER_INPUT_KEY = 13
};

enum COMMAND {
  CMD_START_TIMER = 1,
  CMD_SET_SYNCRONIZED = 2,
  CMD_SET_BATTERY_MATRIX_SOLVED = 3,
  CMD_SET_ENERGY_SUPP_SOLVED = 4,
  CMD_SET_GENERATOR_SOLVED = 5
};

typedef struct Puzzle {
  uint8_t address = ADDR_SLAVE;
  STATE state = INITIALIZED;
  bool forced = false;
  int totalPower = 10;
  uint8_t numberOfRegisters = 15;
  uint16_t registers[15] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  unsigned long startTime = 0;
  unsigned long endTime = 0;
  unsigned long timer = 0;
  unsigned long counter = 0;
};
