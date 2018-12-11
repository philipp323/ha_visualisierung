#pragma once

#include <Arduino.h>
#include <Ticker.h>


class Dht22
{
  public:
    Dht22( uint8_t pin);
    float getTemperature();
    float getHumidity();
    uint8_t getLastErrorCode();
    uint8_t getLastErrorPosition();
    long getSecondsFromLastError();
    long getSecondsFromLastCorrectMeasurement();

    void processEvent();
    void notifyChangingEdge();


  private:
    #define TIMESTAMPS 100  // Zuerst eine Startsequenz und dann 80 Bits
    unsigned long _changingEdgeMicroSeconds[TIMESTAMPS];
    volatile uint8_t _changingEdgeIndex=0;

    unsigned long _lastCorrectMeasurementMilliSeconds=0;
    unsigned long _lastErrorMilliSeconds=0;
    uint8_t _lastErrorCode=0;
    uint8_t _lastErrorPos=0;
    uint8_t _state;
    uint8_t _pin;
    Ticker _ticker;
    float _temperature;
    float _humidity;

};

