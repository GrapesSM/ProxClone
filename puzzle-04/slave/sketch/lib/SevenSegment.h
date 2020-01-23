/*
  SevenSegment.h - Library for ________.
*/
#ifndef SevenSegment_h
#define SevenSegment_h

#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

class SevenSegment : public Adafruit_7segment {
  public:
    void printNumber(long n);
    void printNumber(long n, uint8_t base);
    void printFloat(double n, uint8_t fracDigits, uint8_t base);
    void printString(String s);
    void setDigits(uint8_t value);
  private:
    uint8_t _numberOfDigits = 5;
    int availabPos = 4;
};

void SevenSegment::printString(String s) 
{
  int num = 0;
  int stringLength = s.length();
  int firstZerosCount = 0;
  for(int i = 0; i <stringLength; i++){
    if(s[i] == '0'){
      firstZerosCount++;
    }else{
      break;
    }
  }
  num = s.toInt();
  printNumber(num);
  for(int i = 0; i < firstZerosCount; i++){
    writeDigitNum(_numberOfDigits - stringLength + i, 0, false);
  }
}

void SevenSegment::setDigits(uint8_t numberOfDigits) 
{
  _numberOfDigits = numberOfDigits;
}

void SevenSegment::printNumber(long n)
{
  printNumber(n, 10);
}

void SevenSegment::printNumber(long n, uint8_t base)
{
  printFloat(n, 0, base);
}

void SevenSegment::printFloat(double n, uint8_t fracDigits, uint8_t base) 
{ 
  uint8_t numericDigits = _numberOfDigits;   // available digits on display
  boolean isNegative = false;  // true if the number is negative
  
  // is the number negative?
  if(n < 0) {
    isNegative = true;  // need to draw sign later
    --numericDigits;    // the sign will take up one digit
    n *= -1;            // pretend the number is positive
  }
  
  // calculate the factor required to shift all fractional digits
  // into the integer part of the number
  double toIntFactor = 1.0;
  for(int i = 0; i < fracDigits; ++i) toIntFactor *= base;
  
  // create integer containing digits to display by applying
  // shifting factor and rounding adjustment
  uint32_t displayNumber = n * toIntFactor + 0.5;
  
  // calculate upper bound on displayNumber given
  // available digits on display
  uint32_t tooBig = 1;
  for(int i = 0; i < numericDigits; ++i) tooBig *= base;
  
  // if displayNumber is too large, try fewer fractional digits
  while(displayNumber >= tooBig) {
    --fracDigits;
    toIntFactor /= base;
    displayNumber = n * toIntFactor + 0.5;
  }
  
  // did toIntFactor shift the decimal off the display?
  if (toIntFactor < 1) {
    printError();
  } else {
    // otherwise, display the number
    int8_t displayPos = 4;
    
    if (displayNumber)  //if displayNumber is not 0
    {
      for(uint8_t i = 0; displayNumber || i <= fracDigits; ++i) {
        boolean displayDecimal = (fracDigits != 0 && i == fracDigits);
        writeDigitNum(displayPos--, displayNumber % base, displayDecimal);
        // if(displayPos == 2) writeDigitRaw(displayPos--, 0x00);
        displayNumber /= base;
      }
    }
    else {
      writeDigitNum(displayPos--, 0, false);
    }
  
    // display negative sign if negative
    if(isNegative) writeDigitRaw(displayPos--, 0x40);
  
    // clear remaining display positions
    while(displayPos >= 0) writeDigitRaw(displayPos--, 0x00);
  }
}

#endif