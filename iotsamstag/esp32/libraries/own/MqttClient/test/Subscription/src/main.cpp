#include <Arduino.h>
#include <MqttClient.h>
#include <HttpServer.h>

struct MqttSubscription exampleSubsrition;
char example[50];

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

static void Print(const char *topic, const char *payload){
	Serial.println("Working!!");
	Serial.println(topic);
	Serial.println(payload);
}

void setup() {
  // put your setup code here, to run once:
	Serial.begin(115200);
  HttpServer.init();
  HttpServer.on("/", handleRootTest);
  strcpy(example, "htlleonding");
	MqttClient.init("test");
  exampleSubsrition.topic = example;
	exampleSubsrition.subscriberCallback = Print;
  MqttClient.addSubscription(&exampleSubsrition);
  MqttClient.subscribeToBroker();
}

void loop() {
    // put your main code here, to run repeatedly:
		delay(500);
    HttpServer.handleClient();
		MqttClient.doLoop();
}
