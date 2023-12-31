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
#define ADDR_SEVENSEGMENT 0x74
#define ADDR_SLAVE 2

#define SERIAL_BAUDRATE 19200
#define LED_COUNT 27
#define COLOR_SATURATION 128

#define PIN_OUTPUT_1 18
#define PWM_OUTPUT_1_FREQUENCY 5000
#define PWM_OUTPUT_1_CHANNEL 0 // 0, 1, 2, or 3
#define PWM_OUTPUT_1_RESOLUTION 8 // 8, 10, 12, or 15

#define PWM_SPEAKER_FREQUENCY 1500
#define PWM_SPEAKER_CHANNEL 1
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

// Number of Lights and Pin Numbers
extern int lightPinFor____ = 0;

#define NUMBER_OF_LIGHTS_FOR_SHIP_PREP_STATUS 2
extern int lightPinsForShipPrepStatus[] = {4, 3};

#define NUMBER_OF_LIGHTS_FOR_LIFE_SUPPORT 2
extern int lightPinsForLifeSupportStatus[] = {5, 2};

#define NUMBER_OF_LIGHTS_FOR_BLAST_DOOR 2
extern int lightPinsForBlastDoorStatus[] = {6, 1};

#define NUMBER_OF_LIGHTS_FOR_LASER_GRID 2
extern int lightPinsForLaserGridStatus[] = {7, 0};

enum REGISTER_INDEX {
  REG_MASTER_FORCE = 0,
  REG_MASTER_COMMAND = 1,
  REG_MASTER_BODY = 2,
  REG_SLAVE_MILLIS = 3,
  REG_SLAVE_CONFIRM = 4,
  REG_SLAVE_STATE = 5,
  REG_SLAVE_GAME_POWER_SWITCH_STATE = 6,
  REG_SLAVE_SHIP_PREP_STATUS_STATE = 7,
  REG_SLAVE_LIFE_SUPPORT_STATUS_STATE = 8,
  REG_SLAVE_LASER_GRID_STATUS_STATE = 9,
  REG_SLAVE_BLAST_DOOR_STATUS_STATE = 10,
  REG_SLAVE_COUNTDOWN_STATE = 11,
  REG_SLAVE_COUNTDOWN_VALUE = 12,
  REG_SLAVE_COUNTDOWN = 13,
  REG_SLAVE_SPEAKER_STATE = 14
};

typedef struct {
  uint8_t address = ADDR_SLAVE;
  STATE state;
  uint8_t numberOfRegisters = 20;
  uint16_t registers[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
} Puzzle;

enum COMMAND {
  CMD_NONE = 0,
  CMD_ENABLE = 1,
  CMD_DISABLE = 2,
  CMD_RESET = 3,
  CMD_PAUSE = 4,
  CMD_SET_SOLVED = 6,
  CMD_SET_LASER_GRID_SOLVED = 90,
  CMD_SET_DOCKED_SHIP_SOLVED = 91,
  CMD_SET_KEYPAD_SOLVED = 92,
  CMD_SET_LIFE_SUPPORT_SOLVED = 93,
  CMD_RESET_COUNTDOWN_TIME = 94
};

#define NUMBER_OF_SOUNDS 8
#define SOUND_START 0
#define SOUND_FINAL 1
#define SOUND_50MIN 2
#define SOUND_40MIN 3
#define SOUND_30MIN 4
#define SOUND_20MIN 5
#define SOUND_10MIN 6
#define SOUND_1MIN 7

extern String soundFilenames[NUMBER_OF_SOUNDS] = {
  "/Start.wav",
  "/Final.wav",
  "/50min.wav",
  "/40min.wav",
  "/30min.wav",
  "/20min.wav",
  "/10min.wav",
  "/1min.wav"
};
