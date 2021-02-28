#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'0','3','6','9'},
  {'1','4','7',':'},
  {'2','5','8',';'},
  {'A','B','C','D'}
};

#ifdef ARDUINO_AVR_LEONARDO

#ifdef PROTOTYPE_PCB
byte rowPins[ROWS] = {20, 16, 10, 14};
byte colPins[COLS] = {19, 18, 15, 21};
#else
byte rowPins[ROWS] = {5, 15, 9, 8};
byte colPins[COLS] = {6, 7, 18, 4};
#endif

#elif ESP32

#ifdef PROTOTYPE_PCB
byte rowPins[ROWS] = {23, 33, 32, 25};
byte colPins[COLS] = {14, 27, 26, 13};
#else
byte rowPins[ROWS] = {16, 26, 23, 19};
byte colPins[COLS] = {17, 18, 27, 4};
#endif

#else
#error this board is not supported! Must be Arduino Leonardo or ESP32
#endif

Keypad keypad = Keypad( makeKeymap(keys), colPins, rowPins, ROWS, COLS );