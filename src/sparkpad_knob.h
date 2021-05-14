#ifndef SP_KNOB
#define SP_KNOB

#define KNOB_DEBOUNCE_MS 5

#include <Arduino.h>
#include <arduino-timer.h>

#ifdef ARDUINO_AVR_LEONARDO

#ifdef PROTOTYPE_PCB
#define pinA 1
#define pinB 0
#else
#define pinA 0
#define pinB 1
#endif

#elif ESP32

#ifdef PROTOTYPE_PCB
#define pinA 17
#define pinB 16
#else
#define pinA 13
#define pinB 14
#endif

#else
#error this board is not supported! Must be Arduino Leonardo or ESP32
#endif

// Prototypes
void knob_increase();
void knob_decrease();

#ifdef PROTOTYPE_PCB

Timer<1, millis> knob_timer;

uint8_t state = 0;

bool knob_callback(void *) {

    uint8_t s = state & 3;

    if (digitalRead(pinA)) s |= 4;
    if (digitalRead(pinB)) s |= 8;

    switch (s) {

        case 0: case 5: case 10: case 15:
            break;

        case 1: case 7: case 8: case 14:
            knob_increase();
            break;

        case 2: case 4: case 11: case 13:
            knob_decrease();
            break;

        case 3: case 12:
            knob_increase();
            break;

        default:
            knob_decrease();
            break;
    }
    
    state = (s >> 2);

    return true;
}

#else

void pinA_ISR() {

    if (digitalRead(pinB)) knob_increase();
    else knob_decrease();
}

#endif

void knob_setup() {

    #ifdef PROTOTYPE_PCB

    pinMode(pinA, INPUT);
    pinMode(pinB, INPUT);
    if (digitalRead(pinA)) state |= 1;
    if (digitalRead(pinB)) state |= 2;

    knob_timer.every(KNOB_DEBOUNCE_MS, knob_callback);

    #else

    attachInterrupt(digitalPinToInterrupt(pinA), pinA_ISR, RISING);

    #endif
}

void knob_loop() {

    #ifdef PROTOTYPE_PCB
    knob_timer.tick();
    #endif
}

#endif