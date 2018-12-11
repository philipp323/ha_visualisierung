#include <FS.h>
#include <SPIFFS.h>
#include "ThingConfig.h"

#define FORMAT_SPIFFS_IF_FAILED true  // beim ersten Start auf dem ESP wird das Filesystem angelegt

/*
 * Die Konfiguration im Speicher wird gelöscht.
 * Es wird ein leeres Json-Objekt erzeugt.
 */
void ThingConfigClass::initJsonConfig(){
	_jsonBuffer.clear();
	JsonObject &root = _jsonBuffer.createObject();
	_jsonConfig = &root;
	Serial.print("*TC initJsonConfig(), empty Config: ");
	_jsonConfig->printTo(Serial);
	Serial.println();
   	Serial.println(F("*TC: json config object initialized"));
}

/*
 * Die Konfiguration im Speicher und die Konfigurationsdatei 
 * werden gelöscht.
 */
void ThingConfigClass::clearConfig(){
	initJsonConfig(); // Cache ebenfalls löschen
	if (SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    	Serial.println(F("*TC: clearConfig(), mounted file system"));
		if (SPIFFS.exists("/config.json")) {
			//file exists, reading and loading
			Serial.println(F("*TC: deleting config file"));
			SPIFFS.remove("/config.json");
			} else{
				Serial.println(F("!TC: readConfig(), json config does not exist"));
			}
    } else {
    	Serial.println(F("*TC: failed to mount FS"));
  	}
}

/*
 * Die Konfiguration wird als JSON-String zurückgegeben
 */
void ThingConfigClass::getConfigJson(char* buffer, size_t size){
	_jsonConfig->printTo(buffer, size);
}

/**
 * Konfiguration aus Datei in JSON-Objekt lesen
 */
void ThingConfigClass::readConfig(){
	initJsonConfig();  // damit nicht die alte Config erhalten bleibt
	bool isConfigRead = false;
	if (SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    	Serial.println(F("*TC: readConfig(), mounted file system"));
		if (SPIFFS.exists("/config.json")) {
			//file exists, reading and loading
			Serial.println(F("*TC: reading config file"));
			File configFile = SPIFFS.open("/config.json", FILE_READ);
			if (configFile) {
				Serial.println(F("*TC: opened config file"));
				JsonObject &root = _jsonBuffer.parseObject(configFile);
				_jsonConfig = &root;
				if (_jsonConfig->success()) {	
					Serial.print(F("*TC: Read Config:"));
					_jsonConfig->printTo(Serial);
					Serial.println();
					isConfigRead=true;
				} else {
					Serial.println(F("!TC: readConfig(), failed to read json"));
				}	
				configFile.close();
			} else {
				Serial.println(F("!TC: readConfig(), failed to load json config"));
			}		
		} else{
			Serial.println(F("!TC: readConfig(), failed to open json config"));
		}
    } else {
    	Serial.println(F("*TC: failed to mount FS"));
  	}
	if(!isConfigRead){  // leere Config abspeichern
		initJsonConfig();
		saveConfig();
		delay(1000);
    	Serial.println(F("*TC: readConfig() finished"));
	}
}

/**
 * In JSON-Objekt gechachte Konfiguration in Datei schreiben
 */
void ThingConfigClass::saveConfig(){
	if (SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
		Serial.println(F("*TC: saveConfig(), saving config"));
		Serial.print(F("*TC: Config to save:"));
		_jsonConfig->printTo(Serial);
		Serial.println();
		File configFile = SPIFFS.open("/config.json", FILE_WRITE);
		Serial.println(F("*TC: config file opened for writing"));
		if (!configFile) {
			Serial.println(F("*TC: failed to open config file for writing"));
		}
		else{
			_jsonConfig->printTo(configFile);
			configFile.close();
			Serial.print(F("*TC: Saved Config:"));
			_jsonConfig->printTo(Serial);
			Serial.println();
			readConfig();
		}
	} else {
		Serial.println(F("*TC: failed to mount FS"));
	}
}

/**
 * Wert aus der Konfiguration auslesen (Key ist case-sensitiv)
 */
const char* ThingConfigClass::getValue(const char* key){
	const char* value = _jsonConfig->get<char*>(key);
	if(value == nullptr){
		return "";
	}
	return value;
}


/**
 * Wert in Konfiguration anlegen oder überschreiben
 */
void ThingConfigClass::setValue(const char* key, const char* value){
	_jsonConfig->set(key, value);
	Serial.print(F("*TC: Setted value:"));
	_jsonConfig->printTo(Serial);
	saveConfig();
}

ThingConfigClass ThingConfig;

