// ThingConfig.h
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
	void clearConfig();

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
	StaticJsonBuffer<1000> _jsonBuffer;
	JsonObject* _jsonConfig;
};

extern ThingConfigClass ThingConfig;


