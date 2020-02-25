#define PIN_RX_1 9
#define PIN_TX_1 5
#define PIN_RX_2 10
#define PIN_TX_2 13
#define PIN_ENCODER_A 14
#define PIN_ENCODER_B 38
#define PIN_485_EN 15
#define PIN_INPUT_1 4
#define PIN_INPUT_2 33
#define PIN_INPUT_3 // 18
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
#define ADDR_SEVENSEGMENT_1 0x71
#define ADDR_SEVENSEGMENT_2 0x72
#define ADDR_SLAVE 1

#define SERIAL_BAUDRATE 38400
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
  COUNTING = 24,
  DISABLE = 26,
  ENABLE = 27,
  START_TIMER = 28,
  CHARGING = 29,
  FULL = 30,
  EMPTY = 31
};

// Number of Lights and Pin Numbers
extern int lightPinForPowerLightIndicator = 0;

#define NUMBER_OF_LIGHTS_FOR_BAR_INDICATOR 7
extern int lightPinsForBarIndicator[] = {7, 6, 5, 4, 3, 2, 1};

extern int lightPinForPinIndicator = 8;

#define NUMBER_OF_LIGHTS_FOR_LIGHT_EFFECT 18
extern int lightPinsForLightEffect[] = {9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26};

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
  REG_SLAVE_POWER_ADJUSTER_STATE = 6,
  REG_SLAVE_BATTERY_STATE = 7,
  REG_SLAVE_POWER_LIGHT_INDICATOR_STATE = 8,
  REG_SLAVE_SPEAKER_STATE = 9,
  REG_SLAVE_LIGHT_EFFECT_STATE = 10,
  REG_SLAVE_BATTERY = 11,
  REG_SLAVE_SUPPLY = 12,
  REG_SLAVE_DEMAND = 13,
  REG_SLAVE_LIGHT_EFFECT_PATTERN_NUMBER = 14
};

typedef struct {
  uint8_t address = ADDR_SLAVE;
  STATE state = OFF;
  bool forced = false;
  int totalPower = 10;
  uint8_t numberOfRegisters = 15;
  uint16_t registers[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  unsigned long startTime = 0;
  unsigned long endTime = 0;
  unsigned long timer = 0;
  unsigned long counter = 0;
} Puzzle; 

#define FAILURE_PERIOD_1 10000
#define FAILURE_PERIOD_2 20000
#define FAILURE_PERIOD_3 30000
