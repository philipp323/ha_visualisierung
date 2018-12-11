#include <Arduino.h>
#include <Dht22.h>
#include <MqttClient.h>
#include <HttpServer.h>
#include <ThingConfig.h>
#include <string>

#define LED_BUILTIN 2

//for LED status
#include <Ticker.h>
Ticker ticker;
Dht22 dht22(5);
int LED = LED_BUILTIN;


void handleRootTest() {
	
	String message = "Number of args received:";
	message += HttpServer.args();           //Get number of parameters
	message += "\n";                            //Add a new line
	for (int i = 0; i < HttpServer.args(); i++) {
		message += "Arg " + (String)i +  ": ";   //Include the current iteration value
		message += HttpServer.argName(i) + ": ";     //Get the name of the parameter
		message += HttpServer.arg(i) + "\n";              //Get the value of the parameter
	} 
	HttpServer.send( 200, "text/html",message);       //Response to the HTTP request
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  HttpServer.init();
  HttpServer.on("/", handleRootTest);
  MqttClient.init("test");
  MqttClient.subscribeToBroker();
  //keep LED on
  digitalWrite(LED, LOW);
}

unsigned long lastReportingTime=0;
void loop() {
    delay(1);
    HttpServer.handleClient();
	if(millis()-lastReportingTime > 5000){
		Serial.println();
		Serial.println("----------------------------------------------------------------------------");
		Serial.print("Humidity: ");
    char humidity [50];
		char temperature [50];
    sprintf(humidity, "%f", dht22.getHumidity());
		sprintf(temperature, "%f2", dht22.getTemperature());
    MqttClient.publish("Humidity", humidity);
		Serial.print(dht22.getHumidity());
		Serial.print(", Temperature: ");
		Serial.print(dht22.getTemperature());
		Serial.print(", Age of measurement: ");
		Serial.print(dht22.getSecondsFromLastCorrectMeasurement());
		Serial.println(" sec");
		Serial.print("Errors, last pos: ");
		Serial.print(dht22.getLastErrorPosition());
		Serial.print(", last errorcode: ");
		Serial.print(dht22.getLastErrorCode());
		Serial.print(", Age of last error: ");
		Serial.print(dht22.getSecondsFromLastError());
		Serial.println(" sec");
		Serial.println("----------------------------------------------------------------------------");
		lastReportingTime=millis();
	}
}
