#include <Arduino.h>
#include <Logger.h>
#include <SerialLoggerTarget.h>

int logLevel = 0;
LoggerTarget* serialLogger;

void setup() {
  Serial.begin(115200);                 //Initialisierung der seriellen Schnittstelle
  Serial.println("Logger Test");
  Serial.println("===========");
  Serial.println("Sends a logmessage for each loglevel in a loop");
  Serial.println();
  serialLogger = new SerialLoggerTarget("test", 0);
  Logger.addLoggerTarget(serialLogger);
}

void loop() {
  Serial.printf("Actual minimum Loglevel set: %s\n", Logger.getLogLevelText(logLevel));
  Logger.info("Main-Loop", "Info");
  Logger.debug("Main-Loop", "Debug");
  Logger.exception("Main-Loop", "Exception");
  Logger.error("Main-Loop", "Error");
  Logger.fatalerror("Main-Loop", "FatalError");
  if(logLevel == LOG_LEVEL_NOLOG){
      logLevel = LOG_LEVEL_INFO;
      serialLogger->setLogLevel(logLevel);
      Serial.println("-----------------------------------------------------------------------");
  } else {
      logLevel++;
      serialLogger->setLogLevel(logLevel);
      Serial.println();
  }
	delay(1000);
}