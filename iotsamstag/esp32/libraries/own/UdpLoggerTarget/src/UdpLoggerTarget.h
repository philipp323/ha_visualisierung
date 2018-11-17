#pragma once

#include <LoggerTarget.h>
#include <WiFiUdp.h>

class UdpLoggerTarget : public LoggerTarget
{
    public:
        UdpLoggerTarget(const char* name, int logLevel, IPAddress ip, int port);

        /**
         * Ohne Angabe von Port und/oder Target-IP wird als Standard
         * die Broadcast-IP-Addresse mit 255 im letzten Byte (z.B. 10.0.0.255)
         * und als Portnummer 49155 angenommen.
         */
        UdpLoggerTarget(const char* name, int logLevel);
        
        virtual void log(const char* logLevelText, const char* tag, const char* message);
        
    private:
        WiFiUDP _udp;
        IPAddress _ip;
        int _port;
};