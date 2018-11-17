# Library SerialLoggerTarget

## Allgemeines

Diese Klasse ist dazu da über Serial die Log Nachrichten zu schreiben.
Dabei erbt SerialLoggerTarget von LoggerTarget.
Das der SerialLoggerTarget verwendet werden kann muss ein Logger erstellt werden,
danach muss diesen Logger die Instanz des SerialLoggerTarget mitbekommen da der Logger diesen verwaltet.

## Software

### Abhängigkeiten

- LoggerTarget

### Testanwendung am ESP32

Die Klasse SerialLoggerTarget wird über die Seriale Schnittstelle getestet. 


#### Library


#### Headerdatei SerialLoggerTarget.h

````c

#include <LoggerTarget.h>

class SerialLoggerTarget : public LoggerTarget
{
    public:
        SerialLoggerTarget(const char* name, int type);
        virtual void log(int type, const char* tag, const char* message);
        
    private:
};


````

### Beispiel für einen konkreten SerialLoggerTarget

````
#include <Arduino.h>
#include <Logger.h>
#include <SerialLoggerTarget.h>
#include <LoggerTarget.h>

int logLevel = 0;
LoggerTarget* sl;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    Logger.init("test");
    sl = new SerialLoggerTarget("test", 0);
    Logger.addLoggerTarget(sl);
}

void loop() {
    delay(1000);
    Logger.info("Main-Loop", "Logtest Level: 0");
    Logger.debug("Main-Loop", "Logtest Level: 1");
    Logger.exception("Main-Loop", "Logtest Level: 2");
    Logger.error("Main-Loop", "Logtest Level: 3");
    Logger.fatalerror("Main-Loop", "Logtest Level: 4");
    Logger.nolog("Main-Loop", "Logtest Level: 5");
    if(logLevel == LOG_LEVEL_NOLOG){
        logLevel = LOG_LEVEL_INFO;
        sl->setType(logLevel);
        Serial.println("");
    } else {
        logLevel++;
        sl->setType(logLevel);
    }
    // put your main code here, to run repeatedly:
}

````
