/*
    ESP32: microcontroller
        add SuppressInterripts with empty body to the EventManager library
*/


#include <Arduino.h>
#include <EventManager.h>
#define PIN 9

boolean pin13State;
unsigned long lastToggled;

EventManager gEM;

// Our listener will simply toggle the state of pin 13
void listener( int event, int param )
{
    // event and param are not used in this example function
    pin13State = pin13State ? false : true;
    digitalWrite( PIN, pin13State ? HIGH : LOW  );
    lastToggled = millis();
}

void setup()
{
    // Setup
    pinMode( PIN, OUTPUT );
    digitalWrite( PIN, HIGH );
    pin13State = true;
    lastToggled = millis();

    // Add our listener
    gEM.addListener( EventManager::kEventUser0, listener );
}

void loop()
{
    // Handle any events that are in the queue
    gEM.processEvent();
    // Add events into the queue
    addPinEvents();
}

// Add events to toggle pin 13 every second
// NOTE:  doesn't handle millis() turnover
void addPinEvents()
{
    if ( ( millis() - lastToggled ) > 1000 )
    {
        gEM.queueEvent( EventManager::kEventUser0, 0 );
    }
}
