#include <Arduino.h>
#include <EEPROM.h>

// EEPROM memory addresses
byte globalLedColourAddress = 0;
byte globalLedBrightnessAddress = 1;
byte bar_address = 2;
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
byte switchActive[12] = {0};
byte selectedIndex = 0;
byte selectedLedColour = 0;
byte selectedLightingMode = 0;
byte selectedSwitchMode = 0;
byte selectedDelay = 0;
byte bar_value = 0;

// To detect value changes
byte global_led_brightness_current = 0;
byte selected_index_current = 0;

long knob_value = 0;
long previous_bar_value;

bool save_bar_value(void *) {

  EEPROM.write(bar_address, bar_value);
  #ifdef ESP32
  EEPROM.commit();
  #endif

  return true;
}

void EEPROM_save() {

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
}

void EEPROM_load() {

  selectedLedColour = localLedColour[selectedIndex];
  selectedLightingMode = localLightingMode[selectedIndex];
  selectedSwitchMode = localSwitchMode[selectedIndex];
  selectedDelay = localDelay[selectedIndex];

  EEPROM.write(selectedIndexAddress, selectedIndex);
  #ifdef ESP32
  EEPROM.commit();
  #endif
}

void EEPROM_setup() {

  #ifdef ESP32
  EEPROM.begin(53);
  #endif

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

    EEPROM.write(bar_address, 0);
    #ifdef ESP32
    EEPROM.commit();
    #endif

    EEPROM.write(freshByteAddress, 0);
    #ifdef ESP32
    EEPROM.commit();
    #endif
  }

  globalLedColour = EEPROM.read(globalLedColourAddress);
  globalLedBrightness = EEPROM.read(globalLedBrightnessAddress);
  global_led_brightness_current = globalLedBrightness;
  
  selectedIndex = EEPROM.read(selectedIndexAddress);
  selected_index_current = selectedIndex;

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

  bar_value = EEPROM.read(bar_address);
}