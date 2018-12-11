# Library ThingConfig

## Ziele

Diese Library implementiert die Verwaltung der Konfigurationsdaten eines Things. Dazu gehören der Thingname und die IP-Adresse samt Portnummer des MQTT-Brokers oder auch die IP-Adresse des NTP-Servers.
Die Konfiguration wird im SPIFFS persistent als JSON-Objekt gespeichert.
Der Zugriff erfolgt einfach über das Singletonobjekt ThingConfig mit je einer Methode zum Lesen/Schreiben eines Konfigurationseintrages. Die Konfigurationseinträge werden über einen Key identifiziert. Die gespeicherten Konfigurationsvalues sind einheitlich vom Typ char[].

## Hardware

Es wird lediglich ein ESP benötigt.

## Software

### Abhängigkeiten

- ArduinoJson für die einfache statische Verwaltung der Konfigurationseinträge
- SPIFFS zum Zugriff aus das Dateisystem

### Testanwendung am ESP

Das Testprogramm liest die Konfiguration des letzten Testlaufes aus, ändert Konfigurationen, speichert sie ab und liest den JSON-Inhalt aus der Datei aus.

````c
  ThingConfig.readConfig();  	// alte Config ausgeben
  ThingConfig.clearConfigFile();	// Config im Hauptspeicher löschen

  ThingConfig.setValue("key1", "value1");	// Config imHauptspeicher ändern und gleich persistieren
  ThingConfig.setValue("key2", "value2");
  Serial.printf("Setted Config, Key1: %s\n", ThingConfig.getValue("key1"));
  char buffer[200];
  ThingConfig.getConfigJson(buffer, 200);	// Gesamte Config als JSON-String auslesen
  Serial.printf("Values in config-json: %s",buffer);
  ThingConfig.setValue("key1", "value999");	// Konfiguration ändern
  ThingConfig.getConfigJson(buffer, 200);	// Gesamte Config als JSON-String auslesen
  Serial.printf("Values in config-json: %s",buffer);

````


### UI des Testprogrammes

````
ThingConfigTest
===============
Save Config with static values
*TC: readConfig(), mounted file system
*TC: reading config file
*TC: opened config file
*TC: Read Config:{"key1":"value999","key2":"value2"}
*TC: json config object initialized
*TC: clearConfig(), mounted file system
*TC: deleting config file
*TC: saving config
*TC: Saved Config:{"key1":"value1"}
*TC: saving config
*TC: Saved Config:{"key1":"value1","key2":"value2"}
Setted Config, Key1: value1
Values in config-json: {"key1":"value1","key2":"value2"}*TC: saving config
*TC: Saved Config:{"key1":"value999","key2":"value2"}
Values in config-json: {"key1":"value999","key2":"value2"}
````

#### Testanwendung verwenden

- Infrastruktur anpassen
	- Bibliothek (ArduinoJson) in richtiges Verzeichnis kopieren
- Programm starten und Verhalten über seriellen Monitor kontrollieren

### Library

````c
#pragma once
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson

/*
	Verwaltung der Konfigurationsdaten der App in einer Datei (config.json) im SPI-FlashFileSystem (SPIFFS).
	Die Konfiguration ist als JSON-Objekt gespeichert.
	Der Zugriff erfolgt über Key/Value-Pairs, deren Typen generell char[] sind. 
	ThingConfig ist ein Singleton und ermöglicht daher den einfachen Zugriff auf Konfigurationsdaten
	von überall.
	Änderungen von Konfigurationen werden automatisch persistiert.
*/

class ThingConfigClass
{

public:
	/*
	* Die Konfiguration im Speicher und die Konfigurationsdatei 
	* werden gelöscht.
	*/
	void clearConfigFile();

	/**
	 * Konfiguration aus Datei in JSON-Objekt lesen
	 */
	void readConfig();

	/**
	 * In JSON-Objekt gechachte Konfiguration in Datei schreiben
	 */	
	void saveConfig();

	/*
	* Die Konfiguration wird als JSON-String zurückgegeben
	*/
	void getConfigJson(char* buffer, size_t size);

	/**
	 * Wert aus der Konfiguration auslesen (Key ist case-sensitiv)
	 */
	const char* getValue(const char* key);

	/**
	 * Wert in Konfiguration anlegen oder überschreiben
	 */
	void setValue(const char* key, const char* value);

private:
	void initJsonConfig();
	StaticJsonBuffer<200> _jsonBuffer;
	JsonObject* _jsonConfig;
};

extern ThingConfigClass ThingConfig;
````


