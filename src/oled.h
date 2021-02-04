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

byte ledColourAddress = 0;
byte ledBrightnessAddress = 1;

byte ledColour = 0;
byte ledBrightness = 0;

byte led_colour_current = 0;
byte led_brightness_current = 0;

result adjustColour() {

  EEPROM.write(ledColourAddress, ledColour);
  return proceed;
}

result adjustBrightness() {

  EEPROM.write(ledBrightnessAddress, ledBrightness);
  return proceed;
}

MENU(mainMenu,"LED settings",doNothing,noEvent,wrapStyle
  ,FIELD(ledColour,"Colour","",0,7,1,0,adjustColour,exitEvent,noStyle)
  ,FIELD(ledBrightness,"Brightness","",0,7,1,0,adjustBrightness,exitEvent,noStyle)
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
