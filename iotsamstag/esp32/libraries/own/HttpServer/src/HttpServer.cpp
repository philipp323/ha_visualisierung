#include "HttpServer.h"
#include <InternLed.h>
// #include <WiFiManager.h>
#include <ThingConfig.h>
#include <SPIFFS.h>
#include <WiFi.h>
// #include <Actor.h>
// #include <Thing.h>
//#include <ArduinoOTA.h>

#define WEMOS

#ifdef WEMOS
	#define LED_BUILTIN 2  // WEMOS MINI32
#else 
	#ifdef TTGO
		#define LED_BUILTIN 16  // TTGO
	#endif
#endif



#define MQTT_SERVERADDRESS_LENGTH 40
#define MQTT_PORT_LENGTH 6
#define MQTT_USERNAME_LENGTH 20
#define MQTT_PASSWORD_LENGTH 20
#define MQTT_THING_NAME_LENGTH 80

char _mqttServerAddress[MQTT_SERVERADDRESS_LENGTH]="10.0.0.125";
char _mqttPort[MQTT_PORT_LENGTH] = "1883";
char _mqttUserName[MQTT_USERNAME_LENGTH];
char _mqttPassword[MQTT_PASSWORD_LENGTH];
char _mqttThingName[MQTT_THING_NAME_LENGTH]="iot/test";


HttpServerClass::HttpServerClass(int port) : WebServer(port)
{
}

/*************************************** Callback-Funktionen für den Webserver ******************************/
 //! Gehört in die Sensor und Actor-Library

//! Allgemeiner Config-Request. Übermittelt werden Key/Value-Paare für die gewünschten Konfigurationseinträge
void handleSetConfigRequest()
{
	char configKey[50];
	char configValue[50];
	Serial.println(F("*HS: handleSetConfigRequest()"));
	for ( uint8_t i = 0; i < HttpServer.args(); i++ ) {
		HttpServer.argName(i).toCharArray(configKey,50);
		HttpServer.arg(i).toCharArray(configValue, 50);
		Serial.printf("*HS: SetConfig, Key: %s, Value: %s\n", configKey, configValue);
		ThingConfig.setValue(configKey, configValue );
	}

	char response[500];
	ThingConfig.getConfigJson(response, 500);
	Serial.print(F("*HS: Config: "));
	Serial.println(response);
	Serial.println(response);
	HttpServer.send(200, "text/html", response); //Returns the HTTP response
}

/*
	Umwandlung einer Hexziffer (0-15) in das Hexzeichen (0-F)
*/
char getHexChar(int digit)
{
	if (digit < 10)
	{
		return '0' + digit;
	}
	return 'A' + digit - 10;
}

/*
  MAC-Adresse in entsprechenden String umwandeln
  und in übergebenes char[] speichern
*/
void getMacAddress(char hexChars[])
{
	uint8_t *bssid = WiFi.BSSID();
	for (int i = 0; i < 6; i++)
	{
		hexChars[i * 3] = getHexChar(bssid[i] / 16);
		hexChars[i * 3 + 1] = getHexChar(bssid[i] % 16);
		hexChars[i * 3 + 2] = ':';
	}
	hexChars[17] = 0;
}

/*
	ESP meldet Statusinformationen, wie Mac-Adresse, SSID, WLAN-Funk-Feldstärke,
	freien Heap-Speicher, den eigenen ThingName, MQTT-Username, MQTT-Broker-IP,
	MQTT-Broker-Port
		http://192.168.0.100/state
*/
void handleStateRequest()
{
	char hexChars[18];
	char response[400];
	char buffer[400];
	getMacAddress(hexChars);
	ThingConfig.getConfigJson(buffer, 400);

	sprintf(response, "espMacAddress: %s<br/>\nwifiName: %s<br/>\nwifiStrength: %d<br/>\nfreeHeap: %d<br/>\nConfig: %s<br/>",
			hexChars, WiFi.SSID().c_str(), WiFi.RSSI(), ESP.getFreeHeap(), buffer);

	Serial.printf("*HS response-length: %d", strlen(response));
	Serial.println("");
	Serial.print(F("*HS: State: "));
	Serial.println(response);
	//Returns the HTTP response
	HttpServer.send(200, "text/html", response);
}

//!!! Help-Request so umbauen, dass (wo es sinnvoll ist) gleich ein Link ausgegeben wird (z.B. state, ledfast, ledslow, ledoff, reset)
void handleHelpRequest()
{
	char response[500];
	sprintf(response, "getconfig<br/>setconfig?key1=value1&key2=value2<br/>state<br/>led/(fast|slow|off)<br/>reset<br/>setactor?actor=actorname&value=1<br/>getsensor?sensor=sensorname<br/>");
	Serial.printf("*HS help response, response-length: %d\n", strlen(response));
	//Returns the HTTP response
	HttpServer.send(200, "text/html", response);
}

/**
 * Get the current configuration
 * */
void handleGetConfigRequest()
{
	char response[500];
	ThingConfig.getConfigJson(response, 400);
	Serial.print(F("*HS: Response-Config: "));
	Serial.println(response);
	//Returns the HTTP response
	HttpServer.send(200, "text/html", response);
}

/**
 * Die interne Led kann per Http gesteuert werden
 * Mit dem Parameter fast=1 blinkt die LED schnell,
 * mit slow=1 blinkt sie langsam und mit off=1 wird
 * sie abgeschaltet
 *		http://192.168.0.100/led/fast
 *		http://192.168.0.100/led/slow
 *		http://192.168.0.100/led/off
*/
void handleInternLedFastRequest()
{
	InternLed.blinkFast();
	Serial.println(F("LED blinks fast"));
	HttpServer.send(200, "text/html", "LED blinks fast");
}

void handleInternLedSlowRequest()
{
	InternLed.blinkSlow();
	Serial.println(F("LED blinks slow"));
	HttpServer.send(200, "text/html", "LED blinks slow");
}

void handleInternLedOffRequest()
{
	InternLed.blinkOff();
	Serial.println(F("LED off"));
	HttpServer.send(200, "text/html", "LED off");
}

/*
	Credentials für das WLAN werden zurückgesetzt
	SPIFFS wird formatiert
*/
void handleResetEspRequest()
{
	InternLed.blinkSlow();
	const char *response = "Formating the FlashFileSystem<br>Wifi-Connection reset settings, reboot ESP";
	HttpServer.send(200, "text/html", response); //Returns the HTTP response
	delay(1000);
	Serial.println(F("*HS Format flash memory!"));
	SPIFFS.format();
	Serial.println(F("*HS Done!"));
	WiFi.disconnect(true);
	WiFiManager wm;
	Serial.println(F("*HS WifiManager resetSettings()"));
	wm.resetSettings();
	Serial.println(F("*HS Done!"));

	Serial.println(F("*HS Done!"));

	InternLed.blinkOff();
}

/*************************************** Allgemeine HttpServer-Methoden ******************************/

//flag for saving data
bool saveConfig = false;

//callback notifying us of the need to save config
void saveConfigToFileCallback()
{
	Serial.println("*HS Should save config");
	saveConfig = true;
}

//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster
}


/*
	Accesspoint definieren und öffnen. Webserver starten und auf Get-Request warten
*/
void HttpServerClass::init()
{
	char buffer[200];
	InternLed.init(LED_BUILTIN);
	//set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  	_wifiManager.setAPCallback(configModeCallback);
	// Konfiguration aus ThingConfig einlesen
	ThingConfig.getConfigJson(buffer, 500);	// Gesamte Config als JSON-String auslesen
	Serial.printf("*HS Last saved config-json: %s\n",buffer);

	// WiFiManger vorbereiten für Konfiguration per Webseite
	WiFiManagerParameter custom_mqtt_server("server", "mqtt server", ThingConfig.getValue("server"), MQTT_SERVERADDRESS_LENGTH);
	WiFiManagerParameter custom_mqtt_port("port", "mqtt port", ThingConfig.getValue("port"), MQTT_PORT_LENGTH);
	WiFiManagerParameter custom_mqtt_user("user", "mqtt user", ThingConfig.getValue("user"), MQTT_USERNAME_LENGTH);
	WiFiManagerParameter custom_mqtt_pass("password", "mqtt pass", ThingConfig.getValue("password"), MQTT_PASSWORD_LENGTH);
	WiFiManagerParameter custom_mqtt_thing_name("thingname", "mqtt thing name", ThingConfig.getValue("thingname"), MQTT_THING_NAME_LENGTH);

	_wifiManager.setSaveConfigCallback(saveConfigToFileCallback);

	_wifiManager.addParameter(&custom_mqtt_server);
	_wifiManager.addParameter(&custom_mqtt_port);
	_wifiManager.addParameter(&custom_mqtt_user);
	_wifiManager.addParameter(&custom_mqtt_pass);
	_wifiManager.addParameter(&custom_mqtt_thing_name);

	// WiFiManager wifiManager; // WiFiManager anlegen
	// _wifiManager.autoConnect(PHSEM_AP_NAME);

	// Namen des AP mit Suffix MAC-Adresse erzeugen
	// char hexChars[18]; // Macadresse ermitteln
	// getMacAddress(hexChars);
	// char apName[25];
	// sprintf(apName, "ESP_%s", hexChars); // SSID zusammenstoppeln
	InternLed.blinkFast();				 // Signal dafür, dass ESP mit eigenem AP auf 192.168.4.1 wartet

	bool isConnected = true;
	//fetches ssid and pass and tries to connect
	//if it does not connect it starts an access point with the specified name
	//here  "AutoConnectAP"
	//and goes into a blocking loop awaiting configuration
  	if (!_wifiManager.autoConnect()) {
    	Serial.println("failed to connect and hit timeout");
    	//reset and try again, or maybe put it to deep sleep
    	isConnected=false;
	}

	// _wifiManager.setConfigPortalTimeout(240);
	// //	if (!wifiManager.autoConnect(apName))  // gecachte Credentials werden verwendet
	// if (!wifiManager.autoConnect(PHSEM_AP_NAME)) // gecachte Credentials werden verwendet
	// {
	// 	// Serial.println(F("Failed to connect. Reset and try again..."));
	// 	isConnected = false;
	// 	// delay(3000);
	// 	// ESP.reset();						//reset and try again
	// 	// delay(5000);
	// }
	// delay(100);


	if (saveConfig)
	{
		// read updated parameters
		// save to configfile
		Serial.println(F("*HS: Save config-values to file"));
		ThingConfig.setValue("server", custom_mqtt_server.getValue());
		ThingConfig.setValue("port", custom_mqtt_port.getValue());
		ThingConfig.setValue("user", custom_mqtt_user.getValue());
		ThingConfig.setValue("password", custom_mqtt_pass.getValue());
		ThingConfig.setValue("thingname", custom_mqtt_thing_name.getValue());
		ThingConfig.saveConfig();
		ThingConfig.readConfig();
		ThingConfig.getConfigJson(buffer, 500);	// Gesamte Config als JSON-String auslesen
		Serial.printf("*HS Saved config-json: %s\n",buffer);

		delay(1000);
	}
	if (isConnected)
	{
		Serial.println(F("*HS: Connected to WiFi!"));
		const char* thingName =ThingConfig.getValue("thingname");
		//const char* thingName ="esp32-demo";
		// Serial.printf("*HS: Thingname from config: %s\n", thingName);
		if(strlen(thingName) > 0){
			Serial.printf("*HS: Set hostname to %s\n", thingName);
			char hostName[50];
			strlcpy(hostName, thingName, 50);
			for(int i=0; i<strlen(hostName); i++){
				if(hostName[i]=='/'){
					hostName[i] = '-';
				}
			}
			WiFi.setHostname(hostName);
			//ArduinoOTA.setHostname(hostName);
		}

		Serial.printf("*HS: Hostname: %s\n", WiFi.getHostname());
		// Serial.println(wifi_station_get_hostname());
		InternLed.blinkOff();
		//InternLed.blinkSlow();
		on("/reset", handleResetEspRequest);		  // ESP resetten
		on("/state", handleStateRequest);			  // Status (Heap, Funkfeldstärke, ...) ausgeben
		on("/help", handleHelpRequest);			  
		on("/led/fast", handleInternLedFastRequest);
		on("/led/slow", handleInternLedSlowRequest);
		on("/led/off", handleInternLedOffRequest);
		on("/getconfig", handleGetConfigRequest);
		on("/setconfig", handleSetConfigRequest);
		begin();
		// Serial.println(F("*HS: Webserver started"));
		delay(3000);
		Serial.println(F("*HS: Webserver started"));
	}
	else
	{
		Serial.println(F("!HS: not connected to WiFi!"));
		delay(3000);
		ESP.restart(); //reset and try again
	}
}

// Quasi Singletonimplementierung
HttpServerClass HttpServer;
