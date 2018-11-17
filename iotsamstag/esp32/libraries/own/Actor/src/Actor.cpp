#include "Actor.h"
#include <ThingTime.h>
#include <Thing.h>
#include <MqttClient.h>

//#define Actor_DEBUG

/**
 * Das eingehende Topic wird geparst. Es enthält zuerst die Thingadresse,
 * dann den Actorname gefolgt vom konstanten Text command.
 * 		/wohnzimmer/light/command
 * Die Payload enthält dann die Werte (Standardmäßig ON/OFF)
 */
static void actorMqttCallback(const char *topic, const char *payload)
{
	// Aktor aus topic extrahieren
	int thingNameLength = strlen(Thing.getName());
	int topicLength = strlen(topic);
	int actorNameLength = topicLength-thingNameLength-9;
	char actorName[50];
	// /command am Ende (8 Zeichen) wird nicht mitkopiert
	Serial.print("AC* Topiclength: ");
	Serial.print(topicLength);
	Serial.print(", ThingNameLength: ");
	Serial.print(thingNameLength);
	Serial.print(", ActorNameLength: ");
	Serial.println(actorNameLength);

	strncpy(actorName, topic + thingNameLength + 1, actorNameLength);
	actorName[actorNameLength]=0;
	Serial.print(F("*AC: Actorname: '"));
	Serial.print(actorName);
	Serial.println("'");
	Actor *actorPtr = Thing.getActorByName(actorName);
	actorPtr->processMqttCommand(payload);
}

Actor::Actor(const char *thingName, const char *name)
{
	strcpy(_thingName, thingName);
	strcpy(_name, name);
	_time = ThingTime.getDateTime();
	strcpy(_actorTopic, Thing.getName());
	strcat(_actorTopic, "/");
	strcat(_actorTopic, getName());
	_actorSubscription.topic = _actorTopic;
	_actorSubscription.subscriberCallback = actorMqttCallback;
    Serial.print(F("*AC: Subscribe for Topic: "));
	Serial.println(_actorTopic);
	MqttClient.addSubscription(&_actorSubscription);
	_commandValue=0.0;
}

/**
 * Übersetzt den Text aus der Payload in den Wert (Float), mit dem der
 * Aktor gesetzt werden soll.
 * Defaultimplementierung: OpenHab-Schalter ON/OFF ==> 0/1
 */
float Actor::mapPayloadToCommandValue(const char *payload){
	if (strcmp("ON", payload) == 0)
	{
		return 1;
	}
	else if (strcmp("OFF", payload) == 0)
	{
		return  0;
	}
	Serial.print(F("!AC: Payload isn't a valid value: '"));
	Serial.print(payload);
	Serial.println("'");
	return -1;
}

/**
 * Aus der empfangenen Mqtt-Message ist der zu setzende
 * Wert des Aktors zu übernehmen und im Aktor zu setzen.
 * Beim nächsten Synchronisationsvorgang wird der
 * Aktor dann entsprechend angesteuert.
 */
void Actor::processMqttCommand(const char *payload)
{
	float value = mapPayloadToCommandValue(payload);
	if(value == -1) return;
	setCommandValue(value);
	// _jsonBuffer.clear();
	// JsonObject &jsonObject = _jsonBuffer.parseObject(payload);
	// if (jsonObject.success())
	// {
	// 	if (jsonObject.containsKey("settedValue"))
	// 	{
	// 		_settedValue = jsonObject["settedValue"];
	// 		Serial.print(F("*AC: Settedvalue: "));
	// 		Serial.println(_settedValue);
	// 	}
	// }
	// else
	// {
	// 	Serial.print(F("!AC: Payload isn't a valid json-object: '"));
	// 	Serial.print(payload);
	// 	Serial.println("'");
	// }
}

void Actor::setCommandValue(float value)
{
	_commandValue = value;
}

float Actor::getStateValue()
{
	return _stateValue;
}

float Actor::getCommandValue()
{
	return _commandValue;
}

char *Actor::getName()
{
	return _name;
}

void Actor::mapStateValueToPayload(char* payload, float state){
	if(state > 0.01){
		strcpy(payload, "OFF");
	}
	else{
		strcpy(payload, "ON");
	}
}


void Actor::syncState()
{
	// Serial.print(F("*AC: actual value: "));
	// Serial.print(_sensorPtr->getLastMeasurement());
	// Serial.print(", settedValue: ");
	// Serial.println(_settedValue);
	float actorState = readStateFromActor();
	if (abs(actorState - _commandValue) > 0.1)
	{
		// Serial.print(F("*AC: State: "));
		// Serial.print(actorState);
		// Serial.print(", CommandValue: ");
		// Serial.println(_commandValue);
		setActorTo(_commandValue);
		char fullTopic[100];
		sprintf(fullTopic, "%s/%s/state", Thing.getName(), _name);
		char payload[128];
		mapStateValueToPayload(payload, actorState);
		// _jsonBuffer.clear();
		// JsonObject &payload = _jsonBuffer.createObject();
		// payload["time"] = ThingTime.getDateTime();
		// payload["settedValue"] = _settedValue;
		// payload["actorState"] = actorState;
		// char jsonText[128];
		// payload.printTo(jsonText);
		Serial.print(F("*AC: Topic: "));
		Serial.print(fullTopic);
		Serial.print(", Payload: ");
		Serial.println(payload);
		MqttClient.publish(fullTopic, payload);
	}
}

//void Actor::changeActorTo(float value) {}
