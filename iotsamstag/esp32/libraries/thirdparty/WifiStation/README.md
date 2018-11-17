# Library WifiStation

## Ziele

Mit vorhandenem WLAN verbinden.

## Hardware

Es wird lediglich ein NodeMCU benötigt.

## UserInterface

Der Benutzer gibt über den seriellen Monitor die Zugangsdaten ein und das System verbindet sich mit dem WLAN.

KORREKTUR:
Wird vom ESP per serieller Schnittstelle eingelesen, funktioniert zeitweise die WiFi-Verbindung nicht. Dieses Verhalten ist beim Wemos-Mini häufiger zu beobachten als beim NodeMCU.
Die WLAN-Zugangsdaten werden nicht eingegeben, sondern sind als Konstante definiert. Nach dem Test der Anwendung wieder löschen!
Im Echtbetrieb kommen die Credentials entweder vom Flash-Filesystem oder per Http-Request.

## Software

### Testanwendung am NodeMCU

Ein kleines Testprogramm überprüft die Funktion der Bibliothek. Der Node verbindet sich mit dem in konstanten definierten WLAN.


````c
#include <WiFiUdp.h>
#include <WiFiServer.h>
#include <WiFiClientSecure.h>
#include <WiFiClient.h>
#include <ESP8266WiFiType.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFi.h>
#include "WifiStation.h"


/************************************ WiFi Setup *******************************************/

#define BUFFER_SIZE  20
char ssid[BUFFER_SIZE]="linksysAtHome";
char password[BUFFER_SIZE]="YJXPF4WAC7";
char name[BUFFER_SIZE]="WifiStation";

char ssid2[BUFFER_SIZE];
char password2[BUFFER_SIZE];


/*
	Liest eine Zeile von der seriellen Schnittstelle in den
	Puffer ein.
	Die Methode blockiert, bis der Puffer voll ist oder
	ein Zeilenende entdeckt wurde.
*/
void readLineToCharArray(char buffer[], uint maxLength) {
	int index = 0;
	byte byteRead = 0;
	for (int i = 0; i < maxLength; i++) {
		buffer[i] = '\0';
	}
	// Eingabepuffer leeren
	while (Serial.available() > 0) {
		Serial.read();
		yield();
	}
	while (index + 1 < maxLength && byteRead != 10 && byteRead != 13) {
		yield();
		while (Serial.available() == 0) {
			yield();
			delay(1);
		}
		byteRead = Serial.read();
		Serial.println(byteRead);
		if (byteRead != 10 && byteRead != 13) {
			buffer[index] = (char)byteRead;
			index++;
		}
	}
	buffer[index] = '\0';
}

void writeCharArray(char array[]) {
	int index = 0;
	Serial.println("Char-Array");
	while (index < 100 && array[index] != 0) {
		Serial.println(array[index]);
		index++;
	}
}

void setup() {
	Serial.begin(115200);                 //Initialisierung der seriellen Schnittstelle
	Serial.println();
	Serial.println("WifiStationTest");
	Serial.println("===============");
	Serial.println();
	//Serial.println("Enter WiFi-Credentials");
	//Serial.println("SSID senden");
	//readLineToCharArray(ssid2, BUFFER_SIZE);
	//Serial.print("Connect with ssid: '");
	//Serial.print(ssid2);
	//Serial.println("'");
	//delay(100);
	//Serial.println("Password senden");
	//readLineToCharArray(password2, BUFFER_SIZE);
	//Serial.print("Password: '");
	//Serial.print(password2);
	//Serial.println("'");

	//writeCharArray(ssid2);

	//writeCharArray(ssid);
	//writeCharArray(password);


	//WiFi.persistent(false);
	WifiStation.init(ssid, password, name);
	if (!WifiStation.isConnected()) {
		Serial.println("!!! setup(), Keine Verbindung zum WLAN!");
	}
	else {
		Serial.println("Mit Wifi verbunden");
	}
}

void loop() {
	delay(1);
}
````

