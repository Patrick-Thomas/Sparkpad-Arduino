#ifndef SP_KEYPAD
#define SP_KEYPAD

#define KEYPAD_DEBOUNCE_MS 10

#include <Arduino.h>
#include "sparkpad_oled.h"

#ifdef ARDUINO_AVR_LEONARDO

#ifdef PROTOTYPE_PCB
byte rowPins[4] = {20, 16, 10, 14};
byte colPins[4] = {19, 18, 15, 21};
#else
byte rowPins[4] = {5, 15, 9, 8};
byte colPins[4] = {6, 7, 18, 4};
#endif

#elif ESP32

#ifdef PROTOTYPE_PCB
byte rowPins[4] = {23, 33, 32, 25};
byte colPins[4] = {14, 27, 26, 13};
#else
byte rowPins[4] = {16, 26, 23, 19};
byte colPins[4] = {17, 18, 27, 4};
#endif

#else
#error this board is not supported! Must be Arduino Leonardo or ESP32
#endif

// Prototypes
void key_pressed(byte);
void key_released(byte);
void knob_button();

byte bitMap[4];
byte bitMapNew[4];

unsigned long startTime = 0;

void scanKeys() {

	// Re-intialize the column pins. Allows sharing these pins with other hardware.
	for (byte c = 0; c < 4; c++) {
    
    pinMode(colPins[c], INPUT_PULLUP);
	}

	// bitMap stores ALL the keys that are being pressed.
	for (byte r = 0; r < 4; r++) {

		pinMode(rowPins[r], OUTPUT);
		digitalWrite(rowPins[r], LOW);	// Begin row pulse output

		for (byte c = 0; c < 4; c++) {

			bitWrite(bitMapNew[c], r, !digitalRead(colPins[c]));  // keypress is active low so invert to high.
		}

		// Set pin to high impedance input. Effectively ends row pulse.
		digitalWrite(rowPins[r], HIGH);
		pinMode(rowPins[r], INPUT);
	}
}

void updateKeys() {

	for (byte c = 0; c < 4; c++) {

		for (byte r = 0; r < 4; r++) {

      // Compare old and current values
			bool button_last = bitRead(bitMap[c], r);
      bool button_current = bitRead(bitMapNew[c], r);

      // button pressed
      if (button_last == false && button_current == true) {

        if (c < 3) key_pressed(r * 3 + c);

        else {

          switch (r) {

            case 0:
              knob_button();
              break;
            case 1:
              nav.doNav(downCmd);
              nav.doOutput();
              break;
            case 2:
              nav.doNav(enterCmd);
              nav.doOutput();
              break;
            case 3:
              nav.doNav(upCmd);
              nav.doOutput();
              break;
          }
        }
      }

      // button released
      else if (button_last == true && button_current == false) {

        if (c < 3) key_released(r * 3 + c);
      }

      bitWrite(bitMap[c], r, button_current);
    }
  }
}

void keypad_loop() {
	
	// Limit how often the keypad is scanned. This makes the loop() run 10 times as fast.
	if ( (millis() - startTime) > KEYPAD_DEBOUNCE_MS ) {

		scanKeys();
		updateKeys();
		startTime = millis();
	}
}

#endif