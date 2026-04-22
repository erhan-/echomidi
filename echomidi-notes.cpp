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

// Track the currently playing note
int currentNote = -1;

// Store previous distance for smoothing
int previousDistance = -1;

// Track how long we've been in the same note range (for hysteresis)
int noteStableCount = 0;
const int STABILITY_THRESHOLD = 2;  // Need 3 consecutive readings in same note range

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

  // Send MIDI note if distance is in valid range (2cm to 50cm)
  if (distance > 2 && distance < 50) {
    // Flash LED for valid distance reading
    digitalWrite(ledPin, HIGH);


    
    // Map distance (2-50cm) to MIDI note over one octave (36-48)
    int note = map(distance, 2, 50, 36, 48);
    
    // Hysteresis logic - only change note if stable for STABILITY_THRESHOLD readings
    if (note == currentNote) {
      // Same note, increase stability counter
      noteStableCount++;
    } else {
      // Different note detected - send Note Off for old note and reset counter
      if (currentNote != -1) {
        midi.sendNoteOff(currentNote, 0, 1);
      }
      noteStableCount = 1;
      currentNote = note;
    }
    
    // Only send Note On if we've been stable for the threshold period
    if (noteStableCount >= STABILITY_THRESHOLD) {
      // Send Note On for current note (only once per note change)
      if (noteStableCount == STABILITY_THRESHOLD) {
        midi.sendNoteOn(note, 100, 1);
      }
    }
  } else {
    // No object in range - turn off current note and LED
    if (currentNote != -1) {
      midi.sendNoteOff(currentNote, 0, 1);
      currentNote = -1;
    }
    digitalWrite(ledPin, LOW); // Turn LED off when no object
    noteStableCount = 0;
  }
  
  // Store previous distance for smoothing on next iteration
  previousDistance = distance;
  
  // Delay to control reading frequency
  midi.delay(50);
}

