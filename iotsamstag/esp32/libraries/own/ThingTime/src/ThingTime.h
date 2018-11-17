// ThingTime.h
#pragma once

#define NTP_PACKET_SIZE 48  // NTP time is in the first 48 bytes of message

#include <arduino.h>
#include <IPAddress.h>

class ThingTimeClass
{

public:

	/*
		Setzt einen NTP-Time-Subscriber, der im vorgegebenen Intervall die
		lokale Zeit mit der NTP-Zeit synchronisiert.
	*/
	void setNtpTimeSubscriber();
	
	/*
		Liefert die UNIX-Zeit als long zurück
	*/	
	time_t getDateTime();

	/*
		Setzt die interne Zeit
	*/
	void setThingTime(time_t);

	/*
		Liefert Datum und Uhrzeit im mitteleuropäischen Format zurück
	*/
	void getTextDateTime(char* textTime);

	time_t getNtpTime();


private:

	byte _packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets
	void sendNTPpacket(IPAddress &address);

};

extern ThingTimeClass ThingTime;


