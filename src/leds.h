#define TM16XX_CMD_DATA_AUTO 0x40
#define TM16XX_CMD_DATA_READ 0x42
#define TM16XX_CMD_DATA_FIXED 0x44
#define TM16XX_CMD_DISPLAY 0x80
#define TM16XX_CMD_ADDRESS 0xC0

#ifdef ARDUINO_AVR_LEONARDO

#ifdef PROTOTYPE_PCB
#define dataPin 8
#define clockPin 7
#define strobePin 6
#else
#define dataPin 10
#define clockPin 16
#define strobePin 14
#endif

#elif ESP32

#ifdef PROTOTYPE_PCB
#define dataPin 19
#define clockPin 18
#define strobePin 5
#else
#define dataPin 32
#define clockPin 33
#define strobePin 25
#endif

#else
#error this board is not supported! Must be Arduino Leonardo or ESP32
#endif

byte grid_array[16] = {};

void send(byte data)
{
  for (int i = 0; i < 8; i++) {
    digitalWrite(clockPin, LOW);
    digitalWrite(dataPin, data & 1 ? HIGH : LOW);
    data >>= 1;
    digitalWrite(clockPin, HIGH);
  }
}

void sendCommand(byte cmd)
{
  digitalWrite(strobePin, LOW);
  send(cmd);
  digitalWrite(strobePin, HIGH);
}

void update_tm1638() {

  sendCommand(TM16XX_CMD_DATA_AUTO);
  digitalWrite(strobePin, LOW);
  send(TM16XX_CMD_ADDRESS);  

  for (int i = 0; i < 16; i++) {

    byte data = grid_array[i];
    send(data);
  }

  digitalWrite(strobePin, HIGH);
}

void update_led(byte number, byte RGB) {

  byte seg = number % 6;
  
  // red LED
  byte grid = number < 6 ? 4 : 10;

  if (RGB & 0x1) grid_array[grid] |= 1 << seg;
  else grid_array[grid] &= ~(1 << seg);

  // green LED
  grid = number < 6 ? 0 : 6;

  if (RGB & 0x2) grid_array[grid] |= 1 << seg;
  else grid_array[grid] &= ~(1 << seg);

  // blue LED
  grid = number < 6 ? 2 : 8;

  if (RGB & 0x4) grid_array[grid] |= 1 << seg;
  else grid_array[grid] &= ~(1 << seg);

  update_tm1638();
}

void update_all_leds(byte RGB) {

  for (int i = 0; i < 12; i++) {

    update_led(i, RGB);
  }
}

void update_leds() {

  for (byte i = 0; i < 12; i++) {

    byte my_colour = localLedColour[i];
    byte my_lighting_mode = localLightingMode[i];
    byte my_switch_mode = localSwitchMode[i];
    byte my_state = switchActive[i];

    // static
    if (my_switch_mode == 0) {

      if (my_lighting_mode == 1) update_led(i, my_colour);
      else update_led(i, globalLedColour);
    }

    // toggle, delay, hold
    else {

      // active
      if (my_state == 1) {

        if (my_lighting_mode == 1) update_led(i, my_colour);
        else update_led(i, globalLedColour);
      }

      // inactive
      else update_led(i, 0);
    }
  }
}

void update_bar(byte value) {

  int mask = (1 << value) - 1;

  grid_array[12] = mask & 0xFF;
  grid_array[13] = (mask >> 8) & 0xFF;

  update_tm1638();
}

void setupDisplay(boolean active, byte intensity)
{
  #ifdef ARDUINO_AVR_LEONARDO
  sendCommand(TM16XX_CMD_DISPLAY | (active ? 8 : 0) | min(7, intensity));
  #elif ESP32
  sendCommand(TM16XX_CMD_DISPLAY | (active ? 8 : 0) | _min(7, intensity));
  #endif
}

void LEDS_setup() {

  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(strobePin, OUTPUT);

  digitalWrite(strobePin, HIGH);
  digitalWrite(clockPin, HIGH);

  setupDisplay(true, globalLedBrightness);
  update_leds();
}

void LEDs_loop() {

  // save modified values to the local arrays
  if (selected_index_current != selectedIndex) {

    selected_index_current = selectedIndex;
    EEPROM_load();
  }

  localLedColour[selectedIndex] = selectedLedColour;
  localLightingMode[selectedIndex] = selectedLightingMode;
  localSwitchMode[selectedIndex] = selectedSwitchMode;
  localDelay[selectedIndex] = selectedDelay;

  // update the brightness
  if (global_led_brightness_current != globalLedBrightness) {

    global_led_brightness_current = globalLedBrightness;
    setupDisplay(true, globalLedBrightness);
  }

  // refresh the LEDs
  update_leds();
}