// MqttClient.h
#pragma once

#define MQTT_SOCKET_TIMEOUT 5  // überschreibt das Timeout der PubSubClient-Library (15 Sekunden)

#include <WiFi.h>
#include <PubSubClient.h>
#include <list>

#define TOPIC_LENGTH 200
#define PAYLOAD_LENGTH 200

/*
	Definition des Typs des Functionpointers als Callback
	//! Sollte auch Methoden einer Klasse unterstützen (Readme.md updaten)
*/
typedef void(*SubscriberCallback)(const char *topic, const char *payload);

/*
	Zentrale Datenstruktur bei der Kommandobearbeitung
*/
struct MqttSubscription
{
	char *topic;
	SubscriberCallback subscriberCallback;
};

class MqttClientClass
{
  public:
	void init(const char *mainTopic);
	void addSubscription(MqttSubscription* subscriptionPtr);
	void publish(char *topic, char *payLoad);
	void doLoop();
	void notifySubscribers(char* topic, char *payload);
	boolean subscribeToBroker();
	
  private:
	char _mainTopic[50];
	std::list<MqttSubscription *> _mqttSubscriptions;
	WiFiClient _wifiClient;
	boolean reconnect();
	long _lastReconnectAttempt = 0;
	PubSubClient _mqttClient;
	char _mqttServer[50];
	char _mqttUserName[25];
	char _mqttPassword[25];
	char _mqttThingName[50];
	int _mqttPort; 
};
extern MqttClientClass MqttClient;
