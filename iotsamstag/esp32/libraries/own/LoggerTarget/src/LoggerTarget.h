#pragma once

class LoggerTarget {
    public:
        LoggerTarget(const char *thingName ,int logLevel);
        char* getName();
        int getLogLevel();
        void setLogLevel(int logLevel);
        virtual void log(const char* logLevelText, const char* tag, const char* message)=0;

    private:
        char _thingName[30];
        int _logLevel;
};