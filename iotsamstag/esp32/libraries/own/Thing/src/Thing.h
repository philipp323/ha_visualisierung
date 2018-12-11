#pragma once

#include <list>
#include <Actor.h>
#include <Sensor.h>


/*
	Der Thing (Singleton) verwaltet seine Sensoren und Actoren.
	In der Hauptschleife werden die Sensor und Aktorwerte der Devices 
	zyklisch (< 1ms) aktualisiert.
*/
class ThingClass
{
public:

	/*
		Thing initialisieren (Sensorliste und Actorliste anlegen)
		Callback für Aktualisierung anlegen
	*/
	void init(const char* name, bool json);
	char* getName();

	/*
		Die Sensoren und Aktoren der Devices werden aktualisiert.
		Dabei werden gesetzte und aktuelle Werte synchronisiert und die
		aktuellen Werte neu gelesen.
	*/
	void refreshSensorsAndActors();
	void addSensor(Sensor* sensor);
	void addActor(Actor* actor);
	Actor* getActorByName(char* name);
	Sensor* getSensorByName(char* name);

private:
	// StaticJsonBuffer<200> _jsonBuffer;

	std::list<Sensor*> _sensors;
	std::list<Actor*> _actors;
	char _thingName[80];

	/*
		Callbackroutine mit der sich der Thing beim CommandInterpreter für Befehle
		zum Setzen von Aktoren registriert
	*/
	void registerThingConfigCommandCallback();


};

extern ThingClass Thing;  // Singleton
