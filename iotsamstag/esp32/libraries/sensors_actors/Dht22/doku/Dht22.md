# Library Dht22

## Ziele

Der ESP soll mit Hilfe dieser Libary die Werte von einen Dht22 Sensor lesen.
Dieser Libary blockiert nicht den ESP dadurch gibt es keine wartezeiten.

Diese Libary ist von der Libary Ticker abhägig.

## Hardware

Es wird lediglich ein ESP32 und ein Dht22 Senor benötigt.

## Software

### Beispielaufrufe

```c
Dht22 dht22(5);

void setup() {

}

unsigned long lastReportingTime=0;
void loop() {
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
```

### Erklärung

* ```Dht22( uint8_t pin)``` Muss als erstes aufgerufen werden. Darin wird der Pin festlegen. 

### Library

##### Methoden

| ```Dht22::``` | Erklärung |
|-|-|
|```Dht22( uint8_t pin)```| Setzt den Pin des Dht22 Sensor. |
|```float getTemperature()``` | Gibt den letzten Messwert der Temperatur zurück.  |
|```float getHumidity()``` | Gibt den letzten Messwert der Luftfeuchtigkeit zurück. |
|```uint8_t getLastErrorCode()``` | Gibt den letzten Error Code der Libary zurück. |
|```uint8_t getLastErrorPosition()``` | Gibt den letzte Position des Errors zurück. |
|```long getSecondsFromLastError()``` | Gibt die Zeit in Sekunden seit dem letzten Error zurück. |
|```long getSecondsFromLastCorrectMeasurement()``` | Gibt die Zeit in Sekunden seit der letzten richtigen Messung zurück. |