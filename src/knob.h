#include <Encoder.h>

#ifdef PROTOTYPE_PCB
Encoder knob(5, 4);
#else
Encoder knob(20, 19);
#endif

long knob_value = 0;
byte bar_value = 0;
