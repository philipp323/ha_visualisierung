#define WEMOS

#ifdef WEMOS
	#define LED_BUILTIN_PIN 2  // WEMOS MINI32
#else 
	#ifdef TTGO
		#define LED_BUILTIN_PIN 16  // TTGO
	#endif
#endif	

#include <Arduino.h>
#include <InternLed.h>
#include <HttpServer.h>
#include <ThingTime.h>
#include <ThingConfig.h>
#include <MqttClient.h>
#include <Dht22.h>

Dht22 dht22(15);

void handleGetTemperature(){
	char response[500];
	sprintf(response, "Temperatur: %.2f", dht22.getTemperature());
	HttpServer.send(200, "text/html", response);
}

void handleGetHumidity(){
	char response[500];
	sprintf(response, "Humidity: %.2f", dht22.getHumidity());
	HttpServer.send(200, "text/html", response);
}

void handleGetEverything(){
	char response[500];
	sprintf(response,"Temperatur: %.2f°C \n Luftfeuchtigkeit: %.2f%%",dht22.getTemperature(),dht22.getHumidity());
	//line feed nicht in HTML angeben da ansonsten nur der direkte Link richtig dargestellt wird aber nicht im PopUp!
	// %% um Prozent auszugeben!
	HttpServer.send(200,"text/html",response);
}

void setup() {
	Serial.begin(115200);                 //Initialisierung der seriellen Schnittstelle
	Serial.println();
	Serial.println();
	Serial.println(F("*BA BaseApp"));
	Serial.println(F("*BA ======="));
	ThingConfig.readConfig();			// Einlesen der Konfiguration in den JSON-Cache
	InternLed.init(LED_BUILTIN_PIN);
	HttpServer.init();					// HttpServer initialisieren
	ThingTime.setNtpTimeSubscriber();   // Zeit über Internet synchronisieren	
	MqttClient.subscribeToBroker();
	HttpServer.on("/gettemperature", handleGetTemperature);
	HttpServer.on("/gethumidity", handleGetHumidity);
	HttpServer.on("/get",handleGetEverything);
}

void loop() {
	HttpServer.handleClient();
	MqttClient.doLoop();					// Mqtt-Schnittstelle bedienen
	delay(1);
}