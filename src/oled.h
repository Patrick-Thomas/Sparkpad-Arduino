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

#define LARGE_FONT Verdana12

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

byte globalLedColourAddress = 0;
byte globalLedBrightnessAddress = 1;

byte globalLedColour = 0;
byte globalLedBrightness = 0;

byte led_colour_current = 0;
byte led_brightness_current = 0;

byte localLedColour[12] = {};
byte localLightingMode[12] = {};
byte localSwitchMode[12] = {};
byte localDelay[12] = {};

byte localLedColourAddress = 3;
byte localLightingModeAddress = 15;
byte localSwitchModeAddress = 27;
byte localDelayAddress = 39;

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


result adjustGlobalColour() {

  EEPROM.write(globalLedColourAddress, globalLedColour);
  EEPROM.commit();
  return proceed;
}

result adjustGlobalBrightness() {

  EEPROM.write(globalLedBrightnessAddress, globalLedBrightness);
  EEPROM.commit();
  return proceed;
}

result adjustLocalColour() {

  for (int i = 0; i < 12; i++) {

    EEPROM.write(localLedColourAddress + i, localLedColour[i]);
    EEPROM.commit();
  }

  return proceed;
}

result adjustLightingMode() {

  for (int i = 0; i < 12; i++) {

    EEPROM.write(localLightingModeAddress + i, localLightingMode[i]);
    EEPROM.commit();
  }

  return proceed;
}

result adjustSwitchMode() {

  for (int i = 0; i < 12; i++) {

    EEPROM.write(localSwitchModeAddress + i, localSwitchMode[i]);
    EEPROM.commit();
  }

  return proceed;
}

result adjustDelay() {

  for (int i = 0; i < 12; i++) {

    EEPROM.write(localDelayAddress + i, localDelay[i]);
    EEPROM.commit();
  }

  return proceed;
}

MENU(localLedColourMenu,"Switch LED colour",doNothing,noEvent,wrapStyle
  ,FIELD(localLedColour[0],"Switch 1 colour","",0,7,1,0,adjustLocalColour,exitEvent,noStyle)
  ,FIELD(localLedColour[1],"Switch 2 colour","",0,7,1,0,adjustLocalColour,exitEvent,noStyle)
  ,FIELD(localLedColour[2],"Switch 3 colour","",0,7,1,0,adjustLocalColour,exitEvent,noStyle)
  ,FIELD(localLedColour[3],"Switch 4 colour","",0,7,1,0,adjustLocalColour,exitEvent,noStyle)
  ,FIELD(localLedColour[4],"Switch 5 colour","",0,7,1,0,adjustLocalColour,exitEvent,noStyle)
  ,FIELD(localLedColour[5],"Switch 6 colour","",0,7,1,0,adjustLocalColour,exitEvent,noStyle)
  ,FIELD(localLedColour[6],"Switch 7 colour","",0,7,1,0,adjustLocalColour,exitEvent,noStyle)
  ,FIELD(localLedColour[7],"Switch 8 colour","",0,7,1,0,adjustLocalColour,exitEvent,noStyle)
  ,FIELD(localLedColour[8],"Switch 9 colour","",0,7,1,0,adjustLocalColour,exitEvent,noStyle)
  ,FIELD(localLedColour[9],"Switch 10 colour","",0,7,1,0,adjustLocalColour,exitEvent,noStyle)
  ,FIELD(localLedColour[10],"Switch 11 colour","",0,7,1,0,adjustLocalColour,exitEvent,noStyle)
  ,FIELD(localLedColour[11],"Switch 12 colour","",0,7,1,0,adjustLocalColour,exitEvent,noStyle)
  ,EXIT("Back")
  );

MENU(localLightingModeMenu,"Switch LED mode",doNothing,noEvent,wrapStyle
  ,FIELD(localLightingMode[0],"Switch 1 LED mode","",0,1,1,0,adjustLightingMode,exitEvent,noStyle)
  ,FIELD(localLightingMode[1],"Switch 2 LED mode","",0,1,1,0,adjustLightingMode,exitEvent,noStyle)
  ,FIELD(localLightingMode[2],"Switch 3 LED mode","",0,1,1,0,adjustLightingMode,exitEvent,noStyle)
  ,FIELD(localLightingMode[3],"Switch 4 LED mode","",0,1,1,0,adjustLightingMode,exitEvent,noStyle)
  ,FIELD(localLightingMode[4],"Switch 5 LED mode","",0,1,1,0,adjustLightingMode,exitEvent,noStyle)
  ,FIELD(localLightingMode[5],"Switch 6 LED mode","",0,1,1,0,adjustLightingMode,exitEvent,noStyle)
  ,FIELD(localLightingMode[6],"Switch 7 LED mode","",0,1,1,0,adjustLightingMode,exitEvent,noStyle)
  ,FIELD(localLightingMode[7],"Switch 8 LED mode","",0,1,1,0,adjustLightingMode,exitEvent,noStyle)
  ,FIELD(localLightingMode[8],"Switch 9 LED mode","",0,1,1,0,adjustLightingMode,exitEvent,noStyle)
  ,FIELD(localLightingMode[9],"Switch 10 LED mode","",0,1,1,0,adjustLightingMode,exitEvent,noStyle)
  ,FIELD(localLightingMode[10],"Switch 11 LED mode","",0,1,1,0,adjustLightingMode,exitEvent,noStyle)
  ,FIELD(localLightingMode[11],"Switch 12 LED mode","",0,1,1,0,adjustLightingMode,exitEvent,noStyle)
  ,EXIT("Back")
  );

MENU(localSwitchModeMenu,"Switch mode",doNothing,noEvent,wrapStyle
  ,FIELD(localSwitchMode[0],"Switch 1 mode","",0,2,1,0,adjustSwitchMode,exitEvent,noStyle)
  ,FIELD(localSwitchMode[1],"Switch 2 mode","",0,2,1,0,adjustSwitchMode,exitEvent,noStyle)
  ,FIELD(localSwitchMode[2],"Switch 3 mode","",0,2,1,0,adjustSwitchMode,exitEvent,noStyle)
  ,FIELD(localSwitchMode[3],"Switch 4 mode","",0,2,1,0,adjustSwitchMode,exitEvent,noStyle)
  ,FIELD(localSwitchMode[4],"Switch 5 mode","",0,2,1,0,adjustSwitchMode,exitEvent,noStyle)
  ,FIELD(localSwitchMode[5],"Switch 6 mode","",0,2,1,0,adjustSwitchMode,exitEvent,noStyle)
  ,FIELD(localSwitchMode[6],"Switch 7 mode","",0,2,1,0,adjustSwitchMode,exitEvent,noStyle)
  ,FIELD(localSwitchMode[7],"Switch 8 mode","",0,2,1,0,adjustSwitchMode,exitEvent,noStyle)
  ,FIELD(localSwitchMode[8],"Switch 9 mode","",0,2,1,0,adjustSwitchMode,exitEvent,noStyle)
  ,FIELD(localSwitchMode[9],"Switch 10 mode","",0,2,1,0,adjustSwitchMode,exitEvent,noStyle)
  ,FIELD(localSwitchMode[10],"Switch 11 mode","",0,2,1,0,adjustSwitchMode,exitEvent,noStyle)
  ,FIELD(localSwitchMode[11],"Switch 12 mode","",0,2,1,0,adjustSwitchMode,exitEvent,noStyle)
  ,EXIT("Back")
  );

MENU(localDelayMenu,"Delay settings",doNothing,noEvent,wrapStyle
  ,FIELD(localDelay[0],"Switch 1 delay","s",0,255,1,0,adjustDelay,exitEvent,noStyle)
  ,FIELD(localDelay[1],"Switch 2 delay","s",0,255,1,0,adjustDelay,exitEvent,noStyle)
  ,FIELD(localDelay[2],"Switch 3 delay","s",0,255,1,0,adjustDelay,exitEvent,noStyle)
  ,FIELD(localDelay[3],"Switch 4 delay","s",0,255,1,0,adjustDelay,exitEvent,noStyle)
  ,FIELD(localDelay[4],"Switch 5 delay","s",0,255,1,0,adjustDelay,exitEvent,noStyle)
  ,FIELD(localDelay[5],"Switch 6 delay","s",0,255,1,0,adjustDelay,exitEvent,noStyle)
  ,FIELD(localDelay[6],"Switch 7 delay","s",0,255,1,0,adjustDelay,exitEvent,noStyle)
  ,FIELD(localDelay[7],"Switch 8 delay","s",0,255,1,0,adjustDelay,exitEvent,noStyle)
  ,FIELD(localDelay[8],"Switch 9 delay","s",0,255,1,0,adjustDelay,exitEvent,noStyle)
  ,FIELD(localDelay[9],"Switch 10 delay","s",0,255,1,0,adjustDelay,exitEvent,noStyle)
  ,FIELD(localDelay[10],"Switch 11 delay","s",0,255,1,0,adjustDelay,exitEvent,noStyle)
  ,FIELD(localDelay[11],"Switch 12 delay","s",0,255,1,0,adjustDelay,exitEvent,noStyle)
  ,EXIT("Back")
  );

MENU(mainMenu,"LED settings",doNothing,noEvent,wrapStyle
  ,FIELD(globalLedColour,"Colour","",0,7,1,0,adjustGlobalColour,exitEvent,noStyle)
  ,FIELD(globalLedBrightness,"Brightness","",0,7,1,0,adjustGlobalBrightness,exitEvent,noStyle)
  ,SUBMENU(localLedColourMenu)
  ,SUBMENU(localLightingModeMenu)
  ,SUBMENU(localSwitchModeMenu)
  ,SUBMENU(localDelayMenu)
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
