#include <HID-Project.h>
#include <Rotary.h>

//32u4 based interrupt pins: 0,1,2,3,7 (0&1 only if no Serial communication is needed)
const byte interruptPinMute = 1;
const byte interruptPinPlayPause = 2;
const byte interruptPinNext = 3;
const byte interruptPinPrev = 7;
const byte interruptPinVolRot = 8;
const byte interruptPinVolRot2 = 9;

Rotary rot = Rotary(interruptPinVolRot, interruptPinVolRot2);

//debouncing
#define DEBOUNCE_TIME 200 //debounce time in ms
volatile unsigned long lastTickMute;  // record time of keypress
volatile unsigned long lastTickNext;  // record time of keypress
volatile unsigned long lastTickPrev;  // record time of keypress
volatile unsigned long lastTickPlayPause;  // record time of keypress

void setup() {

  //configure Pins
  pinMode(interruptPinMute, INPUT_PULLUP);
  digitalWrite(interruptPinMute, HIGH);
  pinMode(interruptPinPlayPause, INPUT_PULLUP);
  digitalWrite(interruptPinPlayPause, HIGH);
  pinMode(interruptPinNext, INPUT_PULLUP);
  digitalWrite(interruptPinNext, HIGH);
  pinMode(interruptPinPrev, INPUT_PULLUP);
  digitalWrite(interruptPinPrev, HIGH);
  pinMode(interruptPinVolRot, INPUT_PULLUP);
  digitalWrite(interruptPinVolRot, HIGH);
  pinMode(interruptPinVolRot2, INPUT_PULLUP);
  digitalWrite(interruptPinVolRot2, HIGH);

  //configure voltage supply rotary
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);

  //configure ground sinks for button pins 7 and 1
  pinMode(0, OUTPUT);
  digitalWrite(0, LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, LOW);

  //configure interrupts
  attachInterrupt(digitalPinToInterrupt(interruptPinMute), mute, RISING);
  attachInterrupt(digitalPinToInterrupt(interruptPinPlayPause), playPause, RISING);
  attachInterrupt(digitalPinToInterrupt(interruptPinNext), next, RISING);
  attachInterrupt(digitalPinToInterrupt(interruptPinPrev), prev, RISING);

  //set up rotary encoder interrupts
  rot.begin();
  PCICR |= (1 << PCIE0);
  PCMSK0 |= (1 << PCINT4) | (1 << PCINT5);
  sei();

  Keyboard.begin();
  Serial.end(); //end serial communication just to be sure that pin 0 and 1 are available for GPIO use

  delay(500);
}

void loop() {
  //nothing, everything is handled in interrupts
}

//handle rotary encoder interrupts
ISR(PCINT0_vect) {
  unsigned char result = rot.process();
  if (result == DIR_NONE) {
    // do nothing
  }
  else if (result == DIR_CW) {
    Consumer.write(MEDIA_VOLUME_UP);
  }
  else if (result == DIR_CCW) {
    Consumer.write(MEDIA_VOLUME_DOWN);
  }
}

void mute() {
  unsigned long currentTime = millis();
  if ((currentTime - lastTickMute) > DEBOUNCE_TIME) {
    lastTickMute = currentTime;
    Keyboard.press(KEY_PAUSE); //discord mic-mute mapped to the break/pause key
    Keyboard.release(KEY_PAUSE);
  }
}

void next() {
  unsigned long currentTime = millis();
  if ((currentTime - lastTickNext) > DEBOUNCE_TIME) {
    lastTickNext = currentTime;
    Consumer.write(MEDIA_NEXT);
  }
}

void prev() {
  unsigned long currentTime = millis();
  if ((currentTime - lastTickPrev) > DEBOUNCE_TIME) {
    lastTickPrev = currentTime;
    Consumer.write(MEDIA_PREV);
  }
}

void playPause() {
  unsigned long currentTime = millis();
  if ((currentTime - lastTickPlayPause) > DEBOUNCE_TIME) {
    lastTickPlayPause = currentTime;
    Consumer.write(MEDIA_PLAY_PAUSE);
  }
}
