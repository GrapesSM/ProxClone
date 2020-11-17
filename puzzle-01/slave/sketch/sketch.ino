#include "Constants.h"
#include "SPIFFS.h"
#include "lib/ModbusRtu.h"
#include "NeoPixelBus.h"
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <ESP32Encoder.h>
#include "lib/PowerControl.h"

Puzzle puzzle;

struct Parts {
  Modbus * slave;
  NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip;
  ESP32Encoder encoder;
  Adafruit_7segment matrix1 = Adafruit_7segment(); 
  Adafruit_7segment matrix2 = Adafruit_7segment();
} parts;

Modbus slave(puzzle.address, 1, PIN_485_EN);
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(LED_COUNT, PIN_NEOPIXEL);

PowerControl::Components pcComponents;

void setupPowerPanel();
void runTaskFunction(void*);
void showTaskFunction(void*);

TaskHandle_t runTask;
TaskHandle_t showTask;

void setup() 
{
  Serial.begin(SERIAL_BAUDRATE);
  while (!Serial);

  SPIFFS.begin();
  
  // Setup Modbus communication
  parts.slave = &slave;
  parts.slave->begin( SERIAL_BAUDRATE, PIN_RX_2, PIN_TX_2 );
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  // Setup NeoPixelBus for all lights
  parts.strip = &strip;
  parts.strip->Begin();
  parts.strip->Show();

  // Setup and Init Encoders
  ESP32Encoder::useInternalWeakPullResistors=false;
  //-- attach pins for use as encoder pins
  parts.encoder.attachHalfQuad(PIN_ENCODER_A, PIN_ENCODER_B);
  //-- adjust starting count value to 0
  parts.encoder.clearCount();
  parts.encoder.setCount(0);
  
  // Setup 7 segment LED
  parts.matrix1.begin(ADDR_SEVENSEGMENT_1);
  parts.matrix2.begin(ADDR_SEVENSEGMENT_2);
  
  // Setup power switch
  pinMode(PIN_SWITCH_1, INPUT);
  pinMode(PIN_SWITCH_2, INPUT);
  pinMode(PIN_SWITCH_3, INPUT);
  
  // configure PWM functionalitites for Analog Voltage Gauge
  /*
   * channel = 0 or 1 or 2 or 3
   * frequency = 5000
   * resolution = 8 or 10 or 12 or 15
   * PWM value = from 0 (0%) to 1024 (100%)
   */
  pinMode(PIN_OUTPUT_1, OUTPUT);
  ledcSetup(PWM_OUTPUT_1_CHANNEL, PWM_OUTPUT_1_FREQUENCY, PWM_OUTPUT_1_RESOLUTION);
  ledcAttachPin(PIN_OUTPUT_1, PWM_OUTPUT_1_CHANNEL);

  // Setup speaker pins
//  pinMode(PIN_AMPLIFIER, OUTPUT);
//  digitalWrite(PIN_AMPLIFIER, HIGH);
  
  setupPowerControl();
  // Setup Task functions
  xTaskCreatePinnedToCore(
    runTaskFunction,   /* Task function. */
    "RunTask",     /* name of task. */
    90000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &runTask,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */                  

//  delay(500);

  xTaskCreatePinnedToCore(
    showTaskFunction,   /* Task function. */
    "ShowTask",     /* name of task. */
    90000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &showTask,      /* Task handle to keep track of created task */
    1);
    
//  delay(500);
  pcComponents.state = SETUP;
}

void loop() {}

void setupPowerControl()
{
  pcComponents.powerAdjuster.set(&parts.encoder, &parts.matrix1, &parts.matrix2, PWM_OUTPUT_1_CHANNEL);
  pcComponents.powerLightIndicator.set(parts.strip, lightPinForPowerLightIndicator);
  pcComponents.battery.set(parts.strip, lightPinsForBarIndicator);
  pcComponents.lightEffect.set(parts.strip, lightPinsForLightEffect);
  pcComponents.speaker.set(soundFilenames);
}

//Run Task Function: process changes of puzzle
void runTaskFunction( void * parameters ) {
  Serial.print("Run Task running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    // Map puzzle's values with component's values
    PowerControl::update(puzzle, pcComponents);

    // State changes
    PowerControl::run(pcComponents);

    // Show changes
    PowerControl::show(pcComponents);

    vTaskDelay(10);
  }
}

//Show Task Fucntion: shows changes of puzzle
void showTaskFunction( void * parameters ){
  Serial.print("Show Task running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    // Play sounds
    PowerControl::sound(pcComponents);

    // Enable communication to master
    parts.slave->poll( puzzle.registers, puzzle.numberOfRegisters );
  } 
}
