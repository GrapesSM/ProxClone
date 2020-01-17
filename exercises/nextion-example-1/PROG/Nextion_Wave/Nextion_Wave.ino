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
int i;

//#include "HardwareSerial.h"

//HardwareSerial MySerial(1);

#define RXD2 9
#define TXD2 10

int waveOne[] = {220, 272, 320, 361, 393,
413, 420, 413, 393, 361,
320, 272, 220, 168, 120,
79, 47, 27, 20, 27,
47, 79, 120, 168, 220};

void setup() {
  
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);                                          //Default baud rate of the Nextion TFT is 9600
  //pinMode(potentiometer_pin,INPUT);                             //Define pin as input
}

void loop() {
  i++;
  int Value = map(waveOne[i],0,1024,0,255);  //Read the pot value ann map it to 0.255 (max value of waveform=255)
  String Tosend = "add ";                                       //We send the string "add "
  Tosend += 1;                                                  //send the id of the block you want to add the value to
  Tosend += ",";  
  Tosend += 1;                                                  //Channel of taht id, in this case channel 0 of the waveform
  Tosend += ",";
  Tosend += Value;                                              //Send the value and 3 full bytes
  Serial2.print(Tosend);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  if (i >= 24){
    i = 0;
  }
  //delay (50);
}
