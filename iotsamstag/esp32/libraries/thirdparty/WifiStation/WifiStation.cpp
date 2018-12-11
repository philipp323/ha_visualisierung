#include <WiFi.h>

#include "WifiStation.h"

/*extern "C" {
#include "user_interface.h"
}*/

#define WifiStation_DEBUG

/****************************************** WifiStation **************************************/

/*
	Überprüft, ob eine Anmeldung beim Router möglich war.
*/
bool isWifiConnectionSuccessful(){
	if (WiFi.status() == WL_NO_SHIELD) {  // Auf Vorhandensein eines WLAN-Shields prüfen
		Serial.println("!!! WifiStation::init(), Keine WLAN-Hardware gefunden!");
		return false;
	}
	int attempts = 0;
	Serial.print("Connect to Wifi ");
	while (WiFi.status() != WL_CONNECTED && attempts < 20) {
		delay(500);
		Serial.print(".");
		attempts++;
	}
	Serial.println();
	if (WiFi.status() == WL_CONNECTED)
	{
		Serial.print("WiFi connected, address: ");
		Serial.println(WiFi.localIP());
		return true;
	}
	else {
		return false;
	}
}

/*
	Es wird eine Verbindung zum Wlan aufgebaut.
	Liefert true zurück, wenn die Verbindung erfolgreich aufgebaut wurde.
*/
bool WifiStationClass::init(char* ssid, char* password)
{
	Serial.print("SSID: '");
	Serial.print(ssid);
	Serial.print("', Password: '");
	Serial.print(password);
	Serial.println("'");
	_wifiClient.stop();
	delay(100);				// sonst schlägt Anmeldung schief
	WiFi.begin(ssid, password);
	return isWifiConnectionSuccessful();
}

bool WifiStationClass::init()
{
	Serial.println("Versuch sich mit gespeichertem Kennwort anzumelden");
	int attempts = 0;
	Serial.print("Connect to Wifi ");
	_wifiClient.stop();
	delay(100);
	WiFi.begin();
	return isWifiConnectionSuccessful();
}

/*
	Ist der ESP mit dem WLAN verbunden
*/
bool WifiStationClass::isConnected() {
	return WiFi.status() == WL_CONNECTED;
}

WiFiClient WifiStationClass::getWifiClient(){
	return _wifiClient;
}


WifiStationClass WifiStation;


