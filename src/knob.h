#include <Encoder.h>

#ifdef ARDUINO_AVR_LEONARDO

#ifdef PROTOTYPE_PCB
Encoder knob(1, 0);
#else
Encoder knob(0, 1);
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