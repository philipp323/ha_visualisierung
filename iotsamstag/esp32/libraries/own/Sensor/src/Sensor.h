#pragma once

#include <ArduinoJson.h> //https://github.com/bblanchon/ArduinoJson

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

	virtual void getMqttPayload(char* payload, float measurement);  //! wozu der char* als Rückgabetyp

  protected:
	/*
		Aktueller Messwert wird eingetragen. Löst bei ausreichender Änderung
		Notification aus.
	*/
	void setMeasurement(float value);

  private:
	StaticJsonBuffer<200> _jsonBuffer;

	char _thingName[50];
	char _name[20];
	char _unit[10];
	float _threshold;
	float _measurement;		// letzter übertragener Wert
	float _lastMeasurement; // letzter gemessener Wert
	long _time;
};
