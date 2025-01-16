/*
 *     USB MIDI to WS2812B by mat1jaczyyy & 4D
 *     ---------------------------------------
 */

/*  
 * LED Strip definition
 * --------------------
 */

const byte _NLED = 16;  // Update for 16 LEDs (4x4 grid)
const byte _DPIN = 2;

#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel _LED = Adafruit_NeoPixel(_NLED, _DPIN, NEO_GRB + NEO_KHZ800);

/*
 * Color Palette
 * Generate with retinaConverter.py (Retina 2.0+ Palette)
 * ------------------------------------------------------
 */

const byte _R[128] = {0, 65, 130, 255, 255, 255, 130, 65, 255, 255, 130, 65, 255, 255, 130, 65, 134, 81, 40, 20, 73, 0, 0, 0, 73, 0, 0, 0, 73, 0, 0, 0, 73, 0, 0, 0, 73, 0, 0, 0, 45, 0, ...};
const byte _G[128] = {0, 65, 130, 255, 61, 0, 0, 0, 186, 61, 32, 16, 174, 255, 130, 65, 255, 255, 130, 65, 255, 255, 130, 65, 255, 255, 130, 65, 255, 255, 130, 65, 255, 255, 130, 65, 194, 166, 85, ...};
const byte _B[128] = {0, 65, 130, 255, 61, 0, 0, 0, 105, 0, 0, 0, 45, 0, 0, 0, 49, 0, 0, 0, 73, 0, 0, 0, 93, 24, 12, 4, 89, 85, 45, 24, 182, 150, 73, 36, 255, 255, 130, 65, 255, 255, 130, 65, 255, ...};

// Brightness control (0 to 100)
byte brightness = 100;

/*
 * MIDI handler
 * ------------
 */

#include "MIDIUSB.h"

// Map MIDI notes to LED indices (4x4 grid)
const byte noteToLED[4][4] = {
  {48, 49, 50, 51},
  {44, 45, 46, 47},
  {40, 41, 42, 43},
  {36, 37, 38, 39}
};

bool update = false;

// Adjust color by brightness level (0-100)
byte applyBrightness(byte colorValue) {
  return (colorValue * brightness) / 100;
}

// Convert MIDI note to LED index using the noteToLED array
void noteOn(byte pitch, byte velocity) {
  // Find the LED index corresponding to the MIDI note
  byte ledIndex = 0;
  for (byte row = 0; row < 4; row++) {
    for (byte col = 0; col < 4; col++) {
      if (noteToLED[row][col] == pitch) {
        ledIndex = row * 4 + col;  // Calculate the 1D index from the 2D array
        break;
      }
    }
  }

  // Log the note and velocity for debugging
  if (velocity > 0) {
    Serial.print("Note On: ");
    Serial.print(pitch);
    Serial.print(", Velocity: ");
    Serial.println(velocity);
  } else {
    Serial.print("Note Off: ");
    Serial.println(pitch);
  }

  // Set the LED color with brightness adjustment
  _LED.setPixelColor(ledIndex, 
                     applyBrightness(_R[velocity]), 
                     applyBrightness(_G[velocity]), 
                     applyBrightness(_B[velocity]));
  update = true;
}

void setup() {
  Serial.begin(115200);  // Start serial communication at 115200 baud
  _LED.begin();
  _LED.show();
}

void loop() {
  midiEventPacket_t rx;
  do { 
    rx = MidiUSB.read();
    if (rx.header == 0x9) noteOn(rx.byte2, rx.byte3);  // Note on
    if (rx.header == 0x8) noteOn(rx.byte2, 0);         // Note off
  } while (rx.header == 0);

  if (update) {
    _LED.show();
    update = false;
  }
}
