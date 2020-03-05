#define PIN_RX_1 2
#define PIN_TX_1 4
#define PIN_RX_2 5
#define PIN_TX_2 10
#define PIN_ENCODER_A 33
#define PIN_ENCODER_B 14
#define PIN_485_EN 15
#define PIN_INPUT_1 13
#define PIN_INPUT_2 9
#define PIN_INPUT_3 18
#define PIN_NEOPIXEL 26
#define PIN_SPEAKER 12
#define PIN_RELAY_1 23
#define PIN_RELAY_2 19
#define PIN_SWITCH_1 27
#define PIN_SWITCH_2 32
#define PIN_SWITCH_3 25
#define PIN_ANALOG_INPUT_1 34
#define PIN_ANALOG_INPUT_2 35
#define PIN_ANALOG_INPUT_3 37
#define ADDR_SWITCH_1 0 // 0x20
#define NUMBER_OF_SWITCHES_1 7
#define ADDR_SWITCH_2 4 // 0x24
#define NUMBER_OF_SWITCHES_2 7
#define ADDR_SEVENSEGMENT 0x71
#define ADDR_SLAVE 2

#define SERIAL_BAUDRATE 9600
#define LED_COUNT 18
#define COLOR_SATURATION 128

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

#define NUMBER_OF_LIGHTS_FOR____ 7
extern int lightPinsFor____[] = {1, 2, 3, 4, 5, 6, 7};