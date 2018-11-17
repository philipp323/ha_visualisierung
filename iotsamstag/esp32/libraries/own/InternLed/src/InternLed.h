#pragma once

#include <arduino.h>
#include <Ticker.h>

/* Die interne LED am ESP8266 kann für verschiedene Zwecke angesprochen werden
    kurzes Blinken mit oder ohne vorgegebener Anzahl
    langes Blinken mit oder ohne vorgegebener Anzahl
*/

class InternLedClass {
public:
    InternLedClass();
    void init(uint8_t pin);
    void blinkFast();
    void blinkSlow();
    void blinkFast(uint8_t times);
    void blinkSlow(uint8_t times);
    void blinkOff();
    // Methoden für Zugriff auf Fields mittels
    // Callbackroutine (normale C-Funktion)
    uint32_t getPeriodMs();
    uint8_t getTimesToBlink();
    void setTimesToBlink(uint8_t times);
    uint8_t getPin();
    Ticker* getTicker();
private: 
    Ticker* _ticker;
    uint8_t _timesToBlink;
    uint32_t _periodMs;
    uint8_t _pin;
};
extern InternLedClass InternLed;  
