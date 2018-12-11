# Library Logger

## Ziele
Der Logger bietet die Möglichkeit, zur Laufzeit der App Meldungen an verschiedene Ziele (Targets) auszugeben. Die Targets werden beim Logger mit einem Loglevel registriert, der angibt, welchen Level zu übermittelnde Logs zumindest haben müssen, damit sie ausgegeben werden.
Er verwaltet alle Komponenten des Knoten (SerialLoggerTarget, MqttLoggerTarget, UdpLoggerTarget, FileLoggerTarget) 
und kümmert sich, dass alle LoggerTargets ihre Benachrichtigung entsprechend dem konfigurierten Loglevel bekommen.

## Hardware

- ESP32

## Software

### Logmessage-Typen

LOG_LEVEL_INFO 0
LOG_LEVEL_DEBUG 1
LOG_LEVEL_EXCEPTION 2
LOG_LEVEL_ERROR 3
LOG_LEVEL_FATALERROR 4
LOG_LEVEL_NOLOG 5			// LoggerTarget bekommt keine Messages

### Testanwendung am ESP32

Erstellt einen Logger und fügt den SerialLogger als Target hinzu. 
In der Loop wird jede Sekunde alle Logglevels aufgerufen. Danach wird der Logglevel des SerialLoggerTarget eins hoch gesetzt. 

#### Testprogramm

````c

int logLevel = 0;
LoggerTarget* sl;

void setup() {
	Serial.begin(115200);
	Logger.init("test");
  sl = new SerialLoggerTarget("test", 0);
  Logger.addLoggerTarget(flg);
}

int readFile = 1;
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
}

````

#### Testanwendung verwenden

- Programm uploaden und starten
- Mit seriellem Monitor verbinden

#### Library

#### Headerdatei Logger.h

````c
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson
#include <WiFiUdp.h>
#include <FS.h>
#include <LoggerTarget.h>
#include <list>

#define LOG_LEVEL_INFO 0
#define LOG_LEVEL_DEBUG 1
#define LOG_LEVEL_EXCEPTION 2
#define LOG_LEVEL_ERROR 3
#define LOG_LEVEL_FATALERROR 4
#define LOG_LEVEL_NOLOG 5

class LoggerClass
{
public:
	/*
		Ein Logeintrag mit folgenden Parametern wird zum Server geschickt:
		topic: Topic, für das der LogEntry ausgelöst wurde
		type: Typ der Logmessage (Info - Fatalerror)
		time: Unix-Timecode
		message: Logtext
	*/

	void init(const char* name);
	void info(const char* tag, const char* message);
	void debug(const char* tag, const char* message);
	void exception(const char* tag, const char* message);
	void error(const char* tag, const char* message);
	void fatalerror(const char* tag, const char* message);
	void nolog(const char* tag, const char* message);
	void addLoggerTarget(LoggerTarget* logger);
	LoggerTarget* getLoggerTarget(const char* name);
	char* getName();

private:
	void log(int type, const char* tag, const char* message);
	char _name[30];
	std::list<LoggerTarget*> _logger;


	/*
	void newSave();
	StaticJsonBuffer<200> _jsonBuffer;
	WiFiUDP _udp;
	*/
};
extern LoggerClass Logger;

````
