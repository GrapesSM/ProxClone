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
extern int lightPinsForShipPrepStatus[] = {0, 1};

#define NUMBER_OF_LIGHTS_FOR_LIFE_SUPPORT 2
extern int lightPinsForLifeSupport[] = {0, 1};

#define NUMBER_OF_LIGHTS_FOR_BLAST_DOOR 2
extern int lightPinsForBlastDoor[] = {0, 1};

#define NUMBER_OF_LIGHTS_FOR_LASER_GRID 2
extern int lightPinsForLaserGrid[] = {0, 1};

typedef struct {
  uint8_t address = ADDR_SLAVE;
  STATE state;
  bool forced = false;
  int totalPower = 10;
  uint8_t numberOfRegisters = 20;
  uint16_t registers[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  unsigned long startTime = 0;
  unsigned long endTime = 0;
  unsigned long timer = 0;
  unsigned long counter = 0;
} Puzzle;