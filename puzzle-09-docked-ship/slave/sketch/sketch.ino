#include "Constants.h"
#include "SPIFFS.h"
#include "lib/ModbusRtu.h"
#include "NeoPixelBus.h"
#include <Adafruit_MCP23017.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <ESP32Encoder.h>
#include "lib/EnergySupplemental.h"
#include "lib/ShipPrepAux.h"

Puzzle puzzle;

struct Parts {
  Modbus * slave;
  NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip;
  Adafruit_MCP23017 mcp1;
  Adafruit_MCP23017 mcp2;
  ESP32Encoder encoder;
  Adafruit_7segment matrix = Adafruit_7segment();
} parts;

Modbus slave(puzzle.address, 2, PIN_485_EN);
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(LED_COUNT, PIN_NEOPIXEL);

EnergySupplemental::Components esComponents;
ShipPrepAux::Components spComponents;

void setupEnergySupplemental();
void setupShipPrepAux();

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
//  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
//    clock_prescale_set(clock_div_1);
//  #endif

  // Setup NeoPixelBus for all lights
  parts.strip = &strip;
  parts.strip->Begin();
  parts.strip->Show();

  // Setup and Init Encoders
  ESP32Encoder::useInternalWeakPullResistors = false;
  //-- attach pins for use as encoder pins
  parts.encoder.attachHalfQuad(PIN_ENCODER_A, PIN_ENCODER_B);
  //-- adjust starting count value to 0
  parts.encoder.clearCount();
  parts.encoder.setCount(0);

  // Setup 7 segment LED
  parts.matrix.begin(ADDR_SEVENSEGMENT);

  // Setup power switch
  pinMode(PIN_SWITCH_1, INPUT);
  pinMode(PIN_SWITCH_2, INPUT);

  // Setup pins for bottom I2C switches
  parts.mcp1.begin(ADDR_SWITCH_1);
  for (uint8_t i = 1; i <= NUMBER_OF_SWITCHES_1; i++) {
    parts.mcp1.pinMode(i, INPUT);
  }

  // Setup Pins for upper I2C switches
  parts.mcp2.begin(ADDR_SWITCH_2);
  for (uint8_t i = 1; i <= NUMBER_OF_SWITCHES_2; i++) {
    parts.mcp2.pinMode(i, INPUT);
  }

  // Setup speaker pins
  pinMode(PIN_SPEAKER, OUTPUT);
  pinMode(PIN_AMPLIFIER, OUTPUT);
  digitalWrite(PIN_AMPLIFIER, LOW);

  setupEnergySupplemental();
  setupShipPrepAux();

    // Setup Task functions
  xTaskCreatePinnedToCore(
    runTaskFunction,   /* Task function. */
    "RunTask",     /* name of task. */
    110000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &runTask,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */                  

//  delay(500);

  xTaskCreatePinnedToCore(
    showTaskFunction,   /* Task function. */
    "ShowTask",     /* name of task. */
    40000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &showTask,      /* Task handle to keep track of created task */
    1);
    
//  delay(500);

  esComponents.state = SETUP;
  spComponents.state = SETUP;
}

void loop() {

}

void setupEnergySupplemental()
{
  esComponents.powerAdjuster.set(&parts.encoder, &parts.matrix);
  esComponents.syncroReader.set(parts.strip, lightPinsForSyncroReader, PIN_INPUT_1);
  esComponents.powerSwitch.set(parts.strip, lightPinForPowerSwitchOfEnergySupplemental, PIN_SWITCH_1);
  esComponents.speaker.set(soundFilenames);
}

void setupShipPrepAux()
{
  spComponents.batteryMatrix.set(parts.strip, lightPinsForBatteryMatrix, &parts.mcp2, switchPinsForBatteryMatrix, labelsForBatteryMatrix);
  spComponents.generator.set(parts.strip, lightPinsForGenerator, &parts.mcp1, switchPinsForGenerator, labelsForGenerator);
  spComponents.powerSwitch.set(parts.strip, lightPinForPowerSwitchOfShipPrep, PIN_SWITCH_2);
  spComponents.speaker.set(soundFilenames);
}

//Run Task Function: process changes of puzzle
void runTaskFunction( void * parameters ) {
  Serial.print("Run Task running on core ");
  Serial.println(xPortGetCoreID());

  for(;;) {
    // Map puzzle's values to component's values
    EnergySupplemental::update(puzzle, esComponents);
    ShipPrepAux::update(puzzle, spComponents);

    // State changes
    EnergySupplemental::run(esComponents);
    ShipPrepAux::run(spComponents);

    // Show changes
    EnergySupplemental::show(esComponents);
    ShipPrepAux::show(spComponents);
    
    EnergySupplemental::sound(esComponents);
    ShipPrepAux::sound(spComponents);
    
    vTaskDelay(10);
  }
}

//Show Task Function: shows changes of puzzle
void showTaskFunction( void * parameters ){
  Serial.print("Show Task running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){  
    // Enable communication to master  
    // if (Serial1.available() > 0)
    // {
    parts.slave->poll( puzzle.registers, puzzle.numberOfRegisters );
    // }
  } 
}
