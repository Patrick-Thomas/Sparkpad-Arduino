//
// *** N.B. This code is currently under construction! ***
//

/*
 * default firmware for the ESP32
 */

#define PROTOTYPE_PCB

/*
 * Files to include
 */

// Non-core files
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <FS.h>
#include "SPIFFS.h"
#include <arduino-timer.h>

// Core files
#include "eeprom.h"
#include "leds.h"
#include "oled.h"
#include "knob.h"
#include "sparkpad_keypad.h"
#include "sparkpad_wifi.h"
#include "sparkpad_obs.h"

/*
 * Definitions
 */

// Map keys for the buttons and knob here. For a list of available keys, check out:
// https://github.com/NicoHood/HID/blob/master/src/KeyboardLayouts/ImprovedKeylayouts.h

#define BUTTON_1 KEY_F13
#define BUTTON_2 KEY_F14
#define BUTTON_3 KEY_F15
#define BUTTON_4 KEY_F16
#define BUTTON_5 KEY_F17
#define BUTTON_6 KEY_F18
#define BUTTON_7 KEY_F19
#define BUTTON_8 KEY_F20
#define BUTTON_9 KEY_F21
#define BUTTON_10 KEY_RIGHT_SHIFT
#define BUTTON_11 KEY_F23
#define BUTTON_12 KEY_F24

#define KNOB_INCREASE MEDIA_VOLUME_UP
#define KNOB_DECREASE MEDIA_VOLUME_DOWN
#define KNOB_BUTTON MEDIA_VOLUME_MUTE

// Choose your OS here (Comment out incorrect OS) 
#define OS_WINDOWS10
//#define OS_UBUNTU

#ifdef OS_WINDOWS10
#define VOLUME_RANGE 50
#endif

#ifdef OS_UBUNTU
#define VOLUME_RANGE 18
#endif

/*
 * Bar graph behaviour
 */

Timer<1, millis> save_timer;

void bar_value_changed() {

  float translated = bar_value * (10.0 / VOLUME_RANGE);
  update_bar(round(translated));
}

/*
 * Knob behaviour
 */

void knob_increase() {

  Serial.println("Knob increase");
}

void knob_decrease() {

  Serial.println("Knob decrease");
}

void knob_button() {

  Serial.println("Knob button");
}

/*
 * Keypad behaviour
 */

// Timer array and callbacks

Timer<1, millis, byte> timer_array[12];

bool timer_callback(byte index) {

  switchActive[index] = 0;

  return true;
}

void pressed_callback(byte index) {

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
  }
}

void keyEventListener(KeypadEvent key, KeyState kpadState) {
  if (kpadState == PRESSED) {
  
    switch (key) {
  
      case 'B': 
        nav.doNav(downCmd);
        nav.doOutput();
        break;
      case 'C': 
        nav.doNav(enterCmd);
        nav.doOutput();
        break;
      case 'D': 
        nav.doNav(upCmd);
        nav.doOutput();
        break;
      default: 
        Serial.print("Press: ");
        Serial.println(key);
        break;  
    }
  }
  
  else if (kpadState == RELEASED) {
  
    switch (key) {

      case 'B':
      case 'C':
      case 'D':
        break;
      default:
        Serial.print("Release: ");
        Serial.println(key);
        break;
    }
  }
}

/*
 * Setup function
 */

void setup(){
  
  // EEPROM
  EEPROM_setup();

  // Keyboard
  keypad.addStatedEventListener(keyEventListener);

  // Serial
  Serial.begin(9600);

  // OLED
  Wire.begin();
  oled.begin(&Adafruit128x64, 0x3C);
  oled.setFont(menuFont);
  oled.clear();

  // If commented out, screen initialises blank
//  nav.doOutput();

  // LEDs
  LEDS_setup();

  // Bar graph
  bar_value_changed();
  save_timer.every(1000, save_bar_value);

  // Wifi
  WIFI_setup();

  // Obs
  // Serial.print(getOBSConfig());
}

/*
 * Main program loop - runs continously once setup() is complete
 */

void loop() {

  // check keypad and knob
  keypad.getKeys();
  long knob_value_new = knob.read();

  if (knob_value_new != knob_value) {

    if (knob_value_new > knob_value) knob_increase();
    else knob_decrease();

    byte bar_value_new = _min(_max(0, bar_value + (knob_value_new - knob_value)),VOLUME_RANGE);
    knob_value = knob_value_new;

    if (bar_value_new != bar_value) {

      bar_value = bar_value_new;
      bar_value_changed();
    }
  }

  // tick timers
  for (int i = 0; i < 12; i++) {

    timer_array[i].tick();
  }

  save_timer.tick();

  // update LEDs
  LEDs_loop();

  // do WIFI stuff
  WIFI_loop();
}
