# Library UdpLoggerTarget

## Allgemeines

Diese Klasse ist dazu da über UDP die Log Nachrichten zu senden.
Dabei erbt UdpLoggerTarget von LoggerTarget.
Das der UdpLoggerTarget verwendet werden kann muss ein Logger erstellt werden,
danach muss diesen Logger die Instanz des UdpLoggerTarget mitbekommen da der Logger diesen verwaltet.


## Software

### Abhängigkeiten

- LoggerTarget

### Testanwendung am ESP32

Die Klasse UDPLoggerTarget wird mit dem Tool https://www.microsoft.com/en-us/p/udp-sender-reciever/9nblggh52bt0?activetab=pivot:overviewtab getestet.
Dabei wird die IpAdresse des Ziel Computer benötigt wie auch der Port.


#### Library


#### Headerdatei UdpLoggerTarget.h

````c

#include <LoggerTarget.h>
#include <WiFiUdp.h>

class UdpLoggerTarget : public LoggerTarget
{
    public:
        UdpLoggerTarget(const char* name, int type, IPAddress ip, int port);
        virtual void log(int type, const char* tag, const char* message);
        
    private:
        WiFiUDP _udp;
        IPAddress _ip;
        int _port;
};


````

### Beispiel für einen konkreten UdpLoggerTarget

````
#include <Arduino.h>
#include <HttpServer.h>
#include <ThingTime.h>
#include <Logger.h>
#include <UdpLoggerTarget.h>
#include <LoggerTarget.h>

LoggerTarget* udpl;
int logLevel;

void setup() {
	Serial.begin(115200);                 //Initialisierung der seriellen Schnittstelle
	Serial.println();
	Serial.println();
	Serial.println("LoggerTest");
	Serial.println("==========");
	Serial.println();
	Logger.init("test");
	udpl = new UdpLoggerTarget("test", 1, IPAddress(192,168,1,12), 1883);
	Logger.addLoggerTarget(udpl);
	ThingTime.setNtpTimeSubscriber();
    HttpServer.init();
}

void loop() {
	delay(1000);
    HttpServer.handleClient();
    Logger.info("Main-Loop", "Logtest Level: 0");
    Logger.debug("Main-Loop", "Logtest Level: 1");
    Logger.exception("Main-Loop", "Logtest Level: 2");
    Logger.error("Main-Loop", "Logtest Level: 3");
    Logger.fatalerror("Main-Loop", "Logtest Level: 4");
    Logger.nolog("Main-Loop", "Logtest Level: 5");
    if(logLevel == LOG_LEVEL_NOLOG){
        logLevel = LOG_LEVEL_INFO;
        udpl->setType(logLevel);
        Serial.println("");
    } else {
        logLevel++;
        udpl->setType(logLevel);
    }
}


````
