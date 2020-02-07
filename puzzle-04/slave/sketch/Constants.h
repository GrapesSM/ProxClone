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
#define ADDR_BUTTON_1 4 // 0x24
#define ADDR_BUTTON_2 0 // 0x20
#define ADDR_SEVENSEGMENT 0x72
#define ADDR_SLAVE 4

#define SERIAL_BAUDRATE 38400
#define LED_COUNT 19
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
    INITIALIZED = 11,
    NOT_INITIALIZED = 12,
    READING = 15
};

// Number of Lights and Pin Numbers
extern int lightPinForPowerSwitch = 0;

#define NUMBER_OF_BUTTONS_1 5
extern int buttonPins1[] = {7, 6, 5, 4, 3};

#define NUMBER_OF_BUTTONS_2 5
extern int buttonPins2[] = {7, 6, 5, 4, 3};

extern char buttonLabels[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

extern String keyForCodeReader1 = "65432";
extern String keyForCodeReader2 = "23456";


#define NUMBER_OF_LIGHTS_FOR_LIGHT_EFFECT 18
extern int lightPinsForLightEffect[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18};
