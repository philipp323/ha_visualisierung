
#include "Logger.h"

/*
	Ein Logeintrag mit folgenden Parametern wird zum Server geschickt:
	topic: Topic, für das der LogEntry ausgelöst wurde
	type: Typ der Logmessage (Info - Fatalerror)
	time: Unix-Timecode
	message: Logtext
*/

void LoggerClass::init(const char* name) {
	strcpy(_name, name);
	Serial.print(F("TH* Thing init with name: "));
	Serial.println(_name);
}

void LoggerClass::info(const char* tag, const char* message) {
	log(LOG_LEVEL_INFO, tag, message);
}

void LoggerClass::debug(const char* tag, const char* message) {
	log(LOG_LEVEL_DEBUG, tag, message);
}

void LoggerClass::exception(const char* tag, const char* message) {
	log(LOG_LEVEL_EXCEPTION, tag, message);
}

void LoggerClass::error(const char* tag, const char* message) {
	log(LOG_LEVEL_ERROR, tag, message);
}

void LoggerClass::fatalerror(const char* tag, const char* message) {
	log(LOG_LEVEL_FATALERROR, tag, message);
}

void LoggerClass::log(int logLevel, const char* tag, const char* message) {
	for (std::list<LoggerTarget *>::iterator it = _logger.begin(); it != _logger.end(); ++it)
	{
		// Serial.print(F("*NO: refresh, Sensor: "));
		// Serial.print(it->first);
		// Serial.print(F(", last Value: "));
		// Serial.println(it->second->getLastMeasurement());
		LoggerTarget *loggerTarget = *it;
		if(logLevel >= loggerTarget -> getLogLevel()) {
			loggerTarget->log(_logLevelTexts[logLevel], tag, message);
		}
	}
}

void LoggerClass::addLoggerTarget(LoggerTarget* logger) {
	_logger.push_back(logger);
}

const char* LoggerClass::getLogLevelText(int logLevel){
	return _logLevelTexts[logLevel];
}


LoggerTarget* LoggerClass::getLoggerTarget(const char* name) {
	for (std::list<LoggerTarget *>::iterator it = _logger.begin(); it != _logger.end(); ++it)
	{
		// Serial.print(F("*NO: refresh, Sensor: "));
		// Serial.print(it->first);
		// Serial.print(F(", last Value: "));
		// Serial.println(it->second->getLastMeasurement());
		LoggerTarget *logger = *it;
		if(strcmp(logger->getName(), name) == 0) {
			return logger;
		}
	}
	return nullptr;
}

char * LoggerClass::getName() {
	return _name;
}
LoggerClass Logger;

/*
#include <MqttClient.h>
#include <ThingTime.h>
#include <WiFi.h>



void LoggerClass::logFile(int type, const char* message){
	if(SPIFFS.begin(true)) {
		if(SPIFFS.exists("/log.txt")) {
			File file = SPIFFS.open("/log.txt", FILE_APPEND);
			if(file){
  	  			Serial.println(F("File is opend"));
				char logMessage[128];
				sprintf(logMessage, "*LG: => [ %i ] => %s \n", type, message);
				if(file.print(logMessage)){
    			Serial.println("File content was written");
					Serial.println(file.size());
					if(file.size() > 5000){
						newSave();
					}
  			} else {
    			Serial.println("File append failed");
  			}
				file.close();
			} else {
				Serial.println(F("There was an error opening the file for writing"));
			}
		} else {
			File file = SPIFFS.open("/log.txt", FILE_APPEND);
			file.close();
			logFile(type, message);
		} 
	} else {
		Serial.println(F("An Error has occurred while mounting SPIFFS"));
	}
}

void LoggerClass::logMqtt(const char* topic, int type, const char* message)
{
	char fullTopic[100] = "log/";
	strcat(fullTopic, topic);
	_jsonBuffer.clear();
	JsonObject& logEntry = _jsonBuffer.createObject();
	logEntry["type"] = type;
	logEntry["time"] = ThingTime.getDateTime();
	logEntry["message"] = message;
	char jsonText[128];
	logEntry.printTo(jsonText);
	Serial.print(F("*LG: Topic: "));
	Serial.print(fullTopic);
	Serial.print(F(", Payload: "));
	Serial.println(jsonText);
	MqttClient.publish(fullTopic, jsonText);
}

void LoggerClass::logReadFile(){
	if(SPIFFS.exists("/logLate.txt")){
		File f = SPIFFS.open("/logLate.txt", FILE_READ);
		while(f.available()){
      Serial.print((char)f.read());
    }
		f.close();
	}
	if(SPIFFS.exists("/log.txt")){
		File f = SPIFFS.open("/log.txt", FILE_READ);
		while(f.available()){
      Serial.print((char)f.read());
    }
		f.close();
	}
}

void LoggerClass::newSave(){
	if(SPIFFS.exists("/logLate.txt")){
		SPIFFS.remove("/logLate.txt");
	}
	File file = SPIFFS.open("/logLate.txt", FILE_APPEND);
	File fileToRead = SPIFFS.open("/log.txt", FILE_READ);
  if(fileToRead){
   	Serial.println(F("File Content:"));
   	while(fileToRead.available()){
			file.print((char)fileToRead.read());
		}
		file.close();
		fileToRead.close();
		SPIFFS.remove("/log.txt");
  } else {
		Serial.println(F("Failed to open file for reading"));
	}
	Serial.println(F("newSave Worked!!!"));
}
*/
