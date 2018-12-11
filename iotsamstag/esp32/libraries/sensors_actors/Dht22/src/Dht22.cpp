/*
    Erfassung der Temperatur und Luftfeuchtigkeit ohne Blockade unter Verwendung
    eines Tickers und per GPIO-Interrupt zur PWM-Messung
*/

#include "Dht22.h"

//#define Dht22_DEBUG

// Zustände, die per Ticker.once() im richtigen Zeitabstand durchlaufen werden
#define INIT                        0
#define PREPARE_MEASUREMENT_LOW     1
#define START_MEASUREMENT           2
#define DO_READING                  3
#define COOLDOWN                    4

// Errorpositionen werden für den letzten aufgetretenen Fehler gespeichert
#define ERROR_INDEX       1
#define ERROR_BITLENGTH   2
#define ERROR_CHECKSUM    3

// Statischer Pointer auf die Instanz des Dht22-Objektes, den die indirekt
// per ISR aufgerufenen Funktionen verwenden, um auf private fields zuzugreifen.
// Sonst müssen alle Variablen, die in der ISR verwendet werden global definiert werden
static Dht22 *dht22Instance;

static IRAM_ATTR void processEventStatic(){
  dht22Instance->processEvent();
}

Dht22::Dht22(uint8_t pin)
{
  dht22Instance = this;
  _pin=pin;
  _state = INIT;
  pinMode(_pin, INPUT_PULLUP);
  digitalWrite( _pin, HIGH );
  _ticker.once_ms(10,processEventStatic);  // gleich nach der Instanzierung starten
}

/*
    Statische Methode, die als ISR verwendet wird.
    Ruft ihrerseits die Instanzmethode, die Zugriff auf die fields hat, auf
*/
static IRAM_ATTR void notifyChangingEdgeStatic(){
  dht22Instance->notifyChangingEdge();
}

/**
 * Eigentliche Interruptserviceroutine, die nur den Zeitpunkt des Auftretens einer
 * Flanke im Array dokumentiert.
 */
void IRAM_ATTR Dht22::notifyChangingEdge() {
  if(_changingEdgeIndex < TIMESTAMPS){
    _changingEdgeMicroSeconds[_changingEdgeIndex]=micros();
    _changingEdgeIndex++;
  }
}


/*
 * Der Messablauf des Sensors wird blockadefrei abgearbeitet, indem
 * jeweils mit "EinmalTicker" immer der nächste Zustand angesteuert
 * wird.
 * Die Erfassung der eigentlichen Messdaten erfolgt dann über einen
 * GPIO-Interrupt. Deren Auswertung im nicht zeitkritischen Ablauf
 * der FSM. 
 * Die Zeit bis zum nächsten Ereignis ist abgelaufen
 */
void IRAM_ATTR Dht22::processEvent()
{
  uint8_t humidityHigh=0;
  uint8_t humidityLow=0;
  uint8_t temperatureHigh=0;
  uint8_t temperatureLow=0;
  uint8_t checksum;
  uint8_t byteValue=0;
  int bitIndex = 0;
  int byteIndex = 0;
  unsigned long microSeconds; 
  bool isError=false;

  switch(_state)
  {
  case INIT:  // Neue Messung startet wieder
#ifdef Dht22_DEBUG
    Serial.println();
    Serial.println("INIT");
#endif    
    digitalWrite(_pin, HIGH);  // ist noch Eingang mit PullUp
    _changingEdgeIndex=0;
    for(int i=0; i<TIMESTAMPS;i++){  // Timestamps löschen
      _changingEdgeMicroSeconds[i]=0;
    }
    _state = PREPARE_MEASUREMENT_LOW;
    _ticker.once_ms(250,processEventStatic);
    break;

  case PREPARE_MEASUREMENT_LOW:  // Auf Ausgang und 20 ms auf LOW legen
#ifdef Dht22_DEBUG
    Serial.println("PREPARE_MEASUREMENT_LOW");
#endif    
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    _state = START_MEASUREMENT;
    _ticker.once_ms(20,processEventStatic);
    break;

  case START_MEASUREMENT:  // Startsignal im Mikrosekundenbereich geben und auf Ergebnis warten
#ifdef Dht22_DEBUG
    Serial.println("START_MEASUREMENT");
#endif    
    // End the start signal by setting data line high for 40 microseconds.
    digitalWrite(_pin, HIGH);
    delayMicroseconds(40);
    // Now start reading the data line to get the value from the DHT sensor.
    pinMode(_pin, INPUT_PULLUP);
    // GPIO-Interrupt scharf schalten
    attachInterrupt(digitalPinToInterrupt(_pin), notifyChangingEdgeStatic, CHANGE);
    delayMicroseconds(10);  // Delay a bit to let sensor pull data line low.
    _state = DO_READING;
    _ticker.once_ms(100,processEventStatic);  // in 100 ms ist der Einlesezyklus leicht abgeschlossen
    break;

  case DO_READING: // alle Daten wurden in der ISR gesammelt ==> aufbereiten
    detachInterrupt(digitalPinToInterrupt(_pin));
    if(_changingEdgeIndex < 82){    // zu wenige Daten im Hintergrund von ISR gesammelt
#ifdef Dht22_DEBUG
        Serial.print("!!! DO_READING too less bits, Index: ");
        Serial.println(_changingEdgeIndex);
#endif    
        _lastErrorPos = 1;
        _lastErrorCode = ERROR_INDEX;
        _lastErrorMilliSeconds = millis();
        isError=true;
    }
    else{   // genug Daten im Hintergrund von ISR gesammelt
#ifdef Dht22_DEBUG
      Serial.print("Index: ");
      Serial.println(_changingEdgeIndex);
      Serial.println("Timestamps");
  /*
      for(int i=0; i<TIMESTAMPS; i++){
        Serial.print(_changingEdgeMicroSeconds[i]);
        Serial.print(" ");
      }
      Serial.println();
  */
#endif    
      bitIndex=0;
      byteIndex=0;
      // Startbits überlesen
      for(int i=2; i<TIMESTAMPS && _changingEdgeMicroSeconds[i] != 0; i++){
        microSeconds=_changingEdgeMicroSeconds[i]-_changingEdgeMicroSeconds[i-1];
        if(i%2 != 0){  // nur die Dauer der HIGH-Pegel
          byteValue = byteValue << 1;
          if(microSeconds > 60 && microSeconds < 90){
            byteValue++;
          }
          else if (microSeconds <= 10 || microSeconds >= 40){
#ifdef Dht22_DEBUG
            Serial.println();
            Serial.print("!!! Illegal length of bit: ");
            Serial.print(i-2);
            Serial.print(", Duration: ");
            Serial.print(microSeconds);
            Serial.println(" ms");
#endif    
            _lastErrorPos = 2;
            _lastErrorCode = ERROR_BITLENGTH;
            _lastErrorMilliSeconds = millis();
            isError=true;
            break;  // Schleife beenden
          }
          bitIndex++;
          if(bitIndex >= 8){
            if(byteIndex == 0){
              humidityHigh = byteValue;
            }
            else if(byteIndex == 1){
              humidityLow = byteValue;
            }
            else if(byteIndex == 2){
              temperatureHigh = byteValue;
            }
            else if(byteIndex == 3){
              temperatureLow = byteValue;
            }
            else {
              checksum = byteValue;
              int sumOfBytes=humidityHigh+humidityLow+temperatureHigh+temperatureLow;
              if (checksum != (sumOfBytes % 256 )){
#ifdef Dht22_DEBUG
                Serial.println();
                Serial.print("!!! Checksum Error sumOfDgits: ");
                Serial.print(sumOfBytes % 256);
                Serial.print(" expected: ");
                Serial.println(checksum);
#endif    
                _lastErrorPos = 3;
                _lastErrorCode = ERROR_CHECKSUM;
                _lastErrorMilliSeconds = millis();
                isError=true;
                break;  // Schleife beenden
              }
            }
            byteValue=0;
            bitIndex=0;
            byteIndex++;
          }
        }
#ifdef Dht22_DEBUG
        Serial.print(microSeconds);
        if(i%2==0){
          Serial.print(";");
        }
        else{
          Serial.println();
        }
#endif    
      }
      if(!isError){
#ifdef Dht22_DEBUG
        Serial.println();
        Serial.print("Humidity HighByte: ");
        Serial.print(humidityHigh);
        Serial.print(", Humidity LowByte: ");
        Serial.print(humidityLow);
        Serial.print(", Temperature HighByte: ");
        Serial.print(temperatureHigh);
        Serial.print(", Temperature LowByte: ");
        Serial.print(temperatureLow);
        Serial.print(", CheckSum: ");
        Serial.println(checksum);
#endif    
        _humidity = (humidityHigh*256+humidityLow)/10.0;
        int sign = 1;   // negative Temperaturen richtig berechnen
        if(_temperature >= 256){  // highest bit gesetzt ==> sign negativ und bit löschen
          _temperature-=256;
          sign=-1;
        }
        _temperature = (temperatureHigh*256+temperatureLow)/10.0*sign;
#ifdef Dht22_DEBUG
        Serial.print("Humidity: ");
        Serial.print(_humidity);
        Serial.print(", Temperature: ");
        Serial.println(_temperature);
#endif    
        _lastCorrectMeasurementMilliSeconds=millis();
      }
    }
    _state = COOLDOWN;
    _ticker.once_ms(10,processEventStatic); 
    break;

  // Restliche Zeit des Messintervalls verstreichen lassen.
  case COOLDOWN:
#ifdef Dht22_DEBUG
    Serial.println("COOLDOWN");
#endif    
    _state = INIT;
    pinMode(_pin, INPUT_PULLUP);
    digitalWrite( _pin, HIGH );
    _ticker.once_ms(3000,processEventStatic);  // gleich nach der Instanzierung starten
    break;

  default:
    break;
  }
}

/*
    Wenn innerhalb der letzten Minute ein gültiger Messwert 
    ermittelt wurde, wird dieser zurückgegeben.
*/
float Dht22::getTemperature()
{
  if(millis() - _lastCorrectMeasurementMilliSeconds > 60000){ // letzte Minute kein gültiger Messwert
    return -100;
  }
  return _temperature;
}

/*
    Wenn innerhalb der letzten Minute ein gültiger Messwert 
    ermittelt wurde, wird dieser zurückgegeben.
*/
float Dht22::getHumidity()
{
  if(millis() - _lastCorrectMeasurementMilliSeconds > 60000){ // letzte Minute kein gültiger Messwert
    return -100;
  }
  return _humidity;
}

/*
    Code des zuletzt aufgetretenen Fehlers
*/
uint8_t Dht22::getLastErrorCode(){
  return _lastErrorCode;
}

/*
    Position des zuletzt aufgetretenen Fehlers
*/
uint8_t Dht22::getLastErrorPosition(){
  return _lastErrorPos;
}

/*
    Wann trat der letzte Fehler auf (Zeitabstand in Sekunden)
*/
long Dht22::getSecondsFromLastError(){
  if(_lastErrorMilliSeconds == 0){
    return -1;
  }
  return (millis() -_lastErrorMilliSeconds)/1000;
}

/*
    Wie alt (in Sekunden) ist der gelieferte Messwert.
*/
long Dht22::getSecondsFromLastCorrectMeasurement(){
  if(_lastCorrectMeasurementMilliSeconds == 0){
    return -1;
  }
  return (millis() -_lastCorrectMeasurementMilliSeconds)/1000;
}


