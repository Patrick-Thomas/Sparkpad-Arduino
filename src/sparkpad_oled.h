#ifndef SP_OLED
#define SP_OLED

#include <Arduino.h>
#include <menu.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include <menuIO/SSD1306AsciiOut.h>
#include <menuIO/chainStream.h>
// #include <menuIO/serialIO.h>

#include "sparkpad_eeprom.h"

using namespace Menu;

SSD1306AsciiWire oled;

// Should be 1 for bicolour OLEDs
#define VERTICAL_OFFSET 0

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

result saver() {

  EEPROM_save();
  return proceed;
}

result loader() {
  
  EEPROM_load();
  return proceed;
}

MENU(keyMenu, "Key settings",doNothing,noEvent,wrapStyle
  ,FIELD(selectedLedColour,"Colour","",0,7,1,0,saver,exitEvent,noStyle)
  ,FIELD(selectedLightingMode,"Lighting mode","",0,1,1,0,saver,exitEvent,noStyle)
  ,FIELD(selectedSwitchMode,"Switch mode","",0,3,1,0,saver,exitEvent,noStyle)
  ,FIELD(selectedDefaultToggleState,"Default state","",0,1,1,0,saver,exitEvent,noStyle)
  // ,FIELD(selectedDefaultGroupToggleState,"Default group toggle state","",0,1,1,0,saver,exitEvent,noStyle)
  ,FIELD(selectedDelay,"Delay","s",0,255,1,0,saver,exitEvent,noStyle)
  ,EXIT("<Back")
  );

MENU(mainMenu,"Main menu",doNothing,noEvent,wrapStyle
  ,FIELD(globalLedColour,"Colour","",0,7,1,0,saver,exitEvent,noStyle)
  ,FIELD(globalLedBrightness,"Brightness","",0,7,1,0,saver,exitEvent,noStyle)
  ,FIELD(selectedIndex,"Selected key","",0,11,1,0,loader,exitEvent,noStyle)
  ,SUBMENU(keyMenu)
  ,EXIT("Hide")
  );

const panel panels[] MEMMODE = {{0, VERTICAL_OFFSET, 128 / fontW, 64 / fontH}};
navNode* nodes[sizeof(panels) / sizeof(panel)]; //navNodes to store navigation status
panelsList pList(panels, nodes, 1); //a list of panels and nodes
idx_t tops[MAX_DEPTH] = {0, 0}; //store cursor positions for each level

#ifdef LARGE_FONT
SSD1306AsciiOut outOLED(&oled, tops, pList, 8, 2); //oled output device menu driver

#else
SSD1306AsciiOut outOLED(&oled, tops, pList, 5, 1); //oled output device menu driver
#endif

menuOut* constMEM outputs[] MEMMODE = {&outOLED}; //list of output devices
outputsList out(outputs, 1); //outputs list

// stringIn<0> strIn;//buffer size: 2^5 = 32 bytes, eventually use 0 for a single byte
// serialIn in(Serial);
chainStream<0> in(NULL);

NAVROOT(nav,mainMenu,MAX_DEPTH,in,out);

#endif