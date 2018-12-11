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
#include <UdpLoggerTarget.h>
#include <Logger.h>
#include <Thing.h>
#include <tcpip_adapter.h> 

void setup() {
	Serial.begin(115200);                 //Initialisierung der seriellen Schnittstelle
	Serial.println();
	Serial.println();
	Serial.println(F("*UTT Test fuer UDP-Loggertarget"));
	Serial.println(F("*UTT =========================="));
	ThingConfig.readConfig();			// Einlesen der Konfiguration in den JSON-Cache
	InternLed.init(LED_BUILTIN_PIN);
	HttpServer.init();					// HttpServer initialisieren
	ThingTime.setNtpTimeSubscriber();   // Zeit über Internet synchronisieren	
	const char* thingName = ThingConfig.getValue("thingname");
	Thing.init(thingName, true);		// Thing initialisieren	mit JSON als Messageformat     
	
	// >>>>>>>>>>>>>>>>>>>>>>  Thingspezifischer Teil
	//<<<<<<<<<<<<<<<<<<<<<<< Ende Thingspezifischer Teil

 	const char* udpTargetIpText = ThingConfig.getValue("udploggerip");
 	const char* udpPortText = ThingConfig.getValue("udploggerport");
	UdpLoggerTarget* udpLoggerTarget=nullptr;
	IPAddress udpLoggerIpAddress;
	if( strlen(udpTargetIpText) == 0 || strlen(udpPortText) == 0){
  		Serial.println("UTT No Ip or no port");
	    udpLoggerTarget = new UdpLoggerTarget("udplogger", 0);
	}
	else{
		udpLoggerIpAddress.fromString(udpTargetIpText);
  		Serial.printf("UTT Ip: %s, Port: %s", udpTargetIpText, udpPortText);
		udpLoggerTarget = new UdpLoggerTarget("udplogger", 0, udpLoggerIpAddress, atoi(udpPortText));
	}
    Logger.addLoggerTarget(udpLoggerTarget);
	MqttClient.subscribeToBroker();
}

long lastUdpSentTime = millis();

void sendUdpLogMessages(){
  	LoggerTarget* udpLoggerTarget = Logger.getLoggerTarget("udplogger");
	int logLevel = udpLoggerTarget->getLogLevel();
  	Serial.printf("Actual minimum Loglevel set: %s\n", Logger.getLogLevelText(logLevel));
	Logger.info("Main-Loop", "Info");
	Logger.debug("Main-Loop", "Debug");
	Logger.exception("Main-Loop", "Exception");
	Logger.error("Main-Loop", "Error");
	Logger.fatalerror("Main-Loop", "FatalError");
	if(logLevel == LOG_LEVEL_NOLOG){
		logLevel = LOG_LEVEL_INFO;
		udpLoggerTarget->setLogLevel(logLevel);
		Serial.println("-----------------------------------------------------------------------");
	} else {
		logLevel++;
		udpLoggerTarget->setLogLevel(logLevel);
		Serial.println();
	}
}

void loop() {
	HttpServer.handleClient();
	MqttClient.doLoop();					// Mqtt-Schnittstelle bedienen
	Thing.refreshSensorsAndActors();
	if( millis() - lastUdpSentTime > 1000){
		sendUdpLogMessages();
		lastUdpSentTime = millis();
	}
	delay(1);
}