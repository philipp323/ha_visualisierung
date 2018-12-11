#pragma once

#include "arduino.h"
#include <WiFi.h>

class WifiStationClass
{
public:

	/*
		Es wird eine Verbindung zum Wlan aufgebaut.
		Liefert true zurück, wenn die Verbindung erfolgreich aufgebaut wurde.
	*/
	bool init(char* ssid, char* password);

	bool init();

	/**
	 * liefert den eigentlichen WifiClient zurück
	 * 
	 */
	WiFiClient getWifiClient();
	

	/*
		Ist der ESP mit dem WLAN verbunden
	*/
	bool isConnected();
private:
	WiFiClient _wifiClient;

};

extern WifiStationClass WifiStation;


