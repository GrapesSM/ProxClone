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
#define PIN_AMPLIFIER 2
#define ADDR_SWITCH_1 0 // 0x20
#define NUMBER_OF_SWITCHES_1 7
#define ADDR_SWITCH_2 4 // 0x24
#define NUMBER_OF_SWITCHES_2 7
#define ADDR_SEVENSEGMENT 0x71
#define ADDR_SLAVE 8

#define SERIAL_BAUDRATE 9600
#define LED_COUNT 18
#define COLOR_SATURATION 128

#define PWM_FREQUENCY 1500
#define PWM_CHANNEL 0
#define PWM_RESOLUTION 8
#define PWM_DUTYCYCLE 200

enum STATE {
    ACTIVE = 1,
    STANDBY = 2,
    SOLVED = 3,
    OFFLINE = 4,
    FAILURE = 5,
    SYSTEM_ERROR = 6,
    ERROR = 7,
    SYNCRONIZED = 8,
    ON = 9,
    OFF = 10,
    INITIALIZED = 11,
    NOT_INITIALIZED = 12,
    READING = 15,
    PLAYING = 16
};

// Number of Lights and Pin Numbers
extern int lightPinForPowerSwitchOfEnergySupplemental = 0;

#define NUMBER_OF_LIGHTS_FOR_SYNCRO_READER 3
extern int lightPinsForSyncroReader[] = {1, 2, 3};

#define NUMBER_OF_LIGHTS_FOR_BATTERY_MATRIX 7
extern int lightPinsForBatteryMatrix[] = {17, 16, 15, 14, 13, 12, 11};

#define NUMBER_OF_LIGHTS_FOR_GENERATOR 7
extern int lightPinsForGenerator[] = {10, 9, 8, 7, 6, 5, 4};

extern int lightPinForPowerSwitchOfShipPrep = 18;

#define NUMBER_OF_SOUNDS 3
enum {  // enumeration from 0, 1, 2 ...
  SOUND_POWER_UP,
  SOUND_POWER_DOWN,
  SOUND_KEY_INSERT
};