#include "Thing.h"
#include <MqttClient.h>
#include <ThingConfig.h>
#include <HttpServer.h>

/**
 * Aktorwert kann per Get-Request gesetzt werden. 
 * Implementiert von Ferdinand Hell (Htl Wels)
 */
void handleSetActorRequest()
{
	Serial.println(F("*HS: handleSetActorRequest()"));
	String actorName = HttpServer.arg("actor");
	String actorValue = HttpServer.arg("value");
	if (actorName != NULL && actorValue != NULL)
	{
		Actor *actor = Thing.getActorByName((char *)(actorName.c_str()));
		char response[50];
		if (actor == NULL)
		{
			sprintf(response, "Actor %s not found!", actorName.c_str());
			HttpServer.send(200, "text/html", response); //Returns the HTTP response
		}
		else
		{
			actor->setCommandValue(atof(actorValue.c_str()));
			sprintf(response, "Changed Actor's %s value %.2f", actorName.c_str(), atof(actorValue.c_str()));
			Serial.print("*HS: ");
			Serial.println(response);
			HttpServer.send(200, "text/html", response); //Returns the HTTP response
		}
	}
	else if (actorName == NULL)
	{
		HttpServer.send(200, "text/html", "Parametername actor fehlt"); //Returns the HTTP response
	}
	else
	{
		HttpServer.send(200, "text/html", "Parametername value fehlt"); //Returns the HTTP response
	}
}

/**
 * Sensorwert per Http-Get-Request abfragen
 */
void handleGetSensorRequest()
{
	Serial.println(F("*HS: handleGetSensorRequest()"));
	String sensorName = HttpServer.arg("sensor");
	if (sensorName != NULL)
	{
		Sensor *sensor = Thing.getSensorByName((char *)(sensorName.c_str()));
		char response[50];
		if (sensor == NULL)
		{
			sprintf(response, "Sensor %s not found!", sensorName.c_str());
			HttpServer.send(200, "text/html", response); //Returns the HTTP response
		}
		else
		{
			float measurement = sensor->getLastMeasurement();
			sprintf(response, "Sensor's %s value %.2f", sensorName.c_str(), measurement);
			Serial.print("*HS: ");
			Serial.println(response);
			HttpServer.send(200, "text/html", response); //Returns the HTTP response
		}
	}
	else 
	{
		HttpServer.send(200, "text/html", "Parametername sensor fehlt"); //Returns the HTTP response
	}
}


/*
 * Thing wird initialisiert und Routen zum Lesen von Sensorwerten, bzw. Setzen
 * von Aktorwerten werden registriert.
 */
void ThingClass::init(const char *name, bool json)
{
	strcpy(_thingName, name);
	const char *jsonChar = json ? "1" : "0";
	ThingConfig.setValue("isjsonencoded", jsonChar);   //! Configkeys in Kleinbuchstaben ==> keine Verwechslungen
	MqttClient.init(_thingName);
	Serial.print(F("TH* Thing init with name: "));
	Serial.println(_thingName);
	HttpServer.on("/setactor", handleSetActorRequest);
	HttpServer.on("/getsensor", handleGetSensorRequest);

}

char *ThingClass::getName()
{
	return _thingName;
}

void ThingClass::addSensor(Sensor *sensorPtr)
{
	_sensors.push_back(sensorPtr);
}

void ThingClass::addActor(Actor *actorPtr)
{
	_actors.push_back(actorPtr);
}

void ThingClass::refreshSensorsAndActors()
{
	for (std::list<Sensor *>::iterator it = _sensors.begin(); it != _sensors.end(); ++it)
	{
		// Serial.print(F("*NO: refresh, Sensor: "));
		// Serial.print(it->first);
		// Serial.print(F(", last Value: "));
		// Serial.println(it->second->getLastMeasurement());
		Sensor *sensorPtr = *it;
		sensorPtr->measure();
	}
	for (std::list<Actor *>::iterator it = _actors.begin(); it != _actors.end(); ++it)
	{
		// Serial.print(F("*NO: refresh, Actor: "));
		// Serial.print(it->first);
		// Serial.print(F(", last Value: "));
		// Serial.println(it->second->getLastMeasurement());
		(*it)->syncState();
	}
}

Actor *ThingClass::getActorByName(char *name)
{
	for (std::list<Actor *>::iterator it = _actors.begin(); it != _actors.end(); ++it)
	{
		// Serial.print(F("*NO: refresh, Sensor: "));
		// Serial.print(it->first);
		// Serial.print(F(", last Value: "));
		// Serial.println(it->second->getLastMeasurement());
		Actor *actorPtr = *it;
		if (strcmp(actorPtr->getName(), name) == 0)
		{
			return actorPtr;
		}
	}
	return nullptr;
}

Sensor *ThingClass::getSensorByName(char *name)
{
	for (std::list<Sensor *>::iterator it = _sensors.begin(); it != _sensors.end(); ++it)
	{
		Sensor *sensorPtr = *it;
		if (strcmp(sensorPtr->getName(), name) == 0)
		{
			return sensorPtr;
		}
	}
	return nullptr;
}

ThingClass Thing;
