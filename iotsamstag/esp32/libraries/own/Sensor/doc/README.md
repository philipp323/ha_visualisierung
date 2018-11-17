# Library Sensor

## Ziele

Abstrakte Basisklasse zur Verwaltung der allgemeingültigen Daten und Verhaltens von Sensoren.
Jeder Sensor ist innerhalb des Nodes über ID eindeutig identifizierbar. Er liefert seinen Messwert automatisch per Log an den Server, wenn er sich um eine per Parameter festgelegte Schwelle geändert hat. Er meldet seinen Messwert aber zumindest alle 15 Minuten.

## Software

### Abhängigkeiten

- Logger


### Testanwendung am ESP32

Die Klasse Sensor wird über das Testprogramm der Library Node mitgetestet.


#### Library


#### Headerdatei Sensor.h

````c
/*
	Ein Sensor wird im System über den Thingnamen, der innerhalb des Things eindeutig ist)
	identifiziert.
	Der Sensor verwaltet auch die Einheit seines Messwertes und die Änderungsschwelle,
	die erreicht werden muss, um eine neue Meldung des Messwertes auszulösen.

	Sensor ist eine abstrakte Klasse. Die konkreten Sensorklassen müssen nur mehr die
	Methode measure() überschreiben, mit deren Hilfe der konkrete Code zum Einlesen 
	der Messwerte implementiert wird.
*/
class Sensor
{
  public:
	/*
		Sensor wird mit seinem Namen, einer innerhalb des Things eindeutigen id,
		der Einheit des Messwertes und der Schwelle, ab der eine Änderung des 
		Messwertes gemeldet wird.
	*/
	Sensor(const char *thingName, const char *name, const char *unit, float threshold);

	/*
		Liefert den letzten gemessenen Messwert. Dieser muss nicht mit dem 
		aktuellen Messwert übereinstimmen, da sich der aktuelle Wert innerhalb
		der Schwelle geändert haben kann und daher noch nicht übermittelt wurde.
	*/
	float getLastMeasurement();

	/*
		Liefert den Namen des Sensors
	*/
	char *getName();

	/*
		Abstrakte Messmethode muss vom konkreten Sensor überschrieben (implementiert)
		werden.
	*/
	virtual void measure()=0; // abstrakte Sensoren können nicht in Liste verwaltet werden

	virtual char* getMqttPayload(char* payload, float measurement);

  protected:
	/*
		Aktueller Messwert wird eingetragen. Löst bei ausreichender Änderung
		Notification aus.
	*/
	void setMeasurement(float value);

  private:
	StaticJsonBuffer<200> _jsonBuffer;

	char _thingName[30];
	char _name[20];
	char _unit[10];
	float _threshold;
	float _measurement;		// letzter übertragener Wert
	float _lastMeasurement; // letzter gemessener Wert
	long _time;
};

````

### Beispiel für einen konkreten Temperatursensor

Am Beispiel des Temperatursensors DHT22 wird die Verwendung der Klasse Sensor demonstriert.

Der Temperatursensor leitet von der Klasse Sensor ab und überschreibt die Methode zum Ermitteln des Messwertes.

````c
#include <Arduino.h>
#include <Dht22.h>
#include <Sensor.h>

class Dht22_Temperature : public Sensor
{
public:
	Dht22_Temperature(Dht22* dht, const char* nodeName, const char* name, const char* unit, float threshold);
	virtual void measure();
private:
	Dht22* _dht;
};

````

Die Implementierung benötigt ebenfalls nur einige Zeilen Code

````c
Dht22_Temperature::Dht22_Temperature(Dht22* dht, const char* thingName, 
			const char* name, const char* unit, float threshold) 
		:Sensor(thingName, name, unit, threshold)
{
	_dht = dht;
}

void Dht22_Temperature::measure()
{
	float temperature = _dht->getTemperature();
	setMeasurement(temperature);
}

````

Im Hauptprogramm wird der Sensor instanziert und im Node eingetragen.

````c
void setup() {
	...
	if (WifiStation.isConnected())
	{
		...
		Sensor* temperatureSensorPtr = new Dht22_Temperature(dhtSensorPtr, thingName, "temperature", "Grad", 0.2);
		Node.addSensor(sensorPtr);
	}

}

````
