////#include <toneAC.h>

#include <Wire.h>
#include "Adafruit_MCP23017.h"

#define addr1 0 //0x20
#define addr2 4 //0x24

// Encoder pins
#define encoder0PinA  2
#define encoder0PinB  4


// Neopixel setup
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
//#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define PIN 9 //neopixel control pin
#define NUMPIXELS 19 // Number of neopixel on the puzzle
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//neopixel commands

//pixels.clear();
//pixels.setPixelColor(i, pixels.Color(0, 150, 0));
//pixels.show();
//pixels.setBrightness(64);
//pixels.fill(32000, 0, 19);

// 7 Segment
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"  //I2C Holtek chip
Adafruit_7segment matrix = Adafruit_7segment();

// Input #0 is on pin 21 so connect a button or switch from there to ground

Adafruit_MCP23017 mcp1;
Adafruit_MCP23017 mcp2;

int mcpRead[14];
int lastmcpRead[14];
int j;
int i;
unsigned int timer1;
unsigned int timer1Read;
int tonefreq = 1000;

volatile int encoder0Pos = 0;
float lcdDisplay;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
 * This puzzle constantly get load values written by the modbus master and if the total loads are less than then 13 then player must adjust the power supply via the encoder to 
 * to match the load.
 * Load or power demand is shown on Input 7 segment and Power Supply Output is set by the encoder. 
 * if the player adjusts the encoder to make the Input power demand  7seg == Output(supl then puzzle is curretnly solved....
 * however  the puzzle status can change as puzzles gets  solved and then  then the user has to turn on another puzzle. 
 * Alarm condition if imablanced lvl1 -3 seconds of not balance- red l
 * Three seconds after the Draw/Power readouts are not balanced: a red light flashes on the Power Panel 
Six seconds after the Draw/Power readouts are not balanced: an alarm sounds from the Power Panel, and a second red light flashes 
 Nine seconds after the Draw/Power readouts are not balanced: the alarm and two flashing lights turn off, the Power/Draw readouts show ‘0’, and all puzzle stations ‘power down’, and the battery, if not full, recharges 
 If the battery becomes empty at any time, follow the three bullet points above 

 * Light PuzStsLight should follow Puzz stat
 * each puzzle should have  adifferetn power load demand. 
 * the puzzles which are dependant on this are  
 *       Data-matic4, Safe 5, Corridor circuit8,  Corridor, Ship prep9,  Lasers10  
 * 
 * 
 * 
 * 
 */
//in game.h

/* may change these defines to modbus variables which can be written by the master in the future
*/
#define PUZSLVADD 1  //this is the power puzzle slave addr
#define PWRLOADMAX  130
#define BALANCEDNEO  WHITE   // statuslight indicator - may change these to astructure
#define UNBALANCEDNEO  RED
#define IMBALNCEDLY1 3000 //Imablance DELAY bef
#define NUMOFPUZ 16   // this way we can put some status bit in one MODBUS register



struct Game{
    int gamestate;
};

//int PuzOrder[];
//int PuzState[]; //which puzzles are solved


// each puzzle has this
struct Puzzle {
    int address;
    int status;     
    bool forced; // default is false 
    int totalPower; // Watts int loadunits;   // how many power units is this load /10
    int startTime;
    int endTime;
    int * messages;
    
};

//PuzId are also their slave addresses 
enum PuzID{
STSPUZ,   // master
PWRPUZ,
MEDIAPUZ,  // maybe a raspberry pi?- play media
SHELFPUZ,
DMATICPUZ,
SAFEPUZ,
READERPUZ,
CORRCKTPUZ,
CORRPUZ,
SHIPREPUZ,
SHIPSUPPUZ,
LASERPUZ,
KEYPDPUZ,
BLANK1PUZ,
BLANK2PUZ,
BLANK3PUZ
};

//int numOfPuz NUMOFPUZ; // how many devies attached 12 - NB ther are two puzzles associate with ship prep

// these should be modbus registers
//unsigned int getPuzPwrSt=0b0000 1110 1011 0000;  // inidcates a the pwr status  modbus master updates this - each bit is the slave address 
unsigned int getPuzPwrSt=  0b0000000010110000;  // inidcates a the pwr status  modbus master updates this - each bit is the slave address 
unsigned int setPuzPwrSt;  // tells the master  which devices to pwr off if bit is set - each bit is the slave address 

unsigned int getPuzSolved;  // what game puzzles are solved
unsigned int playmsg;       // a register master should read  and post to mediaplayer slave 

unsigned int pushThisPuzIntoSt; // a register  for this puzzle to read to be pushed into a state from the master.




// what the puzzle loads are  x 10 since modbus does do decimals
const unsigned int PuzPwrLoad[NUMOFPUZ]{
0, //STSPUZ,//MSGPUZ, 
0, //PWRPUZ,  // no power required
0,//MEDIAPUZ,   // no power required
0, //SHELFPUZ, // no power required 
15,  //DMATICPUZ,
10, //SAFEPUZ,
0, //READERPUZ,
25, //CORRCKTPUZ,
0, //CORRPUZ,
35, //SHIPREPUZ,
45, // ship supplemental
15, //LASERPUZ,
0, //KEYPDPUZ
0, //BLANK1PUZ,
0, //BLANK2PUZ,
0 //BLANK3PUZ
};  



Puzzle Puz[KEYPDPUZ];

//power puzzle states
enum PwrPuzSt{
  INITIALIZING,  // setting init values & getting setting form master 
  STANDBY,
  BALANCED, //for this puzzle this is the solved stateSOLVED
  TOOMANYLOADS,  // Total power demand of loads is > PWRLOADMAX
  PWRSETTOOLOWALARM, //IMBALANCED1,// input demand doesn't match output demand for less then 3 seconds
  PWRSETTOOLOWALARM2,// input demand doesn't match output demand for 6-9 seconds
  PWRSETTOOHIGHALARM,//IMBALANCED1,// input demand doesn't match output demand for less then 3 seconds
  PWRSETTOOHIGHALARM2,// input demand doesn't match output demand for 6-9 seconds
  RECHARGE// input demand doesn't match output demand for more than 9 second
            // turn off all powered puzzles, "msg system imbalance" and go baback to balance mode and start again 
  
};


unsigned imBalanceStartTmr,imBalanceElapsedTmr; //how long has puzzle been out of balance ;

// variables global to the entire GAME


int PuzSt;   // O not solved , 1 solved , alarm state lvl 1, alarm state 2, alarm 3(  recharge timeout)


int pwrPuzSt=BALANCED; //  puzzle sate for this puzzle
// if powerload == pwrset and less then 13 then status light is white
int  pwrLoad=0;   // total combined load of puzzles turned on 
int  pwrSet=0;    // what knob is set to  start off as balanced

int PuzStsLightInd;  // white or red

//int               // which puzzles are 

volatile byte serCmd; //pob serial command

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


int timer;

void setup() {
  Serial.begin(9600);
 //// tone (10, random(2000,4000), 25);
  // Attach encoder interrupts
  pinMode(encoder0PinA, INPUT);
  digitalWrite(encoder0PinA, HIGH);       // turn on pull-up resistor
  pinMode(encoder0PinB, INPUT);
  digitalWrite(encoder0PinB, HIGH);       // turn on pull-up resistor

  attachInterrupt(0, doEncoder, CHANGE);  // encoder pin on interrupt 0 - pin 2

  // Start i2c expanders
  mcp1.begin(addr1);      // use default address 0
  mcp2.begin(addr2);      // use default address 0


  // 7 segment

  //#ifndef __AVR_ATtiny85__
  //  Serial.begin(9600);
  //  Serial.println("7 Segment Backpack Test");
  //#endif
  matrix.begin(0x71);

  ////////////////////////////
  //#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  //  clock_prescale_set(clock_div_1);
  //#endif
  // END of Trinket-specific code.

  pixels.begin(); // INITIALIZE NeoPixel strip object
  pixels.setBrightness(6);

  // Setup pins for bottom I2C switches
  mcp1.pinMode(1, INPUT);
  mcp1.pinMode(2, INPUT);
  mcp1.pinMode(3, INPUT);
  mcp1.pinMode(4, INPUT);
  mcp1.pinMode(5, INPUT);
  mcp1.pinMode(6, INPUT);
  mcp1.pinMode(7, INPUT);

  // Setup Pins for upper I2C switches
  mcp2.pinMode(1, INPUT);
  mcp2.pinMode(2, INPUT);
  mcp2.pinMode(3, INPUT);
  mcp2.pinMode(4, INPUT);
  mcp2.pinMode(5, INPUT);
  mcp2.pinMode(6, INPUT);
  mcp2.pinMode(7, INPUT);

  int lastRandom;

  for ( i = 1; i <= 25; i++) {
    lastRandom = random(0, 19);
    pixels.setPixelColor(lastRandom, pixels.Color(random(0, 255), random(0, 255), random(0, 255)));
    pixels.show();
    delay(100);
    pixels.setPixelColor(lastRandom, pixels.Color(0, 0, 0));
    pixels.show();
  }

  pixels.setPixelColor(0, pixels.Color(255, 255, 255));
  pixels.setPixelColor(1, pixels.Color(255, 255, 255));
  pixels.show();

  //reset timers since millis started during setup
  timer1Read =  millis();
  timer1 = 0;


Serial.println("PwrPuz start... \n number of puzzles ");
Serial.println( KEYPDPUZ);
  
}

void doEncoder() {
  /* If pinA and pinB are both high or both low, it is spinning
     forward. If they're different, it's going backward.
     For more information on speeding up this process, see
     [Reference/PortManipulation], specifically the PIND register.
  */
  if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB)) {
    encoder0Pos++;
  ////  toneAC (10, random(2000,4000), 25);
    delay(10);
  } else {
    encoder0Pos--;
   //// tone (10, random(2000,4000), 25);
    delay(10);
  }

}

void doEncoder_Expanded() {
  if (digitalRead(encoder0PinA) == HIGH) {   // found a low-to-high on channel A
    if (digitalRead(encoder0PinB) == LOW) {  // check channel B to see which way
      // encoder is turning
      encoder0Pos = encoder0Pos - 1;         // CCW
    }
    else {
      encoder0Pos = encoder0Pos + 1;         // CW
    }
  }
  else                                        // found a high-to-low on channel A
  {
    if (digitalRead(encoder0PinB) == LOW) {   // check channel B to see which way
      // encoder is turning
      encoder0Pos = encoder0Pos + 1;          // CW
    }
    else {
      encoder0Pos = encoder0Pos - 1;          // CCW
    }

  }

}

void loop() {

       //eachtime round loop check if  pwrSet == Pwrload)
      // 
       //CheckBalance(); 


       if (pwrPuzSt!= RECHARGE){
          pwrLoad = checkLoads();
          if (pwrLoad >=PWRLOADMAX )  pwrPuzSt = TOOMANYLOADS; // regardless of 
          else  if(pwrLoad == pwrSet) pwrPuzSt = BALANCED; // regardless of 
          else  if(pwrLoad <= pwrSet) pwrPuzSt =  PWRSETTOOLOWALARM; // regardless of 
          else  if(pwrLoad >= pwrSet) pwrPuzSt = PWRSETTOOHIGHALARM; // regardless of 

       }   
        
        switch(pwrPuzSt){
        case INITIALIZING:


            break;
          
          case STANDBY:


            break;
          case BALANCED: //for this puzzle this is the solved stateSOLVED- 
            //but changes whenenver a different puzzle get turned on or off
            //set lights           
            //

            pwrPuzSt = BALANCED;
            imBalanceStartTmr = imBalanceElapsedTmr = 0; // reset timer
                 
            break;
          
          case TOOMANYLOADS: // too many loads are turned on
                   Serial.println ("TOOMANYLOADS");
            break;
          
          case PWRSETTOOLOWALARM:// input demand doesn't meet output demand for less then 3 seconds              case PWRSETTOOLOWALARM2:// input demand doesn't meet output demand for less then 3 seconds
          case PWRSETTOOLOWALARM2:
              if (imBalanceStartTmr == 0) imBalanceStartTmr = millis(); // first time in 
              else  {
                  imBalanceElapsedTmr = millis()-imBalanceStartTmr;
              }
              // lvl1 after 3 seconds, red lightflash
               if (imBalanceElapsedTmr >3000 && imBalanceElapsedTmr<6000){
                    // REDLIGHT ON  
                    Serial.println ("level 1 alarm");          
               }
               else if (imBalanceElapsedTmr >=6000 && imBalanceElapsedTmr<9000){
                    //lvl2 6-9 seconds Alarm Sounds & Second red light flashes
                     Serial.println ("level 2 alarm");          
               }
              
              else if (imBalanceElapsedTmr >=9000 )  pwrPuzSt = RECHARGE;

              Serial.println ("PWRSETTOOLOWALARM");
              Serial.print ("imBalanceElapsedTmr = ");
              Serial.println (imBalanceElapsedTmr);
              


            break;
         
         case PWRSETTOOHIGHALARM:// input demand doesn't meet output demand for less then 3 seconds
         case PWRSETTOOHIGHALARM2:// input demand doesn't meet output demand for less then 3 seconds
              if (imBalanceStartTmr == 0) imBalanceStartTmr = millis(); // first time in 
                    else  {
                        imBalanceElapsedTmr = millis()-imBalanceStartTmr;
                    }
              
              // lvl1 after 3 seconds, red lightflash
               if (imBalanceElapsedTmr >3000 && imBalanceElapsedTmr<6000){
                    Serial.println ("level 1 alarm");          
                    // REDLIGHT ON  
               }
               else if (imBalanceElapsedTmr >=6000 && imBalanceElapsedTmr<9000){
                    //lvl2 6-9 seconds Alarm Sounds & Second red light flashes
                    Serial.println ("level 2 alarm");          
               }
              
              else if (imBalanceElapsedTmr >=9000 )  pwrPuzSt = RECHARGE;

              //lvl3 9 seconds  = RECHARGE - readouts shoe 0, all ?unsolved? puzzles power down ( stand by )
              Serial.println ("PWRSETTOOHIGH ");
              Serial.print ("imBalanceElapsedTmr = ");
              Serial.println (imBalanceElapsedTmr);


            break;
         
         
          case RECHARGE:
             // shut down all puzzles
             // set readouts to 0
             // turn off lights 
             Serial.println ("RECHARGE");          
             setPuzPwrSt=0; // write modbus register to be picked up by master
                            // but how do we know that the master has read this register so we can clear it?- 
                            // Answer : getPuzPwrst is all
             while(1);


            
            break;
        default:
          Serial.println ("ldefault");          
        break;

         
        }
          






/*

  if (encoder0Pos <= 0) {
    encoder0Pos = 0;
    matrix.println(lcdDisplay / 10, 1);
  } else if (encoder0Pos >= 1010) {
    encoder0Pos = 1010;
  }
  lcdDisplay = encoder0Pos;
  matrix.println(lcdDisplay / 10, 1);
  matrix.writeDisplay();


  // Countdown timer for sync key

  timer1 =  millis() - timer1Read;

  if ((timer1 >= 0) && (timer1 <= 1000)) {
    pixels.setPixelColor(4, pixels.Color(0, 255, 0));
    pixels.show();
  } else if ((timer1 >= 1000) && (timer1 <= 2000)) {
    pixels.setPixelColor(4, pixels.Color(0, 0, 0));
    pixels.show();
  } else if ((timer1 >= 2000) && (timer1 <= 3000)) {
    pixels.setPixelColor(3, pixels.Color(0, 255, 0));
    pixels.show();
  } else if  ((timer1 >= 3000) && (timer1 <= 4000)) {
    pixels.setPixelColor(3, pixels.Color(0, 0, 0));
    pixels.show();
  } else if ((timer1 >= 4000) && (timer1 <= 5000)) {
    pixels.setPixelColor(2, pixels.Color(0, 255, 0));
    pixels.show();
  } else if  ((timer1 >= 5000) && (timer1 <= 6000)) {
    pixels.setPixelColor(2, pixels.Color(0, 0, 0));
    pixels.show();
  } else if  ((timer1 >= 6000) && (timer1 >= 7000)) {
    timer1Read = millis();
    timer1 = 0;
  }



  for ( i = 1; i <= 14; i++) {
    if (i <= 7) {
      mcpRead[i] = mcp1.digitalRead(i);
      if (lastmcpRead[i] != mcpRead[i]) {
        //Serial.print(lastmcpRead[i]); Serial.print("   "); Serial.println(mcpRead[i]);
        tone (10, 4000, 100);
      }
      lastmcpRead[i] = mcpRead[i];

      if (mcp1.digitalRead(i) == HIGH) {

        //        mcpRead[i] = mcp1.digitalRead(i);
        pixels.setPixelColor((NUMPIXELS - 7) - i, pixels.Color(255, 0, 0));
        pixels.show();
      } else {
        mcpRead[i] = 0;
        pixels.setPixelColor((NUMPIXELS - 7) - i, pixels.Color(0, 0, 0));
        pixels.show();
      }
    }
    if (i >= 8) {
      if (mcp2.digitalRead(i - 7) == HIGH) {
        pixels.setPixelColor((NUMPIXELS + 7) - i, pixels.Color(255, 0, 0));
        pixels.show();
      } else {
        pixels.setPixelColor((NUMPIXELS + 7) - i, pixels.Color(0, 0, 0));
        pixels.show();
      }
    }
  }
  i = 0;

  if ((mcpRead[1] == 1) && (mcpRead[2] == 1)) {
    pixels.fill(32000, 0, 19);
    //pixels.clear();
    pixels.show();
    matrix.writeDigitRaw (0, B01110011); //P
    matrix.writeDigitRaw (1, B00111000); //l
    matrix.writeDigitRaw (3, B01110111); //A
    matrix.writeDigitRaw (4, B01101110); //y
    matrix.writeDisplay();
    while (1) {}
  }
*/
}


/*
// first sums all the loads then compares to power set and maxloads
checkloadbalance(){
int totalloads;

 pwrLoad=0;   // total combined load of puzzles turned on 
 int  pwrSet=0;    // what knob is set to  start off as balanced

}
*/

// check the power status bits and add totals all the loads togethe
int checkLoads(){
    pwrLoad = 0;
    for (int i = 0; i<=NUMOFPUZ;i++){
          
          if (bitRead(getPuzPwrSt,i))
                    pwrLoad += PuzPwrLoad[i]; 
          
    }
    Serial.print (" total loads=");
    Serial.print (pwrLoad);




    
    return (pwrLoad);
}



/*

void serialEvent(){

 if (Serial.available() > 0) {
                // read the incoming byte:
                int inChar = Serial.read();
                Serial.write (inChar);  // echo back as char
      switch(inChar){
        case'1':   // scroll
        case 'n':
           serCmd = 1;
           break;
        case'2':   //enter
        case 's':
          serCmd = 2;
          break;
        case'3':   //back
        case '<':
        case 'B':
        case 'b':
          serCmd = 3;
          break;

        case 'F':
          //forward();
          serCmd = 0;
         break;
        default:
          Serial.println ("\ninvalid entry");
          Serial.println("1-Scroll\n2-enter\n3-back");
      }
 }
}


// initializes all of this Power puzzles variables-
// some variable are in MODBUS registerss eg. getpwrstatus, set power status, 
// will currently fix the power

*/
/*
initPwrPuz(){
  
   // may want to add loads and set inital set the supply pwrSet to loads


  
  
}*/
