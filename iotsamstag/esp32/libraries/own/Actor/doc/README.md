# Library Actor

## Allgemeines

Abstrakte Basisklasse zur Verwaltung der allgemein gültigen Daten und Verhaltens von Aktoren.
Jeder Aktor ist innerhalb des Nodes über ID eindeutig identifizierbar. Für die Verwaltung des Zustandes des Aktors enthält jeder Aktor einen Sensor mit der selben ID und dem selben Namen. Der gewünschte Wert des Aktors kann per Methode gesetzt werden. Dieser gesetzte Wert muss nicht immer mit dem aktuellen Wert (Sensor des Aktors) übereinstimmen. Zum Beispiel kann der gewünschte Wert eines Rollladenaktors bei 50% Behanghöhe eingestellt werden, der aktuelle Wert allerdings 0% betragen. Es dauert dann einige Sekunden, bis der Wert erreicht wird. Ähnliches gilt bei der Steuerung von Temperaturen, Füllständen, ...



## Software


### Testanwendung am ESP32

Die Klasse Sensor wird über das Testprogramm der Library Thing mitgetestet.


#### Library


#### Headerdatei Actor.h

````c
/*
	Die Klasse Actor dient als abstrakte Basisklasse für beliebige Typen von Aktoren.
	Jeder Actor hat einen aktuellen Zustand, der als Sensor behandelt wird.
	Weiters gibt es einen Wert, auf den der Actor gesetzt werden soll. Nach endlicher Zeit
	stimmen die beiden Werte überein, sofern der Actor korrekt funktioniert.
	Beispiel PowerSwitch:
				Der aktuelle Wert repräsentiert den Zustand des Schalters (1 für ein, 0 für aus)
				Der zu setzende Wert gibt den Zielzustand an. Wird dieser Wert per Commando umgesetzt, 
				wird innerhalb einiger Millisekunden das SSR geschaltet und auch der aktuelle Wert angepasst.
	Beispiel Rollladenschalter:
				Der aktuelle Wert zwischen 0 und 1 repräsentiert die Stellung des Rollladens (0 geschlossen, 0,5
				halb offen, 1 offen). Wird der Zielwert neu gesetzt, dauert es natürlich einige Sekunden, bis
				der aktuelle Wert den Zielwert erreicht.

	Alle speziellen Aktoren (z.B. Powerswitch) leiten von dieser Basisklasse ab initialisieren ihren 
	zugehörigen Sensor (z.B. PowerSwitchSensor) und überschreiben die Methode syncValue(), die dafür sorgt,
	dass der aktuelle Aktorwert mit dem gesetzten übereinstimmt.
*/
class Actor
{
  public:
	/*
		Der Aktor wird mit seinen Stammdaten initialisiert. Die Parameter
		unit und threashold werden an den Messwert (Sensor) des Aktors 
		weiter gereicht.
	 */
	Actor(const char *thingName, const char *name);

	/**
	 * Ein eingehendes Mqtt-Kommando wird ausgewertet und in einen
	 * Kommandowert übersetzt 
	 */
	void processMqttCommand(const char *payload);

	// zuletzt gesetzter Zielwert des Aktors
	void setCommandValue(float value);
	float getCommandValue();

	// Aktueller Zustand des Aktors
	void setStateValue(float value);
	float getStateValue();

	// Flexible Mappingmöglichkeit in Abhängigkeit des Aktortyps
	float mapPayloadToCommandValue(const char *payload);
	virtual void mapStateValueToPayload(char* payload, float state);

	/*
		Konkreter Actor wird aufgefordert, den gewünschten Zielwert zu setzen,
		falls er vom aktuellen Wert abweicht.
	 */
	void syncState();

	char *getName();


  protected:

	// muss vom konkreten Aktor implementiert werden
	virtual float readStateFromActor()=0;
	virtual void setActorTo(float value)=0;

	char _thingName[30];
	char _name[20];
	long _time;
	float _commandValue;
	float _stateValue;

  private:
	StaticJsonBuffer<200> _jsonBuffer;
	MqttSubscription _actorSubscription;
	char _actorTopic[50];
};

````

### Beispiel für einen konkreten Actor PowerSwitch

Ein Powerswitch schaltet per SSR die Netzspannung ein und aus. Die Ansteuerung erfolgt über einen digitalen Ausgang mit Vorwiderstand.

PowerSwitch leitet von Actor ab und hat als Spezialität nur die Pinnummer des digitalen Ausgangs.


````c
/*
	PowerSwitch ist ein spezieller sehr einfacher Aktor als
	Ein/Ausschalter über einen digitalen Ausgang.
*/
class PowerSwitch : public Actor
{
 public:
	 PowerSwitch(const int pinNumber, const bool isInverse, const char* thingName, const char* name);
	/**
	 * Der konkrete Aktor ändert den Zustand entsprechend
	 * z.B. schaltet den Schalter ein/aus
	 */
	virtual void setActorTo(float value);
	
	/**
	 * Der konkrete Aktor liefert seinen aktuellen Zustand
	 */
	
	virtual float readStateFromActor();
private:
	int _pinNumber;
	bool _isInverse;
};

````

Die Implementierung der Methoden ist auch nicht aufregend.

````c
#include <PowerSwitch.h>
#include <PowerSwitchSensor.h>
#include <Thing.h>

/*
	Der Schalter hat keine Einheit des Messwertes EIN/AUS und auch eine fixe Schwelle von 0.001,
	da sowieso nur die Werte 0 und 1 verwendet werden.
*/
PowerSwitch::PowerSwitch(const int pinNumber, const bool isInverse, const char* thingName, const char* name) : Actor(thingName, name)
{
	_pinNumber = pinNumber;
	_isInverse = isInverse;
	_commandValue = 0;
	pinMode(_pinNumber, OUTPUT);
	if(isInverse){
		digitalWrite(_pinNumber, 1);
	}
	else{
		digitalWrite(_pinNumber, 0);
	}
}

void PowerSwitch::setActorTo(float value)
{
	if(_isInverse){
		if (value > 0.5)
		{
			Serial.println(F("*PS: set pin low!"));
			digitalWrite(_pinNumber, 0); // inverse Logik
		}
		else
		{
			Serial.println(F("*PS: set pin high!"));
			digitalWrite(_pinNumber, 1);
		}
	}
	else{  // normale (nicht inverse Logik)
		if (value > 0.5)
		{
			Serial.println(F("*PS: set pin high!"));
			digitalWrite(_pinNumber, 1); // normale Logik
		}
		else
		{
			Serial.println(F("*PS: set pin low!"));
			digitalWrite(_pinNumber, 0);
		}

	}
}

float PowerSwitch::readStateFromActor()
{
	if(_isInverse){
		if (digitalRead(_pinNumber) == HIGH)
		{
			return 0;
		}
		else{
			return 1;
		}
	}
	else{
		if (digitalRead(_pinNumber) == HIGH)
		{
			return 1;
		}
		else{
			return 0;
		}
	}
}

````

Zur Verwaltung des aktuellen Zustands des Powerswitch wird ein eigener Sensor (PowerSwitchSensor) angelegt, der wider sehr einfach ist:

````c
#include "arduino.h"
#include "Sensor.h"

/*
	Der PowerSwitchSensor ist der zum Actor PowerSwitch zugehörige Sensor.
	Damit wird der aktuelle Zustand des digitalen Schaltpins ausgelesen.
*/
class PowerSwitchSensor : public Sensor
{

public:
	PowerSwitchSensor(const char* nodeName, const char* name, const char* unit, float threashold, uint8_t pinNumber);
	virtual void measure();

private:
	int _pinNumber;

};

````


Implementierung überschreibt die Methode Measure() und liest den aktuellen Zustand des IO-Pins aus.

````c
#include <PowerSwitchSensor.h>

PowerSwitchSensor::PowerSwitchSensor(const char *nodeName, const char *name, const char *unit, float threashold, uint8_t pinNumber)
	: Sensor(nodeName, name, unit, threashold)
{
	Serial.print(F("*PS: Sensor initialized: "));
	Serial.println(name);
	_pinNumber = pinNumber;
}

void PowerSwitchSensor::measure()
{
	// inverse Logik
	if (digitalRead(_pinNumber) == 0)
	{
		// Serial.println(F("*PS: PowerSwitchSensor low ==> return 1"));
		setMeasurement(1);
	}
	else
	{
		setMeasurement(0);
		// Serial.println(F("*PS: PowerSwitchSensor high ==> return 0"));
	}
}

````
