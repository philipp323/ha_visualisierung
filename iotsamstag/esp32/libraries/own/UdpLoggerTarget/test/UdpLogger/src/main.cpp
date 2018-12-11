#include <Arduino.h>
#include <HttpServer.h>
#include <ThingTime.h>
#include <Logger.h>
#include <UdpLoggerTarget.h>
#include <LoggerTarget.h>

LoggerTarget* udpLogger;
int logLevel;

void setup() {
  Serial.begin(115200);                 //Initialisierung der seriellen Schnittstelle
  Serial.println("Logger Test");
  Serial.println("===========");
  Serial.println("Sends a logmessage for each loglevel in a loop");
  Serial.println();
  udpLogger = new UdpLoggerTarget("test", 0, IPAddress(192,168,137,1), 1883);
  Logger.addLoggerTarget(udpLogger);
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
      udpLogger->setLogLevel(logLevel);
      Serial.println("-----------------------------------------------------------------------");
  } else {
      logLevel++;
      udpLogger->setLogLevel(logLevel);
      Serial.println();
  }
	delay(1000);
}