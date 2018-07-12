# LED Strip MIDI for Teensy - controlling LED strips via MIDI

This Arduino project allows for controlling a LED strip from a DAW program (via MIDI) over a Teensy microcontroller.

## System Requirements

* LED Strip
	* The **LPD8806** and **WS2812B** strips are implemented in separate Arduino projects. 
* USB Development Board
    * A **32-bit PJRC Teensy** is highly recommended. A [Teensy-LC](https://www.pjrc.com/store/teensylc.html) was used while developing this project and is officially supported alongside other Teensy 3 models.
    * Teensy 2 (8-bit) and certain Arduino boards might work, but have not been tested and are not officially supported.
* [Arduino Software](https://www.arduino.cc/en/Main/Software)
    * Developed with **version 1.8.5**, but older versions will likely work.
    * Also requires [Teensyduino](https://www.pjrc.com/teensy/teensyduino.html).
* OS
    * **Windows, Linux and macOS** are officially supported.
* DAW Software
    * [Ableton Live](https://www.ableton.com/en/live/) is recommended due to its flexibility in creating MIDI Effects.

## Hardware

Connect your board with the LED strip using the [schematic](https://github.com/mat1jaczyyy/teensy-underlights/blob/master/LPD8806.svg). You can attach the strip to any object, I used a short wooden cylinder which I wrapped the strip around and it works well.

## Software

Modify the Arduino script according to your LED strip configuration (`_nLED` = Number of LEDs < 128, `_dLED` = Data pin, `_cLED` = Clock pin) and compile.

## Usage

Sending standard MIDI note-offs and note-ons on any channel will trigger the LED with the match color. Sending a SysEx message in the format `F0 PP RR GG BB F7` (where `PP` is the LED index, `RR` is the red component of the color, `GG` is the greeb component of the color, and `BB` is the blue component of the color) will directly set the LED to a desired color.

## Retina Palettes

The `retinaConverter.py` Python 3 script will take a Retina palette file as input, and output equivalent `_r`, `_g` and `_b` arrays to replace in the script.
```$ python3 retinaConverter.py < paletteFile > array.txt```