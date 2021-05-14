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
#include "sparkpad_eeprom.h"
#include "sparkpad_leds.h"
#include "sparkpad_oled.h"
#include "sparkpad_knob.h"
#include "sparkpad_keypad.h"
#include "sparkpad_wifi.h"
// #include "sparkpad_obs.h"

/*
 * Definitions
 */

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
 * Knob + Bargraph behaviour
 */

Timer<1, millis> save_timer;

void knob_increase() {

  if (!muted) {
    
    knob_value = min(max(0, knob_value + 1), VOLUME_RANGE);
    update_bar(round(knob_value * (10.0 / VOLUME_RANGE)));
  }
}

void knob_decrease() {

  if (!muted) {
    
    knob_value = min(max(0, knob_value - 1), VOLUME_RANGE);
    update_bar(round(knob_value * (10.0 / VOLUME_RANGE)));
  }
}

void knob_button() {

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

}

/*
 * Arduino code
 */

void setup(){
  
  // EEPROM settings
  EEPROM_setup();

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
  update_bar(round(knob_value * (10.0 / VOLUME_RANGE)));
  save_timer.every(1000, save_knob_value);

  // Knob
  knob_setup();

  // Wifi
  WIFI_setup();

  // Obs
  // Serial.print(getOBSConfig());
}

/*
 * Main program loop - runs continously once setup() is complete
 */

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

  // do WIFI stuff
  WIFI_loop();
}
