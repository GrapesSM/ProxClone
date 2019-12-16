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
#define ADDR_SLAVE 1

#define SERIAL_BAUDRATE 19200
#define LED_COUNT 18
#define COLOR_SATURATION 128

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
    INITIALIZED = 11
};

// Number of Lights and Pin Numbers
extern int lightPinForPowerSwitch = 0;