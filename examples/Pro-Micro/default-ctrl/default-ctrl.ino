/*
 * default firmware for the Pro Micro board (aka Arduino Leonardo)
 */

// #define PROTOTYPE_PCB

/*
 * Files to include
 */

// Non-core files
#include <Arduino.h>
#include <HID-Project.h>
#include <arduino-timer.h>

// Core files
#include "eeprom.h"
#include "oled.h"
#include "knob.h"
#include "leds.h"
#include "keypad.h"

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
#define BUTTON_10 KEY_LEFT_CTRL
#define BUTTON_11 KEY_F23
#define BUTTON_12 KEY_F24

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

  Consumer.write(KNOB_INCREASE);
}

void knob_decrease() {

  Consumer.write(KNOB_DECREASE);
}

void knob_button() {

  Consumer.press(KNOB_BUTTON);

  if (bar_value != 0)
  {
    previous_bar_value = bar_value;
    bar_value = 0;
  }
  else
  {
    bar_value = previous_bar_value;
  }

  bar_value_changed();
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
  
      case '0': 
        Keyboard.press(BUTTON_1);
        pressed_callback(0);
        break;

      case '1': 
        Keyboard.press(BUTTON_2); 
        pressed_callback(1);
        break;

      case '2': 
        Keyboard.press(BUTTON_3); 
        pressed_callback(2);
        break;

      case '3': 
        Keyboard.press(BUTTON_4);
        pressed_callback(3);
        break;

      case '4': 
        Keyboard.press(BUTTON_5); 
        pressed_callback(4);
        break;

      case '5': 
        Keyboard.press(BUTTON_6); 
        pressed_callback(5);
        break;

      case '6': 
        Keyboard.press(BUTTON_7);
        pressed_callback(6);
        break;

      case '7': 
        Keyboard.press(BUTTON_8); 
        pressed_callback(7);
        break;

      case '8': 
        Keyboard.press(BUTTON_9); 
        pressed_callback(8);
        break;

      case '9': 
        Keyboard.press(BUTTON_10);
        pressed_callback(9);
        break;

      case ':': 
        Keyboard.press(BUTTON_11); 
        pressed_callback(10);
        break;

      case ';': 
        Keyboard.press(BUTTON_12); 
        pressed_callback(11);
        break;

      case 'A':
        knob_button();
        break;
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
      default: break;  
    }
  }
  
  else if (kpadState == RELEASED) {
  
    switch (key) {
  
      case '0': Keyboard.release(BUTTON_1); break;
      case '1': Keyboard.release(BUTTON_2); break;
      case '2': Keyboard.release(BUTTON_3); break;
      case '3': Keyboard.release(BUTTON_4); break;
      case '4': Keyboard.release(BUTTON_5); break;
      case '5': Keyboard.release(BUTTON_6); break;
      case '6': Keyboard.release(BUTTON_7); break;
      case '7': Keyboard.release(BUTTON_8); break;
      case '8': Keyboard.release(BUTTON_9); break;
      case '9': Keyboard.release(BUTTON_10); break;
      case ':': Keyboard.release(BUTTON_11); break;
      case ';': Keyboard.release(BUTTON_12); break;
      case 'A': Consumer.release(KNOB_BUTTON); break;
      default: break;
    }
  }
}

void setup(){

  // EEPROM settings
  EEPROM_setup();

  // Keyboard
  Keyboard.begin();
  keypad.addStatedEventListener(keyEventListener);
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
//  nav.doOutput();

  // LEDs
  LEDS_setup();

  // Bar graph
  bar_value_changed();
  save_timer.every(1000, save_bar_value);
}

void loop() {

  // check keypad and knob
  keypad.getKeys();
  long knob_value_new = knob.read();

  if (knob_value_new != knob_value) {

    if (knob_value_new > knob_value) knob_increase();
    else knob_decrease();

    byte bar_value_new = min(max(0, bar_value + (knob_value_new - knob_value)),VOLUME_RANGE);
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
}