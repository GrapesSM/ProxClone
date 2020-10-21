#include "SPIFFS.h"
#define numCol 14
#define numRow 6
#define wavCharMAX 30
#define buffersize 1024
#define dacPin 25
#define speakerEnable 2

char p[numCol][numRow][wavCharMAX];  // initialize array
char pOld[numCol][numRow][wavCharMAX];  // initialize array
char buffer[buffersize];
unsigned int scaled;
bool loadFile = true;
int i;
int Filesize;
int fileposition;
int readbyte;
int numBytes;
int byteflag = 0;
int old;
int x;

int oldindex;
int oldindex2;

byte loopflag = 1;
byte CheckFile = 1;
unsigned int timer1; unsigned int timer1old;

File f;

void playSound(const int index, const int index2, byte SoundFlag, uint8_t volume) {

  if (SoundFlag == true) {


      //Serial.println (loadFile);
      if (pOld[oldindex][oldindex2][wavCharMAX] == p[index][index2][wavCharMAX]) {
        loadFile = false;
        //Serial.println (loadFile);
      } else {
        loadFile = true;
      }

       if (CheckFile == 1) {
        loadFile = true;
        CheckFile = 0;
      }

    //Serial.println (loadFile);
    if (loadFile == true ) {
      //Serial.print("old Name: "); Serial.print(p[index][index2]); Serial.print("  Selected: "); Serial.println(p[index][index2]);
      // 0 - Common Files
      strcpy(p[0][0], "/StationPowerUp.wav");
      strcpy(p[0][1], "/StationPowerDown.wav");

      // 1 - Power Control
      strcpy(p[1][0], "/-----.wav");
      strcpy(p[1][1], "/-----.wav");
      strcpy(p[1][2], "/-----.wav");

      // 4 - Datamatic
      strcpy(p[4][0], "/DialClear.wav");
      strcpy(p[4][1], "/DialInput.wav");
      strcpy(p[4][2], "/NumberButtons.wav");
      strcpy(p[4][3], "/ProcessingScreen.wav");
      strcpy(p[4][4], "/TransmitButton.wav");

      // 5 - Safeomatic
      strcpy(p[5][0], "/speech3.wav");
      strcpy(p[5][1], "/-----.wav");
      strcpy(p[5][2], "/-----.wav");

      // 8 - Life Support
      strcpy(p[8][0], "/ExternalVent.wav");
      strcpy(p[8][1], "/LifePump.wav");

      // 9 - Docked Ship (energy and ship prep aux.)
      strcpy(p[9][0], "/CountdownBeep.wav");
      strcpy(p[9][1], "/KeySwitch.wav");
      strcpy(p[9][2], "/SycroBeep.wav");
      strcpy(p[9][3], "/Switch.wav");

      // 10 - Prep Status
      strcpy(p[10][0], "/-----.wav");
      strcpy(p[10][1], "/-----.wav");
      strcpy(p[10][2], "/-----.wav");

      // 11 - Laser Grid
      strcpy(p[11][0], "/KeyInsert.wav");

      // 12 - KeyPad
      strcpy(p[12][0], "/NumberButtons.wav");
      strcpy(p[12][1], "/Error.wav");
      strcpy(p[12][2], "/Correct.wav");

      // 13 - Laserbar
      strcpy(p[13][0], "/-----.wav");
      strcpy(p[13][1], "/-----.wav");
      strcpy(p[13][2], "/-----.wav");

      pOld[oldindex][oldindex2][wavCharMAX] = p[index][index2][wavCharMAX];

      SPIFFS.begin();
      pinMode(2, OUTPUT);



      f = SPIFFS.open(p[index][index2], "r");
      //Serial.print("File Name: "); Serial.print(p[index][index2]); Serial.print("File Size: "); Serial.println(f.size());
      Filesize = f.size();
      //Serial.print (micros());
    }



    //if ((SoundFlag == true) && (f.position() < f.size())) {

    if (f.position() < (f.size() - 1)) {
      int numBytes = _min(1024, f.size() - f.position() - 1);

      if (byteflag == 0) {
        f.readBytes(buffer, numBytes);
        byteflag = 1;
      }

      if (i < numBytes) {
        int old = micros();
        scaled = map((unsigned int)buffer[i], 0, 255, 0, volume);
        i++;
        dacWrite(25, scaled);
        timer1old = micros();
        while (micros() - old < 45); //125usec = 1sec/8000 and assume 5us for overhead like wifi

      } else {
        i = 0;
        byteflag = 0;
      }

      //Serial.print("Position "); Serial.println(f.position());
    } else {
      Serial.println("File Closed");
      pOld[oldindex][oldindex2][wavCharMAX] = p[index][index2][wavCharMAX];
      f.close();
      SoundFlag = true;
      loopflag = 0;
      i = 0;
      byteflag = 0;
      digitalWrite(25,LOW);
      return;
    }
  }
}
