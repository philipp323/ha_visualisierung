# Library Logger

## Ziele

Abstrakte Basisklasse zur Verwaltung der allgemein Verhaltens von LoggerTarget.
Jeder LoggerTarget ist innerhalb des Logger über den Namen eindeutig identifizierbar. 

## Hardware

- ESP32

## Software

### Logmessage-Typen

LOG_LEVEL_INFO 0
LOG_LEVEL_DEBUG 1
LOG_LEVEL_EXCEPTION 2
LOG_LEVEL_ERROR 3
LOG_LEVEL_FATALERROR 4
LOG_LEVEL_NOLOG 5

### Testanwendung am ESP32

Erstellt einen Logger und fügt den SerialLoggerTarget hinzu. In der Loob wird jede Sekunde alle Logglevels aufgerufen. Danach wird der Logglevel des SerialLoggerTarget eins hoch gesetzt. 

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

#### Headerdatei LoggerTarget.h

````c
class LoggerTarget {
    public:
        LoggerTarget(const char *thingName ,int type);
        char* getName();
        int getType();
        void setType(int type);
        virtual void log(int type, const char* tag, const char* message)=0;

    private:
        char _thingName[30];
        int _type;
};

````
