#include <WiFi.h>
#include <WiFiUdp.h>

#include "Time.h"
#include "ThingTime.h"

/*-------- NTP code ----------*/

// NTP Servers:
/****
 * http://www.pool.ntp.org/zone/at
 * In most cases it's best to use pool.ntp.org to find an NTP server 
 * (or 0.pool.ntp.org, 1.pool.ntp.org, etc if you need multiple server names). 
 * The system will try finding the closest available servers for you.
 ****/
static const char ntpServerName[] = "0.at.pool.ntp.org";

const int timeZone = 1;     // Central European Time
							//const int timeZone = -5;  // Eastern Standard Time (USA)
							//const int timeZone = -4;  // Eastern Daylight Time (USA)
							//const int timeZone = -8;  // Pacific Standard Time (USA)
							//const int timeZone = -7;  // Pacific Daylight Time (USA)


WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets

/*
	Send an NTP request to the time server at the given address
*/
void ThingTimeClass::sendNTPpacket(IPAddress &address)
{
	// set all bytes in the buffer to 0
	memset(_packetBuffer, 0, NTP_PACKET_SIZE);
	// Initialize values needed to form NTP request
	// (see URL above for details on the packets)
	_packetBuffer[0] = 0b11100011;   // LI, Version, Mode
	_packetBuffer[1] = 0;     // Stratum, or type of clock
	_packetBuffer[2] = 6;     // Polling Interval
	_packetBuffer[3] = 0xEC;  // Peer Clock Precision
							 // 8 bytes of zero for Root Delay & Root Dispersion
	_packetBuffer[12] = 49;
	_packetBuffer[13] = 0x4E;
	_packetBuffer[14] = 49;
	_packetBuffer[15] = 52;
	// all NTP fields have been given values, now
	// you can send a packet requesting a timestamp:
	Udp.beginPacket(address, 123); //NTP requests are to port 123
	Udp.write(_packetBuffer, NTP_PACKET_SIZE);
	Udp.endPacket();
}

/*
	Read the time from the ntp-server
*/
time_t ThingTimeClass::getNtpTime()
{
	IPAddress ntpServerIP; // NTP server's ip address

	while (Udp.parsePacket() > 0); // discard any previously received packets
	Serial.println(F("*NT: Transmit NTP Request"));
	// get a random server from the pool
	WiFi.hostByName(ntpServerName, ntpServerIP);
	Serial.print(ntpServerName);
	Serial.print(": ");
	Serial.println(ntpServerIP);
	sendNTPpacket(ntpServerIP);
	uint32_t beginWait = millis();
	while (millis() - beginWait < 1500) {
		int size = Udp.parsePacket();
		if (size >= NTP_PACKET_SIZE) {
			Serial.println(F("*NT: Receive NTP Response"));
			Udp.read(_packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
			Serial.println(F("*NT: UDP read"));
			unsigned long secsSince1900;
			// convert four bytes starting at location 40 to a long integer
			secsSince1900 = (unsigned long)_packetBuffer[40] << 24;
			secsSince1900 |= (unsigned long)_packetBuffer[41] << 16;
			secsSince1900 |= (unsigned long)_packetBuffer[42] << 8;
			secsSince1900 |= (unsigned long)_packetBuffer[43];
			Serial.println(F("*NT: After calculation"));
			return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
			//return secsSince1900 - 2208988800UL;  // UTC
		}
	}
	Serial.println(F("!NT: No NTP Response :-("));
	return 0; // return 0 if unable to get the time
}

/**
 * C-Callbackroutine, die zyklisch entsprechend dem Synchronisierungsintervall
 * aufgerufen wird und die interne Zeit neu setzt
 */
time_t getNtpTimeHandler(){
	return ThingTime.getNtpTime();
}

/*
	Setzt einen NTP-Time-Subscriber, der im vorgegebenen Intervall die 
	lokale Zeit mit der NTP-Zeit synchronisiert.
*/
void ThingTimeClass::setNtpTimeSubscriber() {
	Serial.println(F("*NT: Starting UDP"));
	Udp.begin(localPort);
	Serial.print(F("*NT: Local port: "));
	Serial.println(Udp.remotePort());
	Serial.println(F("waiting for sync"));
	setSyncProvider(getNtpTimeHandler);
	setSyncInterval(3600);  // Synchronisation jede Stunde
	long time = getNtpTime();
	Serial.print(F("*NT: Time: "));
	Serial.println(time);
	setThingTime(getNtpTime());
	Serial.println(F("*NT: getNtpTime()"));
	Serial.print(F("*NT: Time from ntp: "));
	char dateTimeText[30];
	ThingTime.getTextDateTime(dateTimeText);
	Serial.print(F("Date and Time: "));
	Serial.println(dateTimeText);
}

/*
	Liefert die UNIX-Zeit als long zurück
*/
time_t ThingTimeClass::getDateTime() {
	return now();
}

/*
	Setzt die interne Zeit
*/
void ThingTimeClass::setThingTime(time_t time) {
	setTime(time);
}


/*
	Liefert Datum und Uhrzeit im mitteleuropäischen Format zurück
*/
void ThingTimeClass::getTextDateTime(char* textTime)
{
	TimeElements timeElements;
	breakTime(now(), timeElements);
	sprintf(textTime, "%02d.%02d.%04d %02d:%02d:%02d", timeElements.Day, timeElements.Month, 
		timeElements.Year + 1970, timeElements.Hour, timeElements.Minute, timeElements.Second );
}

ThingTimeClass ThingTime;
