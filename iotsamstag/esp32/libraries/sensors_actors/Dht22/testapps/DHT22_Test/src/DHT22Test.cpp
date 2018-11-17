#include <Dht22.h>
#include <Arduino.h>

/****************************************** Statische Variablen **************************************/

Dht22 dht22(5);



/****************************************** Setup **************************************/


void setup() {
	Serial.begin(115200);                 //Initialisierung der seriellen Schnittstelle
	Serial.println();
	Serial.println("Dht22Test");
	Serial.println("=========");
	Serial.println();
}

/****************************************** Loop **************************************/

unsigned long lastReportingTime=0;
void loop() {
	delay(1);
	if(millis()-lastReportingTime > 5000){
		Serial.println();
		Serial.println("----------------------------------------------------------------------------");
		Serial.print("Humidity: ");
		Serial.print(dht22.getHumidity());
		Serial.print(", Temperature: ");
		Serial.print(dht22.getTemperature());
		Serial.print(", Age of measurement: ");
		Serial.print(dht22.getSecondsFromLastCorrectMeasurement());
		Serial.println(" sec");
		Serial.print("Errors, last pos: ");
		Serial.print(dht22.getLastErrorPosition());
		Serial.print(", last errorcode: ");
		Serial.print(dht22.getLastErrorCode());
		Serial.print(", Age of last error: ");
		Serial.print(dht22.getSecondsFromLastError());
		Serial.println(" sec");
		Serial.println("----------------------------------------------------------------------------");
		lastReportingTime=millis();
	}
}

