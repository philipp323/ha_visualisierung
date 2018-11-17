#include <Arduino.h>
#include <ThingConfig.h>

/*************************************** Setup ******************************/
void setup() {
  char buffer[200];
  Serial.begin(115200);                 //Initialisierung der seriellen Schnittstelle
  Serial.println();
  Serial.println();
  Serial.println(F("ThingConfigTest"));
  Serial.println(F("==============="));
  ThingConfig.readConfig();  	// alte Config ausgeben
  ThingConfig.getConfigJson(buffer, 200);	// Gesamte Config als JSON-String auslesen
  Serial.printf("Last saved config-json: %s\n",buffer);
  Serial.printf("After clearConfig\n");
  ThingConfig.clearConfig();	// Config im Hauptspeicher löschen
  ThingConfig.readConfig();
  const char* value =ThingConfig.getValue("undefinedkey");
  Serial.printf("read of undefined key, value: '%s'\n",value);
  ThingConfig.setValue("key1", "value1");	// Config imHauptspeicher ändern und gleich persistieren
  ThingConfig.setValue("key2", "value2");
  Serial.printf("Setted Config, Key1: %s\n", ThingConfig.getValue("key1"));
  ThingConfig.getConfigJson(buffer, 200);	// Gesamte Config als JSON-String auslesen
  Serial.printf("Values in config-json: %s",buffer);
  ThingConfig.setValue("key1", "value999");	// Konfiguration ändern
  ThingConfig.getConfigJson(buffer, 200);	// Gesamte Config als JSON-String auslesen
  Serial.printf("Values in config-json: %s",buffer);
}

/*************************************** Loop ******************************/
void loop() {
	delay(1);
}
