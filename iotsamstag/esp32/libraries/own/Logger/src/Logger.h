// Logger.h
#pragma once

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
		logLevel: Level der Logmessage (Info - Fatalerror)
		time: Unix-Timecode
		message: Logtext
	*/

	void init(const char* name);
	void info(const char* tag, const char* message);
	void debug(const char* tag, const char* message);
	void exception(const char* tag, const char* message);
	void error(const char* tag, const char* message);
	void fatalerror(const char* tag, const char* message);
	// void nolog(const char* tag, const char* message);  //! Wozu?

	void addLoggerTarget(LoggerTarget* logger);
	LoggerTarget* getLoggerTarget(const char* name);
	char* getName();

	const char* getLogLevelText(int logLevel);

private:
	void log(int logLevel, const char* tag, const char* message);
	char _name[30];
	std::list<LoggerTarget*> _logger;
	const char* _logLevelTexts[6] = {"Info", "Debug", "Exception", "Error", "FatalError", "NoLog"};
	/*
	void newSave();
	StaticJsonBuffer<200> _jsonBuffer;
	WiFiUDP _udp;
	*/
};
extern LoggerClass Logger;
