/*
 *     USB MIDI to WS2812B by mat1jaczyyy & 4D
 *     ---------------------------------------
 */

/*  
 * LED Strip definition
 * --------------------
 */

const byte _NLED = 64;  // Update for 64 LEDs
const byte _DPIN = 2;

#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel _LED = Adafruit_NeoPixel(_NLED, _DPIN, NEO_GRB + NEO_KHZ800);

/*
 * Color Palette
 * Generate with retinaConverter.py (Retina 2.0+ Palette)
 * ------------------------------------------------------
 */

const byte _R[128] = {0, 65, 130, 255, 255, 255, 130, 65, 255, 255, 130, 65, 255, 255, 130, 65, 134, 81, 40, 20, 73, 0, 0, 0, 73, 0, 0, 0, 73, 0, 0, 0, 73, 0, 0, 0, 73, 0, 0, 0, 73, 0, 0, 0, 45, 0, 0, 0, 105, 45, 24, 12, 255, 255, 130, 65, 255, 255, 130, 65, 255, 150, 117, 32, 0, 0, 0, 0, 0, 16, 125, 28, 255, 186, 174, 97, 12, 0, 0, 0, 24, 89, 174, 40, 255, 134, 113, 0, 57, 85, 53, 89, 49, 105, 210, 255, 255, 182, 142, 130, 57, 0, 12, 20, 20, 101, 130, 219, 215, 255, 158, 101, 20, 219, 125, 154, 142, 61, 113, 223, 158, 53, 24, 4, 182, 61, 178, 73};
const byte _G[128] = {0, 65, 130, 255, 61, 0, 0, 0, 186, 61, 32, 16, 174, 255, 130, 65, 255, 255, 130, 65, 255, 255, 130, 65, 255, 255, 130, 65, 255, 255, 130, 65, 255, 255, 130, 65, 194, 166, 85, 45, 134, 85, 45, 24, 36, 0, 0, 0, 53, 0, 0, 0, 61, 0, 0, 0, 65, 0, 0, 0, 12, 53, 81, 53, 57, 73, 20, 0, 69, 0, 125, 28, 0, 255, 235, 255, 138, 255, 166, 40, 0, 0, 24, 16, 49, 223, 255, 255, 255, 255, 255, 138, 81, 81, 28, 0, 69, 166, 255, 89, 40, 73, 77, 20, 28, 57, 0, 65, 73, 190, 223, 178, 20, 210, 235, 150, 101, 61, 113, 255, 0, 0, 206, 65, 174, 49, 81, 20};
const byte _B[128] = {0, 65, 130, 255, 61, 0, 0, 0, 105, 0, 0, 0, 45, 0, 0, 0, 49, 0, 0, 0, 73, 0, 0, 0, 93, 24, 12, 4, 89, 85, 45, 24, 182, 150, 73, 36, 255, 255, 130, 65, 255, 255, 130, 65, 255, 255, 130, 65, 251, 255, 130, 65, 255, 255, 130, 65, 109, 81, 40, 20, 0, 0, 0, 4, 0, 24, 109, 255, 77, 202, 125, 28, 0, 45, 4, 8, 0, 93, 255, 255, 255, 255, 121, 0, 0, 4, 20, 0, 36, 109, 202, 255, 194, 231, 255, 89, 0, 0, 0, 4, 0, 12, 32, 40, 89, 24, 0, 40, 16, 36, 45, 12, 45, 105, 138, 255, 255, 61, 113, 255, 0, 0, 0, 0, 0, 0, 0, 0};

// Brightness control (0 to 100)
byte brightness = 100;

/*
 * MIDI handler
 * ------------
 */

#include "MIDIUSB.h"

// Map MIDI notes to LED indices (8x8 grid)
const byte noteToLED[8][8] = {
  {64, 65, 66, 67, 96, 97, 98, 99},
  {60, 61, 62, 63, 92, 93, 94, 95},
  {56, 57, 58, 59, 88, 89, 90, 91},
  {52, 53, 54, 55, 84, 85, 86, 87},
  {48, 49, 50, 51, 80, 81, 82, 83},
  {44, 45, 46, 47, 76, 77, 78, 79},
  {40, 41, 42, 43, 72, 73, 74, 75},
  {36, 37, 38, 39, 68, 69, 70, 71}
};

bool update = false;

// Adjust color by brightness level (0-100)
byte applyBrightness(byte colorValue) {
  return (colorValue * brightness) / 100;
}

// Convert MIDI note to LED index using the noteToLED array
void note(byte pitch, byte velocity) {
  // Find the LED index corresponding to the MIDI note
  byte ledIndex = 0;
  for (byte row = 0; row < 8; row++) {
    for (byte col = 0; col < 8; col++) {
      if (noteToLED[row][col] == pitch) {
        ledIndex = row * 8 + col;  // Calculate the 1D index from the 2D array
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
    if (rx.header == 0x9) note(rx.byte2, rx.byte3);  // Note on
    if (rx.header == 0x8) note(rx.byte2, 0);         // Note off
  } while (rx.header == 0);

  if (update) {
    _LED.show();
    update = false;
  }
}
