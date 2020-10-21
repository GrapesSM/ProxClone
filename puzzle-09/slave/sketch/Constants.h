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
#define PIN_SPEAKER 25
#define PIN_ANALOG_INPUT_1 34
#define PIN_ANALOG_INPUT_2 35
#define PIN_ANALOG_INPUT_3 37
#define PIN_AMPLIFIER 2
#define ADDR_SWITCH_1 0 // 0x20
#define ADDR_SWITCH_2 4 // 0x24
#define ADDR_SEVENSEGMENT 0x71
#define ADDR_SLAVE 9

#define SERIAL_BAUDRATE 19200
#define LED_COUNT 19
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
  CLOSED = 33,
  NODATA = 34,
  TRANSMITTED = 35,
  KEY_ENTERED = 36,
  DETECTED = 37,
  OUT_OF_RANGE = 38
};

enum REGISTER_INDEX {
  REG_MASTER_FORCE = 0,
  
  REG_MASTER_SP_COMMAND = 1,
  REG_MASTER_SP_BODY = 2,
  REG_SLAVE_SP_MILLIS = 3,
  REG_SLAVE_SP_CONFIRM = 4,
  REG_SLAVE_SP_STATE = 5,
  REG_SLAVE_SP_POWER_SWITCH_STATE = 6,
  REG_SLAVE_SP_BATTERY_MATRIX_STATE = 7,
  REG_SLAVE_SP_GENERATOR_STATE = 8,
  REG_SLAVE_SP_SPEAKER_STATE = 9,
  
  REG_MASTER_ES_COMMAND = 10,
  REG_MASTER_ES_BODY = 11,
  REG_SLAVE_ES_MILLIS = 12,
  REG_SLAVE_ES_CONFIRM = 13,
  REG_SLAVE_ES_STATE = 14,
  REG_SLAVE_ES_POWER_SWITCH_STATE = 15,
  REG_SLAVE_ES_POWER_ADJUSTER_STATE = 16,
  REG_SLAVE_ES_SYNCRO_READER_STATE = 17,
  REG_SLAVE_ES_SPEAKER_STATE = 18,
  REG_SLAVE_ES_SYNCRO_READER_INPUT_KEY = 19
};


enum COMMAND {
  CMD_NONE = 0,
  CMD_ENABLE = 1,
  CMD_DISABLE = 2,
  CMD_RESET = 3,
  CMD_PAUSE = 4,
  CMD_SET_SOLVED = 6,
  CMD_START_TIMER = 7,
  CMD_SET_DS_BATTERY_MATRIX_SOLVED = 20,
  CMD_SET_DS_ENERGY_SUPP_SOLVED = 21,
  CMD_SET_DS_GENERATOR_SOLVED = 22,
  CMD_SET_DS_POWER_ADJUSTER_KEY = 23,
  CMD_SET_DS_BATTERY_MATRIX_KEY= 24,
  CMD_SET_DS_GENERATOR_KEY= 25,
  CMD_SET_DS_SYNCRO_READER_SYNCRONIZED = 26,
  CMD_SET_DS_SYNCRO_READER_WRONG_SOLVED = 27,
  CMD_SET_DS_SYNCRO_READER_COUNTING_PERIOD = 28,
  CMD_ENABLE_DS_SYNCRO_READER = 29
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
extern int keyForBatteryMatrix = 41527;

#define NUMBER_OF_LIGHTS_FOR_GENERATOR 7
extern int lightPinsForGenerator[] = {4, 5, 6, 7, 8, 9, 10};
#define NUMBER_OF_SWITCHES_2 7
extern int switchPinsForGenerator[] = {7, 6, 5, 4, 3, 2, 1};
extern int labelsForGenerator[] = {1, 2, 3, 4, 5, 6, 7};
extern int keyForGenerator = 46275;

extern int lightPinForPowerSwitchOfShipPrep = 18;

extern int keyForPowerAdjuster = 656;

typedef struct {
  uint8_t address = ADDR_SLAVE;
  STATE state;
  uint8_t numberOfRegisters = 20;
  uint16_t registers[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
} Puzzle;


#define NUMBER_OF_SOUNDS 6
#define SOUND_STATION_UP 0
#define SOUND_STATION_DOWN 1
#define SOUND_COUNTDOWN_BEEP 2 
#define SOUND_KEY_SWITCH 3
#define SOUND_SYNCRO_BEEP 4
#define SOUND_SWITCH 5

extern String soundFilenames[NUMBER_OF_SOUNDS] = {
  "/StationPowerUp.wav",
  "/StationPowerDown.wav",
  "/CountdownBeep.wav",
  "/KeySwitch.wav",
  "/SycroBeep.wav",
  "/Switch.wav"
};
