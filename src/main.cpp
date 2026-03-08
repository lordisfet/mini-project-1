#include <Arduino.h>
#include <Ticker.h>

bool isRelayActive(int value);
void printRelayStatus(bool status, int value);

const int MONITOR_BAUD_RATE = 115200;
const int8_t ADC_PIN = 5;
const int8_t ADC_RESOLUTION = 12;
constexpr int16_t ADC_MAX_VALUE = (1 << ADC_RESOLUTION) - 1;
const int8_t RELAY_IN_PIN = 4;
const int8_t RELAY_STATUS_PRINT_DELAY = 1;
int rawValue;

Ticker printRelayStatusTicker;

void setup() {
    Serial.begin(MONITOR_BAUD_RATE);
    pinMode(ADC_PIN, INPUT);
    pinMode(RELAY_IN_PIN, OUTPUT);
    
    analogReadResolution(ADC_RESOLUTION);

    printRelayStatusTicker.attach(RELAY_STATUS_PRINT_DELAY, [] () {
      printRelayStatus(digitalRead(RELAY_IN_PIN), rawValue);
    });
}

void loop() {
  rawValue = analogRead(ADC_PIN);
  if(isRelayActive(rawValue)) {
    digitalWrite(RELAY_IN_PIN, LOW);
  }
  else {
    digitalWrite(RELAY_IN_PIN, HIGH);
  }
}

bool isRelayActive(int value) {
  static bool relayState = false; 

  int upperThreshold = 3 * ADC_MAX_VALUE / 4;
  int lowerThreshold = upperThreshold - 300;

  if (value > upperThreshold) {
    relayState = true;
  } 
  else if (value < lowerThreshold) {
    relayState = false;
  }

  return relayState;
}

void printRelayStatus(bool status, int value) {
  if (status) 
  {
    Serial.println("Relay " + String(status) + ". RAW: " + String(value));
  }
  else
  {
    Serial.println("Relay " + String(status) + ". RAW: " + String(value));
  }
}
