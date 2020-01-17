

#define RXD2 10
#define TXD2 13


void setup() {

  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);                                          //Default baud rate of the Nextion TFT is 9600
  //pinMode(potentiometer_pin,INPUT);                             //Define pin as input
}

void loop() {
  //  i++;
  //  int Value = map(waveOne[i],0,1024,0,255);  //Read the pot value ann map it to 0.255 (max value of waveform=255)
  //  String Tosend = "add ";                                       //We send the string "add "
  //  Tosend += 1;                                                  //send the id of the block you want to add the value to
  //  Tosend += ",";
  //  Tosend += 1;                                                  //Channel of taht id, in this case channel 0 of the waveform
  //  Tosend += ",";
  //  Tosend += Value;                                              //Send the value and 3 full bytes
  Serial2.print("p0.pic=0");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);

  delay (2000);
  Serial2.print("p0.pic=1");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);

  delay (2000);
    Serial2.print("p0.pic=2");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);

  delay (2000);
    Serial2.print("p0.pic=3");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);

  delay (2000);
    Serial2.print("p0.pic=4");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);

  delay (2000);
    Serial2.print("p0.pic=5");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);

  delay (2000);
}
