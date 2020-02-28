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
#define PIN_SWITCH_3 25
#define PIN_ANALOG_INPUT_1 34
#define PIN_ANALOG_INPUT_2 35
#define PIN_ANALOG_INPUT_3 37
#define PIN_AMPLIFIER 2
#define ADDR_SWITCH_1 0 // 0x20
#define ADDR_SWITCH_2 4 // 0x24
#define ADDR_SEVENSEGMENT 0x71
#define ADDR_SLAVE 9

#define SERIAL_BAUDRATE 38400
#define LED_COUNT 19
#define COLOR_SATURATION 128

#define PWM_FREQUENCY 1500
#define PWM_CHANNEL 0
#define PWM_RESOLUTION 8
#define PWM_DUTYCYCLE 200

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

enum REGISTER_INDEX {
  REG_DATA_STATE = 0,
  REG_POWER_STATE = 1,
  REG_MASTER_FORCE = 2,
  REG_MASTER_COMMAND = 3,
  REG_MASTER_CONFIRM = 4,
  REG_SHIP_PREP_AUX_POWER_STATE = 5,
  REG_SHIP_PREP_AUX_STATE = 6,
  REG_SLAVE_BATTERY_MATRIX_STATE = 7,
  REG_SLAVE_GENERATOR_STATE = 8,
  REG_SLAVE_SHIP_PREP_AUX_SPEAKER_STATE = 9,
  REG_ENERGY_SUPPLEMENTAL_STATE = 10,
  REG_SLAVE_SYNCRO_READER_INPUT_KEY = 11
};


enum COMMAND {
  CMD_ENABLE_CODE_READER = 1,
  CMD_DISABLE_CODE_READER = 2,
  CMD_ENABLE_SPEAKER = 3,
  CMD_DISABLE_SPEAKER = 4,
  CMD_START_TIMER = 5,
  CMD_SET_SYNCRONIZED = 6,
  CMD_SET_BATTERY_MATRIX_SOLVED = 7,
  CMD_SET_POWER_ADJUSTER_SOLVED = 8,
  CMD_SET_GENERATOR_SOLVED = 9,
  CMD_ENABLE_SYNCRO = 10
};

// Number of Lights and Pin Numbers
extern int lightPinForPowerSwitchOfEnergySupplemental = 0;

#define NUMBER_OF_LIGHTS_FOR_SYNCRO_READER 3
extern int lightPinsForSyncroReader[] = {1, 2, 3};

#define NUMBER_OF_LIGHTS_FOR_BATTERY_MATRIX 7
extern int lightPinsForBatteryMatrix[] = {11, 12, 13, 14, 15, 16, 17};
#define NUMBER_OF_SWITCHES_1 7
extern int switchPinsForBatteryMatrix[] = {7, 6, 5, 4, 3, 2, 1};
extern int labelsForBatteryMatrix[] = {1, 2, 3, 4, 5, 6, 7};
extern int keyForBatteryMatrix = 46725;

#define NUMBER_OF_LIGHTS_FOR_GENERATOR 7
extern int lightPinsForGenerator[] = {4, 5, 6, 7, 8, 9, 10};
#define NUMBER_OF_SWITCHES_2 7
extern int switchPinsForGenerator[] = {7, 6, 5, 4, 3, 2, 1};
extern int labelsForGenerator[] = {1, 2, 3, 4, 5, 6, 7};
extern int keyForGenerator = 46725;

extern int lightPinForPowerSwitchOfShipPrep = 18;

extern int keyForPowerAdjuster = 656;

#define NUMBER_OF_SOUNDS 3
enum {  // enumeration from 0, 1, 2 ...
  SOUND_POWER_UP,
  SOUND_POWER_DOWN,
  SOUND_KEY_INSERT
};