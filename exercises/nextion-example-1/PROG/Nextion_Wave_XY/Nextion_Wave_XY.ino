#include "Nextion.h"

#define arraySize 23

NexWaveform s0 = NexWaveform(0, 1, "s0");

int offset = 125;
int multiplier = 100;

int timer1;
int timerRead;
//Determine the X coordinate 24 values in Array
float posX1[] =
{ 0, 15, 30, 45, 60,
  75, 90, 105, 120, 135,
  150, 165, 180, 195, 210,
  225, 240, 255, 270, 285,
  300, 315, 330, 345
};

float posX2[] =
{ 120, 135,
  150, 165, 180, 195, 210,
  225, 240, 255, 270, 285,
  300, 315, 330, 345, 0, 15, 30, 45, 60,
  75, 90, 105
};
float posX3[] =
{ 240, 255, 270, 285,
  300, 315, 330, 345, 0, 15, 30, 45, 60,
  75, 90, 105, 120, 135,
  150, 165, 180, 195, 210,
  225
};

float Rads = 1000 / 57296 ;

float sinX[arraySize];

int i = 0;
int j = 8 ;

int waveOne[arraySize]; //25 elements in the array


void setup() {
  // put your setup code here, to run once:
  nexInit();
  //nexSerial.begin(112500);//Default baud rate of the Nextion TFT is 9600
  //Serial.begin(9600);

  //populate sin vave table
  //  for (i = 0; i <= 23; i++) {
  //    waveOne[i] = sin(posX1[i]);
  //  }
  //  i = 0;
}
void loop() {
  // put your main code here, to run repeatedly:
  //  s0.addValue(0, waveOne[i] + 2);
  //  s0.addValue(0, waveOne[i] + 4);
  //Serial.println(waveOne[i]);
  //line(50,50,100,150,31);

  if (i >= arraySize) {
    i = 0;
  }

  //Serial.println(waveOne[i]);
  //Serial.print(posX[i]); Serial.print("     "); Serial.println((sin(posX[i] * 1000 / 57296)) * 100 + 145);
  //waveOne[i] = sin(posX[i]);
  //s0.addValue(0, waveOne[i]);
  s0.addValue(0, sin(posX1[i] * 1000 / 57296) * multiplier + offset);
  s0.addValue(0, sin(posX1[i + 2] * 1000 / 57296) * multiplier + offset);
  //s0.addValue(1, sin(posX1[i + j] * 1000 / 57296) * multiplier + offset);
  //s0.addValue(2, sin(posX3[i] * 1000 / 57296) * multiplier + offset);
  //s0.addValue(2, sin(posX3[i + 1] * 1000 / 57296) * multiplier + offset);
  //delay(100);

  if (i == 0 ) {
    j = 8;
  } else if (i >= 15) {
    j = -15;
  }
  i++;
  timer1 = millis() - timerRead;
  if (timer1 >= 5) {
    //i++;
    //shift++;
    timerRead = millis();
    timer1 = 0;
  }
  delay(1);
}

//int potentiometer_pin = A0;                                     //Define the analog input pin
//int i;
//
//int waveOne[] = {220, 272, 320, 361, 393,
//413, 420, 413, 393, 361,
//320, 272, 220, 168, 120,
//79, 47, 27, 20, 27,
//47, 79, 120, 168, 220};
//
//void setup() {
//  //Serial.begin(9600);
//
//  //pinMode(potentiometer_pin,INPUT);                             //Define pin as input
//}
//
//void loop() {
//  i++;
//  int Value = map(waveOne[i],0,1024,0,255);  //Read the pot value ann map it to 0.255 (max value of waveform=255)
//  String Tosend = "add ";                                       //We send the string "add "
//  Tosend += 1;                                                  //send the id of the block you want to add the value to
//  Tosend += ",";
//  Tosend += 0;                                                  //Channel of taht id, in this case channel 0 of the waveform
//  Tosend += ",";
//  Tosend += Value;                                              //Send the value and 3 full bytes
//  Serial.print(Tosend);
//  Serial.write(0xff);
//  Serial.write(0xff);
//  Serial.write(0xff);
//  if (i >= 24){
//    i = 0;
//  }
//  //delay (50);
//}
