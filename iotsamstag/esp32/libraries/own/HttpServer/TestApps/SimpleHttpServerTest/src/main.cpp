#include <HttpServer.h>


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
	Serial.begin(115200);                 //Initialisierung der seriellen Schnittstelle
	Serial.println();
	Serial.println();
	Serial.println("HttpServerTest");
	Serial.println("==============");
	Serial.println("Webserver als Echoserver starten");
	HttpServer.init();
	HttpServer.on("/", handleRootTest);
}

void loop() {
	HttpServer.handleClient();
	delay(1);
}
