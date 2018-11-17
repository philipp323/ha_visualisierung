#include "UdpLoggerTarget.h"
#include <Arduino.h>
#include <Thing.h>

/**
 * Ermittelt aus der aktuellen IP-Adresse die Broadcastadresse, die im letzten Byte
 * FF enthält.
 */
uint32_t getBroadcastIp(){
	tcpip_adapter_ip_info_t ipInfo; 
	tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ipInfo); 
	uint32_t broadcastAddress = ipInfo.ip.addr % (16777216) + 4278190080u; //(255*256*256*256);
	Serial.printf("*UT Broadcast-IP: %x\n", broadcastAddress);
	return broadcastAddress;
}

UdpLoggerTarget::UdpLoggerTarget(const char* name, int logLevel)
    :LoggerTarget(name, logLevel){
	Serial.printf("*UT In UdpLoggerTarget Constructor without ip\n");
	_ip = IPAddress(getBroadcastIp());
	_port = 49155;
	String ipAddressText = _ip.toString();
	char ipAddressBuffer[100];
	ipAddressText.toCharArray(ipAddressBuffer, 100);
	Serial.printf("*UT Udp-TargetAddress: %s, Port: %d\n", ipAddressBuffer, _port);
}

UdpLoggerTarget::UdpLoggerTarget(const char* name, int logLevel, IPAddress ip, int port)
    :LoggerTarget(name, logLevel){
	Serial.printf("*UT In UdpLoggerTarget Constructor with ip\n");
	_ip = IPAddress(getBroadcastIp());
	_port = 49155;
	String ipAddressText = _ip.toString();
	char ipAddressBuffer[100];
	ipAddressText.toCharArray(ipAddressBuffer, 100);
	Serial.printf("*UT Udp-TargetAddress: %s, Port: %d\n", ipAddressBuffer, _port);
}

void UdpLoggerTarget::log(const char* logLevelText, const char* tag, const char* message){
    _udp.beginPacket(_ip, _port);
	char logMessage[256];
	const char* thingName = Thing.getName();
	Serial.printf("*UT Thingname: %s\n", thingName);
	sprintf(logMessage, "*LG: %s;%s;%s;%s\n", Thing.getName(), logLevelText,tag, message);
	_udp.printf(logMessage);
	_udp.endPacket();
	Serial.print(logMessage);
}
