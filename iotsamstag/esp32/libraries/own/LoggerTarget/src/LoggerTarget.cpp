#include<LoggerTarget.h>
#include<string.h>


LoggerTarget::LoggerTarget(const char *thingName, int logLevel) {
    strcpy(_thingName, thingName);
    _logLevel = logLevel;
}

char * LoggerTarget::getName() {
    return _thingName;
}

int LoggerTarget::getLogLevel() {
    return _logLevel;
}

void LoggerTarget::setLogLevel(int type) {
    _logLevel = type;
}