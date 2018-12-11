#include <Arduino.h>
#include <ThingTime.h>
#include <WiFiManager.h> 


void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster
  //ticker.attach(0.2, tick);
}



void handleNtpRequest(String choose) {
	String response = "";
	
		if(choose.equals("init")) {
			ThingTime.setNtpTimeSubscriber();
		} else if (choose.equals("get")) {
			Serial.print("Time: ");
			Serial.print(ThingTime.getDateTime());
			Serial.print(", String: ");
            char textTime [50];
			ThingTime.getTextDateTime(textTime);
			Serial.println(textTime);
		} else {
		response = "time-command no parameter found";
	}
	Serial.println("Time-Command, "+response);
	//WifiHttpServer.send( 200, "text/html", response);          //Returns the HTTP response
}

void setup() {
	Serial.begin(115200);                 //Initialisierung der seriellen Schnittstelle
	Serial.println();
	Serial.println();
	Serial.println("ThingTimeTest");
	Serial.println("============");
	Serial.println();
	WiFiManager wm;
	wm.setAPCallback(configModeCallback);
	if (!wm.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(1000);
  }


  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
 // ticker.detach();
  //keep LED on
 // digitalWrite(LED, LOW);
	handleNtpRequest("init");
	handleNtpRequest("get");

}
void loop() {
	handleNtpRequest("get"); 
	delay(10000);
}