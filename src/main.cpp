#include <Arduino.h>

bool isRelayActive(int value);

const int MONITOR_BAUD_RATE = 115200;
const int8_t ADC_PIN = 5;
const int8_t ADC_RESOLUTION = 12;
constexpr int16_t ADC_MAX_VALUE = (1 << ADC_RESOLUTION) - 1;
const int8_t RELAY_IN_PIN = 4;

void setup() {
    Serial.begin(MONITOR_BAUD_RATE);
    pinMode(ADC_PIN, INPUT);
    pinMode(RELAY_IN_PIN, OUTPUT);

    analogReadResolution(ADC_RESOLUTION);

}

void loop() {
  int rawValue = analogRead(ADC_PIN);
  if(isRelayActive(rawValue)) {
    digitalWrite(RELAY_IN_PIN, HIGH);
    Serial.println("Relay activated. RAW: " + String(rawValue));
  }
  else {
    digitalWrite(RELAY_IN_PIN, LOW);
    Serial.println("Relay deactivated. RAW: " + String(rawValue));
  }

  // digitalWrite(RELAY_IN_PIN, LOW);
  // delay(1000);
  // digitalWrite(RELAY_IN_PIN, HIGH);
  // delay(1000);
}

bool isRelayActive(int value) {
  static bool relayState = false; 

  int upperThreshold = ADC_MAX_VALUE / 2;
  int lowerThreshold = (ADC_MAX_VALUE - 300) / 2;

  if (value > upperThreshold) {
    relayState = true;
  } 
  else if (value < lowerThreshold) {
    relayState = false;
  }

  return relayState;
}
