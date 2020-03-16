#ifndef helpers_h
#define helpers_h

#include <Arduino.h>
#include "Queue.h"

bool flagOnce = false;
bool flagToggle = false;
bool flagOnoff = false;
bool flagOffon = false;
bool flagOnoffon = false;

// Functions ---------------

bool & once(bool & flag);
bool & toggle(bool & flag);
bool & onoff(bool & flag);
bool & offon(bool & flag);

// -------------------------

bool & once(bool & flag) {
  if (flag)
    return flag;
  flag = true;
  
  return flag;
}

bool & toggle(bool & flag) {
  flag = !flag;
  
  return flag;
}

bool & onoff(bool & flag) {
  flag = flag ? flag: false;
  
  return flag;
}

bool & offon(bool & flag) {
  flag = flag ? false : flag;
  
  return flag;
}

// bool & once(bool & flag, void (*f)()) {
//   if (flag)
//     return flag;
//   (*f)();
//   flag = true;
//   return flag;
// }

// bool & toggle(bool & flag, void (*f)()) {
//   flag = !flag;
//   (*f)();
//   return flag;
// }

// bool & onoff(bool & flag, void (*f)()) {
//   flag = flag ? flag: false;
//   (*f)();
//   return flag;
// }

// bool & offon(bool & flag, void (*f)()) {
//   flag = flag ? false : flag;
//   (*f)();
//   return flag;
// } 


// class Timer {
//   public:
//     void setTimer(void (*fun_ptr)(int), unsigned long milliseconds);
//   private:
//     DataQueue<unsigned int> _queue;
// }

// Timer::Timer() 
// {

// }

// void Timer::setTimer(void (*fun_ptr)(), unsigned long milliseconds, int option)
// {
  
// }

// void Timer::runTimer(void (*fun_ptr)(), unsigned long milliseconds, int option)
// {
//   if ((millis() - current) > milliseconds) {

//   } else {

//   }
// }
#endif