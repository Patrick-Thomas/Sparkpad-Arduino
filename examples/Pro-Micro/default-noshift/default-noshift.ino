/*
 * default firmware for the Pro Micro board (aka Arduino Leonardo)
 */

// #define PROTOTYPE_PCB
// #define FACTORY_RESET

/*
 * Files to include
 */

// Non-core files
#include <Arduino.h>
#include <HID-Project.h>
#include <arduino-timer.h>

// Core files
#include "sparkpad_eeprom.h"
#include "sparkpad_oled.h"
#include "sparkpad_knob.h"
#include "sparkpad_leds.h"
#include "sparkpad_keypad.h"

/*
 * Definitions
 */

// Map keys for the buttons and knob here. For a list of available keys, check out:
// https://github.com/NicoHood/HID/blob/master/src/KeyboardLayouts/ImprovedKeylayouts.h

KeyboardKeycode key_mappings[12] = {

  KEY_F13,          // Key 1
  KEY_F14,          // Key 2
  KEY_F15,          // Key 3
  KEY_F16,          // Key 4
  KEY_F17,          // Key 5
  KEY_F18,          // Key 6
  KEY_F19,          // Key 7
  KEY_F20,          // Key 8
  KEY_F21,          // Key 9
  KEY_F22,          // Key 10
  KEY_F23,          // Key 11
  KEY_F24           // Key 12
};

#define KNOB_INCREASE MEDIA_VOLUME_UP
#define KNOB_DECREASE MEDIA_VOLUME_DOWN
#define KNOB_BUTTON MEDIA_VOLUME_MUTE

// Choose your OS here (Comment out incorrect OS) 
#define OS_WINDOWS10
// #define OS_UBUNTU

#ifdef OS_WINDOWS10
#define VOLUME_RANGE 50
#endif

#ifdef OS_UBUNTU
#define VOLUME_RANGE 18
#endif

/*
 * Knob + Bargraph behaviour
 */

Timer<1, millis> save_timer;

void knob_increase() {

  Consumer.write(KNOB_INCREASE);

  if (!muted) {
    
    knob_value = min(max(0, knob_value + 1), VOLUME_RANGE);
    update_bar(round(knob_value * (10.0 / VOLUME_RANGE)));
  }
}

void knob_decrease() {

  Consumer.write(KNOB_DECREASE);

  if (!muted) {
    
    knob_value = min(max(0, knob_value - 1), VOLUME_RANGE);
    update_bar(round(knob_value * (10.0 / VOLUME_RANGE)));
  }
}

void knob_button() {

  Consumer.write(KNOB_BUTTON);

  if (muted) {

    update_bar(round(knob_value * (10.0 / VOLUME_RANGE)));
    muted = false;
  }

  else {
    
    update_bar(0);
    muted = true;
  }
}

/*
 * Keypad behaviour
 */

Timer<1, millis, byte> timer_array[12];

bool timer_callback(byte index) {

  switchActive[index] = 0;
  return true;
}

void key_pressed(byte index) {

  Keyboard.press(key_mappings[index]);

  byte my_mode = localSwitchMode[index];
  byte my_state = switchActive[index];
  byte my_delay = localDelay[index];

  switch (my_mode) {

    // static
    case 0:
      break;

    // toggle
    case 1:

      if (my_state == 0) switchActive[index] = 1;
      else switchActive[index] = 0;
      break;

    // delay
    case 2:

      // stop active timer
      timer_array[index].cancel();
      
      // start timer
      timer_array[index].in(my_delay*1000, timer_callback, index);

      switchActive[index] = 1;
      break;

    // group toggle
    case 3:

      // turn off all buttons in a group
      for (int i = 0; i < 12; i++) {

        if (localSwitchMode[i] == 3) switchActive[i] = 0;
      }

      switchActive[index] = 1;
      break;
  }
}

void key_released(byte index) {

  Keyboard.release(key_mappings[index]);
}

/*
 * Arduino code
 */

void setup(){

  // EEPROM settings
  EEPROM_setup();

  // Keyboard
  Keyboard.begin();
  Consumer.begin();

  // Serial must be disabled for HID to work properly
  // Serial.begin(9600);
  // while(!Serial);

  // OLED
  Wire.begin();
  oled.begin(&Adafruit128x64, 0x3C);
  oled.setFont(menuFont);
  oled.clear();

  // If commented out, screen initialises blank
  // nav.doOutput();

  // LEDs
  LEDS_setup();

  // Bar graph
  update_bar(round(knob_value * (10.0 / VOLUME_RANGE)));
  save_timer.every(1000, save_knob_value);

  // Knob
  knob_setup();
}

void loop() {

  // check keypad
  keypad_loop();

  // check knob
  knob_loop();

  // update EEPROM
  EEPROM_loop();

  // update LEDs
  LEDs_loop();

  // tick timers
  for (int i = 0; i < 12; i++) {

    timer_array[i].tick();
  }

  save_timer.tick();
}