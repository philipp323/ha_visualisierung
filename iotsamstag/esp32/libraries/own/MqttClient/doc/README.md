# Library MqttClient

## Ziele

Der ESP soll sich als Mqtt-Client bei einem Broker registrieren können und das möglichst einfach. Mqtt ist ein einfaches Protokoll, mit dem der Mikrocontroller (Mess-) Werte an
eine Zentrale (Broker) schicken, und solche Werte auch empfangen kann. Dies implementiert
zudem das Publish-Subscribe-Pattern.

Diese Library bildet einen Wrapper um die ["PubSubClient"-Library von Nick O'Leary](https://github.com/knolleary/pubsubclient)

## Hardware

Es wird lediglich ein ESP32 benötigt.

## Software

### Beispielaufrufe

```c
static void myMethodName(const char *topic, const char *payload){...}
char test[40];
MqttSubscription exampleSubscription;

void setup(){
	MqttClient.init(NodeConfig.getMqttNodeName());
	strcpy(test, "example");
	exampleSubscription.topic = test;
	exampleSubscription.callback = myMethodName;

	MqttClient.addSubscription(&exampleSubscription);
	MqttClient.subscribeToBroker();
	...
}

void loop(){
	MqttClient.doLoop();
	...
	MqttClient.publish("MY TOPIC NAME", data);
	...
}
```

### Erklärung

* ```init(...)``` Muss als erstes aufgerufen werden. Darin wird der Mqtt-Client mit dem Broker verbunden.
* ```MqttSubscription``` bildet die Datenkapselung für eine einzelne Subscription. Immer wenn beim Broker ein Publish unter dieser Topic eingeht, wird mit den Daten das angegebene Callback ausgeführt
  * ```topic``` spezifiziert die Topic für die diese Subscription zählt.
  * ```callback``` spezifiziert die Methode die aufgerufen wird.
* ```addSubscription(...)``` damit wird diese Subscription lokal registriert, der Broker weiß aber noch nichts davon
* ```subscribeToBroker()``` damit werden alle eingetragenen Subscriptions an den Broker gesendet und damit fertiggestellt.
* ```doLoop()``` dies muss periodisch aufgerufen werden, am besten im Loop. Diese Funktion bedient die Mqtt-Schnittstelle. Die Funktion sollte oft aufgerufen werden.
* ```publish(...)``` wenn man eigene Daten an den Broker senden will, kann man dies mit dieser Funktion machen.

### Library

#### MqttClient

```MqttClient``` im ```MqttClient.h``` bildet das Singleton über welches alle Mqtt-Anforderungen erfüllt werden können.

##### Methoden

| ```MqttClient::``` | Erklärung |
|-|-|
|```void init(const char *mainTopic)```| Setzt alle Configs des MqttClients und versucht sich dann mit dem Mqtt-Broker zu verbinden.  Die Adresse des Brokers wird aus  ```NodeConfig.getMqttServerAddress()``` : ```NodeConfig.getMqttPort()``` ausgelesen. |
|```void addSubscription(MqttSubscription* subscriptionPtr)``` | Registriert die [Subscription](#mqttsubscription) lokal, damit sie beim nächsten reconnect an den Broker gesendet werden kann.  |
|```void publish(char *topic, char *payLoad)``` | Schickt die angegebenen Daten `payload` unter der angegebenen `topic` an den Broker, wenn der Client verbunden ist. |
|```void doLoop()``` | Kümmert sich um die Netzwerk-Arbeiten. Hier wird die gleichnamige Methode der PubSubClient-Library aufgerufen. Falls die Verbindung unterbrochen wurde, wird ein reconnect versucht. |
|```void notifySubscribers(char* topic, char *payload)``` | Simuliert eine gepublishte Nachricht, und ruft die Callbacks aller Subscriber auf. Dies ist nur lokal und wird nicht zum Broker weitergeschickt. Diese Methode unterstützt zum Stand 24.02.18 noch keine Wildcards |
|```boolean subscribeToBroker()``` | Sendet alle lokal registrierten Subscriptions an den Broker. |

#### MqttSubscription <a name="mqttsubscription"></a>
Dieses struct stellt eine einzige Subscription dar. Sie beinhaltet:
|Feld|Erklärung|
|-|-|
|`char* topic`| Ein String in dem die Topic gespeichert ist, für die diese Subscription gilt.|
|`SubscriberCallback subscriberCallback`| Dies ist eine Referenz auf die Callback-Funktion mit spezieller [Signatur](#callbackSignature) die aufgerufen wird, sobald diese Subscription einen Publish erhält.

#### SubscriberCallback <a name="callbackSignature"></a>
Stellt einen Funktionspointer dar, welcher folgende Signatur aufweist:

```c 
void(const char* topic, const char* payload)
```

