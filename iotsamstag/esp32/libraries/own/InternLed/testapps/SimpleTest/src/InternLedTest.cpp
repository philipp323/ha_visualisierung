#include <InternLed.h>

#define WEMOS

#ifdef WEMOS
	#define LED_BUILTIN_PIN 2  // WEMOS MINI32
#else 
	#ifdef TTGO
		#define LED_BUILTIN_PIN 16  // TTGO
	#endif
#endif	

void setup() {
    Serial.begin(115200);                 //Initialisierung der seriellen Schnittstelle
	Serial.println();
	Serial.println();
	Serial.println("LedBlinkingTest");
	Serial.println("===============");
	InternLed.init(LED_BUILTIN_PIN);
}

void loop() {
	Serial.println("Schnelles blinken");
	InternLed.blinkFast();
	delay(4000);
	Serial.println("Langsames blinken");
	InternLed.blinkOff();
	InternLed.blinkSlow();
	delay(4000);
	Serial.println("Stop blinken");
	InternLed.blinkOff();
	delay(4000);
	Serial.println("Zehn mal schnell blinken");
	InternLed.blinkFast(10);
	delay(8000);
	Serial.println("Fünf mal langsam blinken");
	InternLed.blinkFast(5);
	delay(8000);
}