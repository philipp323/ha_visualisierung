#pragma once

#include <LoggerTarget.h>

class SerialLoggerTarget : public LoggerTarget
{
    public:
        SerialLoggerTarget(const char* name, int logLevel);
        virtual void log(const char* logLevelText, const char* tag, const char* message);
        
    private:
};