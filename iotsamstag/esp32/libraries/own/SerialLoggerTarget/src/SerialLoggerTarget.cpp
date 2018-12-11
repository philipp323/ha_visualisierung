#include "SerialLoggerTarget.h"
#include <Arduino.h>

SerialLoggerTarget::SerialLoggerTarget(const char* name, int logLevel)
    :LoggerTarget(name, logLevel){
}

void SerialLoggerTarget::log(const char* logLevelText, const char* tag, const char* message){
    Serial.printf("*LG: %s => [ %s ] => %s\n", logLevelText, tag, message);
}
