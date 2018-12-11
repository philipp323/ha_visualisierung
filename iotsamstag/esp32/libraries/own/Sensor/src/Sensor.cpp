#include "Sensor.h"
#include <ThingTime.h>
// #include <Thing.h>
#include <MqttClient.h>
#include <ThingConfig.h>
#include <string.h>
#include <Logger.h>

//#define Sensor_DEBUG

Sensor::Sensor(const char *thingName, const char *name, const char *unit, float threshold)
{
	strcpy(_thingName, thingName);
	strcpy(_name, name);
	strcpy(_unit, unit);
	_threshold = threshold;
	_measurement = 0;
	_lastMeasurement = 0;
	_time = 0;
	Serial.printf("*SE: Sensor initialized: %s", name);
}

/**
 * Der konkrete Sensor meldet einen Messwert. 
 * Es wird überprüft, ob der Messwert gemeldet werden muss (Zeitablauf, Änderung)
 * und bei Bedarf per MQTT gemeldet.
 */
void Sensor::setMeasurement(float value)
{
	// Serial.print(F("*SE: Neuer Messwert fuer SR04T: "));
	// Serial.print(value);
	// Serial.print(", last measurement: ");
	// Serial.print(_measurement, 1);
	// Serial.print(", threshold:  ");
	// Serial.print(_threshold);
	_lastMeasurement = value;
	float delta = fabs(value - _measurement);
	// Serial.print(", delta:  ");
	// Serial.println(delta);
	long time = ThingTime.getDateTime();
	if (delta >= _threshold || time > _time + 900)
	{
		_measurement = value;
		_time = time;
		Serial.printf("*SE: Sensor::setMeasurement()\n*SE: Neuer Messwert fuer %s: %.1f %s, Time: %ld \n", _name, _measurement, _unit, _time);
		char fullTopic[100];
		//!sprintf(fullTopic, "%s/%s", Thing.getName(), _name);
		Serial.println(_thingName);
		sprintf(fullTopic, "%s/%s", _thingName, _name);
		char payload[128];
		getMqttPayload(payload, value);
		if(atoi(ThingConfig.getValue("isJsonEncoded")))
		{
			_jsonBuffer.clear();
			JsonObject& jsonObject = _jsonBuffer.createObject();
			jsonObject["time"] = ThingTime.getDateTime();
			jsonObject["value"] = value;
			memset(&payload[0], 0, sizeof(payload));  //! wozu?
			jsonObject.printTo(payload);
		}
		Serial.print(F("*SE: Topic: "));
		Serial.print(fullTopic);
		Serial.print(", Payload: ");
		Serial.println(payload);
		MqttClient.publish(fullTopic, payload);
		char message[50];
		sprintf(message, "Neuer Messwert fuer %s: %.1f %s, Time: %ld",_name, _measurement, _unit, _time);
		Logger.info("setMeasurement", message);
	}
}

float Sensor::getLastMeasurement()
{
	return _lastMeasurement;
}

char *Sensor::getName()
{
	return _name;
}

void Sensor::getMqttPayload(char *payload, float measurement)
{
	sprintf(payload, "%.2f", measurement);
}

//void Sensor::measure(){}
