//#include "Nextion.h"
//
//NexWaveform s0 = NexWaveform(0, 1, "s0");
//int i;
//
//int waveOne[] = {220,
//228,
//235,
//241,
//246,
//249,
//250,
//249,
//246,
//241,
//235,
//228,
//220,
//212,
//205,
//199,
//194,
//191,
//190,
//191,
//194,
//199,
//205,
//212,
//220
//}; //25 elements in the array
//
//
//void setup() {
//  // put your setup code here, to run once:
//  nexInit();
//  Serial.begin(9600);
//}
//
//void loop() {
//  // put your main code here, to run repeatedly:
//  s0.addValue(0, waveOne[i]);
//  s0.addValue(0, waveOne[i] + 2);
//  s0.addValue(0, waveOne[i] + 4);
//  //Serial.println(waveOne[i]);
//    //line(50,50,100,150,31);
//  i++;
//  if (i >= 25){
//    i = 0;
//  }
//
//  delay(50);
//}

int potentiometer_pin = A0;                                     //Define the analog input pin
int i = 0, j = 119, k = 239;

//#include "HardwareSerial.h"

//HardwareSerial MySerial(1);

#define RXD2 10
#define TXD2 13

float waveValue(int degree, int phase = 0, int a = 150) {
  float radian = ((degree + phase) * PI) / 180;
  return a * sin(radian);
}

#define N 25
unsigned long timer = 0;
unsigned long interval = 0;

void setup() {
  Serial.begin(115200);
  Serial2.begin(57600, SERIAL_8N1, RXD2, TXD2);                                          //Default baud rate of the Nextion TFT is 9600
  //pinMode(potentiometer_pin,INPUT);                             //Define pin as input
  timer = millis();
}

void loop() {
  if (millis() - timer > interval) {
    timer = millis();
    i++;
    j++;
    k++;
  }
  
  int Value = map(waveValue(i), -150, 150, 10, 250);  //Read the pot value ann map it to 0.255 (max value of waveform=255)
  String Tosend = "add ";                                       //We send the string "add "
  Tosend += 1;                                                  //send the id of the block you want to add the value to
  Tosend += ",";
  Tosend += 0;                                                  //Channel of taht id, in this case channel 0 of the waveform
  Tosend += ",";
  Tosend += Value;                                              //Send the value and 3 full bytes
  Tosend += "\xFF\xFF\xFF";
  Serial2.print(Tosend);
  
  int Value2 = map(waveValue(j), -150, 150, 10, 250);  //Read the pot value ann map it to 0.255 (max value of waveform=255)
  String Tosend2 = "add ";                                       //We send the string "add "
  Tosend2 += 1;                                                  //send the id of the block you want to add the value to
  Tosend2 += ",";
  Tosend2 += 1;                                                  //Channel of taht id, in this case channel 0 of the waveform
  Tosend2 += ",";
  Tosend2 += Value2;                                              //Send the value and 3 full bytes
  Tosend2 += "\xFF\xFF\xFF";
  Serial2.print(Tosend2);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);

  int Value3 = map(waveValue(k), -150, 150, 10, 250);  //Read the pot value ann map it to 0.255 (max value of waveform=255)
  String Tosend3 = "add ";                                       //We send the string "add "
  Tosend3 += 1;                                                  //send the id of the block you want to add the value to
  Tosend3 += ",";
  Tosend3 += 2;                                                  //Channel of taht id, in this case channel 0 of the waveform
  Tosend3 += ",";
  Tosend3 += Value3;                                              //Send the value and 3 full bytes
  Tosend3 += "\xFF\xFF\xFF";
  Serial2.print(Tosend3);
  
  if (i >= 359) {
    i = 0;
  }

  if (j >= 359) {
    j = 0;
  }

  if (k >= 359) {
    k = 0;
  }
}
