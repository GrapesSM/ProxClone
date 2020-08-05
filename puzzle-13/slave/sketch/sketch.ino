#include "Constants.h"
#include "lib/ModbusRtu.h"
#include "lib/LaserBar.h"
#include "Adafruit_VL53L0X.h"
#include "sounds/soundPowerUp.h"
//#include "sounds/soundPowerDown.h"


Puzzle puzzle;

struct Parts {
  Modbus * slave;
  Adafruit_VL53L0X lox = Adafruit_VL53L0X();
  unsigned char* listOfSounds[NUMBER_OF_SOUNDS];
  unsigned int listOfLengthOfSounds[NUMBER_OF_SOUNDS];
} parts;

Modbus slave(puzzle.address, 1, PIN_485_EN);

LaserBar::Components lbComponents;

void setupLaserBar();
void runTaskFunction(void*);
void showTaskFunction(void*);

TaskHandle_t runTask;
TaskHandle_t showTask;


void setup() 
{
  Serial.begin(SERIAL_BAUDRATE);

  // Setup Modbus communication
  parts.slave = &slave;
  parts.slave->begin( SERIAL_BAUDRATE, PIN_RX_1, PIN_TX_1 );
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif
  
  // Adafruit VL53L0X test
  if (!parts.lox.begin()) {
    // Failed to boot VL53L0X
    while(1);
  }
  // ----------------------------------------------------------

  // Setup Laser Output ---------------------------------------
  pinMode(PIN_OUTPUT_1, OUTPUT);
  // ----------------------------------------------------------

  // Setup speaker pins ---------------------------------------
  pinMode(PIN_SPEAKER, OUTPUT);
  ledcSetup(PWM_SPEAKER_CHANNEL, PWM_SPEAKER_FREQUENCY, PWM_SPEAKER_RESOLUTION);
  ledcAttachPin(PIN_SPEAKER, PWM_SPEAKER_CHANNEL);
  pinMode(PIN_AMPLIFIER, OUTPUT);
  digitalWrite(PIN_AMPLIFIER, HIGH);

  parts.listOfSounds[SOUND_POWER_UP] = soundPowerUp;
  parts.listOfLengthOfSounds[SOUND_POWER_UP] = sizeof(soundPowerUp)/sizeof(soundPowerUp[0]);
  //  parts.listOfSounds[SOUND_POWER_DOWN] = soundPowerDown;
  //  parts.listOfLengthOfSounds[SOUND_POWER_DOWN] = sizeof(soundPowerDown)/sizeof(soundPowerDown[0]);
  // ----------------------------------------------------------

  setupLaserBar();

  // Setup Task functions
  xTaskCreatePinnedToCore(
    runTaskFunction,   /* Task function. */
    "RunTask",     /* name of task. */
    100000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &runTask,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */                  

//  delay(500);

  xTaskCreatePinnedToCore(
    showTaskFunction,   /* Task function. */
    "ShowTask",     /* name of task. */
    100000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &showTask,      /* Task handle to keep track of created task */
    1);
  
  lbComponents.state = SETUP;
}

void loop() 
{
  
}

void setupLaserBar()
{
  lbComponents.detector.set(&parts.lox, PIN_OUTPUT_1);
  lbComponents.speaker.set(PIN_SPEAKER, PIN_AMPLIFIER, 65, parts.listOfSounds, parts.listOfLengthOfSounds);
}

//Run Task Function: process changes of puzzle
void runTaskFunction( void * parameters ) {
  Serial.print("Run Task running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    // Enable communication to master
    parts.slave->poll( puzzle.registers, puzzle.numberOfRegisters );
    
    // Map puzzle's values to component's values
    LaserBar::update(puzzle, lbComponents);
  
    // State changes
    LaserBar::run(lbComponents);

    vTaskDelay(10);
  } 
}

//Show Task Fucntion: shows changes of puzzle
void showTaskFunction( void * parameters ){
  Serial.print("Show Task running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    // Show changes
    LaserBar::show(lbComponents);

    vTaskDelay(10);
  } 
}
