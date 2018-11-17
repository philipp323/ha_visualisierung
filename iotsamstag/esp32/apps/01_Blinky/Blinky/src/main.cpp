#include <Arduino.h>
#define LED_BUILTIN 21 // 2 normale TTGO, 21 TTGO T8

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  Serial.println("Blinky");
  Serial.println("=======");
  delay(200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);
  delay(700);
  digitalWrite(LED_BUILTIN, LOW);
  delay(700);
  Serial.println("LOOP");
}