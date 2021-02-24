# Arduino Multimedia Keyboard

## Overview
Since my new keyboard requires me to press some fn + X key combinations to access the multimedia keys, I quickly bought an Arduino Pro Micro clone, 4 Buttons and a rotary encoder and created this simple project.

All pins on one side are used (0-9).

## Functions
4 Buttons are used for
- Play/Pause
- Next Track
- Previous Track
- Mic-Mute (emulates pause/break key which is mapped to toggle mute in discord)

Rotary encoder used for volume up and volume down. Button press of encoder is not mapped.

## Notes
- 32u4 based Arduino is needed for the native USB interface to emulate HID. Other solutions for Uno/Nano exist but require more effort.
- Debouncing interval of buttons can be changed if you have more/less sensitive buttons.
- Pins 0 and 1 are used as GPIO, therefore theres no serial communication

## Libraries used
### - [Rotary by brianlow](https://github.com/brianlow/Rotary)
### - [HID Project by NicoHood](https://github.com/NicoHood/HID)

