#include <Arduino.h>
#include <DigiMIDI.h>

// DigisparkMIDI uses DigiMIDIDevice as the class name
DigiMIDIDevice midi;

// HC-SR04 ultrasonic sensor pins (Digispark pin numbers)
// P0 (pin 0) for TRIG
// P2 (pin 2) for ECHO
const int trigPin = 0;
const int echoPin = 2;

// Onboard LED pin (Digispark pin 1)
const int ledPin = 1;

// Control Change (CC) number to send - this represents the "knob" in Ableton
// CC 74 is commonly used for filter cutoff, but you can choose any value 0-127
const int CC_NUMBER = 24;

// Store previous CC value for change detection
int previousCCValue = -1;

// Minimum change threshold to avoid sending duplicate values
const int MIN_CHANGE_THRESHOLD = 1;  // Only send if value changes by at least this much

// Track how long we've been in the same CC value range (for smoothing)
int valueStableCount = 0;
const int STABILITY_THRESHOLD = 1;  // Need 2 consecutive readings in same value range

void setup() {
  // Set up HC-SR04 pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // Set up LED pin
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Start with LED off
}

void loop() {
  // Keep USB connection alive - must be called regularly
  midi.update();

  // Trigger HC-SR04 measurement
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echo pulse duration
  long duration = pulseIn(echoPin, HIGH, 30000);
  
  // Calculate distance in cm (duration / 2 / 29.1)
  int distance = (duration / 2) / 29.1;

  // Send Control Change if distance is in valid range (2cm to 30cm)
  if (distance > 2 && distance < 30) {
    // Flash LED for valid distance reading
    digitalWrite(ledPin, HIGH);

    // Map distance (2-50cm) to CC value (0-127)
    // This creates a continuous control range for the "knob"
    int ccValue = map(distance, 2, 50, 127, 0);  // Inverted: closer = higher value
    
    // Check if value changed significantly enough to send
    int valueDiff = abs(ccValue - previousCCValue);
    
    if (valueDiff >= MIN_CHANGE_THRESHOLD) {
      // Send CC message immediately for faster response
      midi.sendControlChange(CC_NUMBER, ccValue, 1);
      previousCCValue = ccValue;
    }
  } else {
    // No object in range - do NOT reset CC value
    // This keeps the parameter at its last position when hand is moved away
    digitalWrite(ledPin, LOW); // Turn LED off when no object
  }
  
  // Minimal delay - just enough for USB polling
  // Reduced from 50ms to 10ms for faster response
  delay(10);
}

