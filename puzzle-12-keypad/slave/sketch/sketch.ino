#include "Constants.h"
#include "SPIFFS.h"
#include "lib/ModbusRtu.h"
#include "lib/BlastDoorKeyPad.h"
#include <ESP32Encoder.h>

Puzzle puzzle;

struct Parts {
  Modbus * slave;
  KEYPAD keypad;
  ESP32Encoder encoder;
} parts;

Modbus slave(puzzle.address, 1, PIN_485_EN);

BlastDoorKeypad::Components bdComponents;

void setupBlastDoorKeypad();
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
  parts.slave->begin( SERIAL_BAUDRATE, PIN_RX_1, PIN_TX_1 );
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  //Setup and Init Encoders
  ESP32Encoder::useInternalWeakPullResistors=false;
  //-- attach pins for use as encoder pins
  parts.encoder.attachHalfQuad(PIN_ENCODER_A, PIN_ENCODER_B);
  //-- adjust starting count value to 0
  parts.encoder.clearCount();
  parts.encoder.setCount(0);

  //Keypad setup
  
  if (parts.keypad.begin() == false) 	// Note, using begin() like this will use default I2C address, 0x4B. 
                // You can pass begin() a different address like so: keypad1.begin(Wire, 0x4A).
  {
    Serial.println("Keypad does not appear to be connected. Please check wiring. Freezing...");
    while (1);
  }

  // Setup speaker pins
  pinMode(PIN_SPEAKER, OUTPUT);
  pinMode(PIN_AMPLIFIER, OUTPUT);
  digitalWrite(PIN_AMPLIFIER, HIGH);

  // Output Setup
  pinMode(PIN_OUTPUT_1, OUTPUT);
  digitalWrite(PIN_OUTPUT_1, HIGH);
  
  setupBlastDoorKeypad();

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
    60000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &showTask,      /* Task handle to keep track of created task */
    1);
    
//  delay(500);

  bdComponents.state = SETUP;
}


void loop()
{
  // It should be empty
}

void setupBlastDoorKeypad()
{
  bdComponents.codeReader.set(&parts.keypad, PIN_INPUT_1);
  bdComponents.speaker.set(soundFilenames);
  bdComponents.blastDoorOutput.set(PIN_OUTPUT_1);
}

//Run Task Function: process changes of puzzle
void runTaskFunction( void * parameters ) {
  Serial.print("Run Task running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    // Enable communication to master
    parts.slave->poll( puzzle.registers, puzzle.numberOfRegisters );

    // Map puzzle's values to component's values
    BlastDoorKeypad::update(puzzle, bdComponents);

    // State changes
    BlastDoorKeypad::run(bdComponents);

    // Show changes
    BlastDoorKeypad::show(bdComponents);
    
    vTaskDelay(10);
  } 
}

//Show Task Function: shows changes of puzzle
void showTaskFunction( void * parameters ){
  Serial.print("Show Task running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    BlastDoorKeypad::sound(bdComponents);
  } 
}
