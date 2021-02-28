#include <Arduino.h>
#include <menu.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include <menuIO/SSD1306AsciiOut.h>
#include <menuIO/serialIO.h>
#include <EEPROM.h>

using namespace Menu;

SSD1306AsciiWire oled;

#define MAX_DEPTH 2

#define FIRST_LINE 0 //text position for first line
#define SECOND_LINE 1 //text position for second line

//#define LARGE_FONT Verdana12

 /*Do not change the values(recomended)*/
#ifdef LARGE_FONT
#define menuFont LARGE_FONT
#define fontW 8
#define fontH 16
#else
// #define menuFont System5x7
#define menuFont lcd5x7
#define fontW 5
#define fontH 8
#endif

/*
 * LED settings
 */

// EEPROM memory addresses
byte globalLedColourAddress = 0;
byte globalLedBrightnessAddress = 1;
byte localLedColourAddress = 3;
byte localLightingModeAddress = 15;
byte localSwitchModeAddress = 27;
byte localDelayAddress = 39;
byte selectedIndexAddress = 51;
byte freshByteAddress = 52;

// Values to update
byte globalLedColour = 0;
byte globalLedBrightness = 0;
byte localLedColour[12] = {0};
byte localLightingMode[12] = {0};
byte localSwitchMode[12] = {0};
byte localDelay[12] = {0};
byte selectedIndex = 0;
byte selectedLedColour = 0;
byte selectedLightingMode = 0;
byte selectedSwitchMode = 0;
byte selectedDelay = 0;

// To detect value changes
byte global_led_colour_current = 0;
byte global_led_brightness_current = 0;
byte selected_led_colour_current = 0;
byte selected_lighting_mode_current = 0;

void update_leds();
void update_led(byte number, byte RGB);

/*
Add three more menus on the oled screen, 12 entries each
- one menu for individual switch colour 
- one menu for lighting mode
- one menu for switch mode

lighting modes

-- global (switch is the global colour)
-- individual

switch mode

-- static
-- toggle
-- delay 

delay setting

8 bit value in seconds

*/

result saver() {

  EEPROM.write(globalLedColourAddress, globalLedColour);
  #ifdef ESP32
  EEPROM.commit();
  #endif

  EEPROM.write(globalLedBrightnessAddress, globalLedBrightness);
  #ifdef ESP32
  EEPROM.commit();
  #endif

  EEPROM.write(localLedColourAddress + selectedIndex, selectedLedColour);
  #ifdef ESP32
  EEPROM.commit();
  #endif

  EEPROM.write(localLightingModeAddress + selectedIndex, selectedLightingMode);
  #ifdef ESP32
  EEPROM.commit();
  #endif

  EEPROM.write(localSwitchModeAddress + selectedIndex, selectedSwitchMode);
  #ifdef ESP32
  EEPROM.commit();
  #endif

  EEPROM.write(localDelayAddress + selectedIndex, selectedDelay);
  #ifdef ESP32
  EEPROM.commit();
  #endif

  EEPROM.write(selectedIndexAddress, selectedIndex);
  #ifdef ESP32
  EEPROM.commit();
  #endif

  return proceed;
}

result loader() {

  selectedLedColour = localLedColour[selectedIndex];
  selectedLightingMode = localLightingMode[selectedIndex];
  selectedSwitchMode = localSwitchMode[selectedIndex];
  selectedDelay = localDelay[selectedIndex];

  EEPROM.write(selectedIndexAddress, selectedIndex);
  #ifdef ESP32
  EEPROM.commit();
  #endif

  return proceed;
}

byte fresh_check(byte input) {

  return input == 255 ? 0 : input;
}

void EEPROM_init() {

  // This should only run for a fresh Sparkpad
  if (EEPROM.read(freshByteAddress) == 255) {

    EEPROM.write(globalLedColourAddress, 0);
    #ifdef ESP32
    EEPROM.commit();
    #endif

    EEPROM.write(globalLedBrightnessAddress, 0);
    #ifdef ESP32
    EEPROM.commit();
    #endif

    EEPROM.write(selectedIndexAddress, 0);
    #ifdef ESP32
    EEPROM.commit();
    #endif

    for (int i = 0; i < 12; i++) {

      EEPROM.write(localLedColourAddress + i, 0);
      #ifdef ESP32
      EEPROM.commit();
      #endif

      EEPROM.write(localLightingModeAddress + i, 0);
      #ifdef ESP32
      EEPROM.commit();
      #endif

      EEPROM.write(localSwitchModeAddress + i, 0);
      #ifdef ESP32
      EEPROM.commit();
      #endif

      EEPROM.write(localDelayAddress + i, 0);
      #ifdef ESP32
      EEPROM.commit();
      #endif
    }

    EEPROM.write(freshByteAddress, 0);
    #ifdef ESP32
    EEPROM.commit();
    #endif
  }

  globalLedColour = EEPROM.read(globalLedColourAddress);
  globalLedBrightness = EEPROM.read(globalLedBrightnessAddress);
  global_led_colour_current = globalLedColour;
  global_led_brightness_current = globalLedBrightness;

  selectedIndex = EEPROM.read(selectedIndexAddress);

  for (int i = 0; i < 12; i++) {

    localLedColour[i] = EEPROM.read(localLedColourAddress + i);
    localLightingMode[i] = EEPROM.read(localLightingModeAddress + i);
    localSwitchMode[i] = EEPROM.read(localSwitchModeAddress + i);
    localDelay[i] = EEPROM.read(localDelayAddress + i);
  }

  selectedLedColour = localLedColour[selectedIndex];
  selectedLightingMode = localLightingMode[selectedIndex];
  selectedSwitchMode = localSwitchMode[selectedIndex];
  selectedDelay = localDelay[selectedIndex];
}

MENU(mainMenu,"LED settings",doNothing,noEvent,wrapStyle
  ,FIELD(globalLedColour,"Colour","",0,7,1,0,saver,exitEvent,noStyle)
  ,FIELD(globalLedBrightness,"Brightness","",0,7,1,0,saver,exitEvent,noStyle)
  ,FIELD(selectedIndex,"Switch","",0,11,1,0,loader,exitEvent,noStyle)
  ,FIELD(selectedLedColour,"Colour","",0,7,1,0,saver,exitEvent,noStyle)
  ,FIELD(selectedLightingMode,"Lighting mode","",0,1,1,0,saver,exitEvent,noStyle)
  ,FIELD(selectedSwitchMode,"Switch mode","",0,2,1,0,saver,exitEvent,noStyle)
  ,FIELD(selectedDelay,"Delay","s",0,255,1,0,saver,exitEvent,noStyle)
  ,EXIT("Hide")
  );

const panel panels[] MEMMODE = {{0, 0, 128 / fontW, 64 / fontH}};
navNode* nodes[sizeof(panels) / sizeof(panel)]; //navNodes to store navigation status
panelsList pList(panels, nodes, 1); //a list of panels and nodes
idx_t tops[MAX_DEPTH] = {0, 0}; //store cursor positions for each level

#ifdef LARGE_FONT
SSD1306AsciiOut outOLED(&oled, tops, pList, 8, 2); //oled output device menu driver

#else
SSD1306AsciiOut outOLED(&oled, tops, pList, 5, 1); //oled output device menu driver
#endif

menuOut* constMEM outputs[]  MEMMODE  = {&outOLED}; //list of output devices
outputsList out(outputs, 1); //outputs list

//stringIn<0> strIn;//buffer size: 2^5 = 32 bytes, eventually use 0 for a single byte
serialIn serial(Serial);

NAVROOT(nav,mainMenu,MAX_DEPTH,serial,out);
