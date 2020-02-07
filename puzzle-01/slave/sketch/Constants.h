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

#define PIN_OUTPUT_1 18
#define PWM_OUTPUT_1_FREQUENCY 5000
#define PWM_OUTPUT_1_CHANNEL 0 // 0, 1, 2, or 3
#define PWM_OUTPUT_1_RESOLUTION 10 // 8, 10, 12, or 15

#define SERIAL_BAUDRATE 38400
#define LED_COUNT 27
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
    START = 13,
    END = 14,
    READING = 15,
    PLAYING = 16
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