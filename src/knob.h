#ifndef SP_KNOB
#define SP_KNOB
#include <Arduino.h>
#include <Encoder.h>

#ifdef ARDUINO_AVR_LEONARDO

#ifdef PROTOTYPE_PCB
Encoder knob(5, 4);
#else
Encoder knob(20, 19);
#endif

#elif ESP32

#ifdef PROTOTYPE_PCB
Encoder knob(17, 16);
#else
Encoder knob(13, 14);
#endif

#else
#error this board is not supported! Must be Arduino Leonardo or ESP32
#endif
#endif