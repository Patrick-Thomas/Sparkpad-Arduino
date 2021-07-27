#ifndef SP_EEPROM
#define SP_EEPROM

#include <Arduino.h>
#include <EEPROM.h>

// EEPROM memory addresses
byte globalLedColourAddress = 0;
byte globalLedBrightnessAddress = 1;
byte knob_address = 2;
byte localActiveLedColourAddress = 3;
byte localLightingModeAddress = 15;
byte localSwitchModeAddress = 27;
byte localDelayAddress = 39;
byte selectedIndexAddress = 51;
byte freshByteAddress = 52;
byte localDefaultToggleStateAddress = 53;
byte localInactiveLedColourAddress = 65;

// Values to update
byte globalLedColour = 0;
byte globalLedBrightness = 0;
byte localActiveLedColour[12] = {0};
byte localInactiveLedColour[12] = {0};
byte localLightingMode[12] = {0};
byte localSwitchMode[12] = {0};
byte localDelay[12] = {0};
byte localDefaultToggleState[12] = {0};
byte switchActive[12] = {0};
byte selectedIndex = 0;
byte selectedActiveLedColour = 0;
byte selectedInactiveLedColour = 0;
byte selectedLightingMode = 0;
byte selectedSwitchMode = 0;
byte selectedDefaultToggleState = 0;
byte selectedDelay = 0;
byte knob_value = 0;

// To detect value changes
byte global_led_brightness_current = 0;
byte selected_index_current = 0;
bool muted = false;

bool save_knob_value(void *) {

  EEPROM.write(knob_address, knob_value);
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

  EEPROM.write(localActiveLedColourAddress + selectedIndex, selectedActiveLedColour);
  #ifdef ESP32
  EEPROM.commit();
  #endif

  EEPROM.write(localInactiveLedColourAddress + selectedIndex, selectedInactiveLedColour);
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

  EEPROM.write(localDefaultToggleStateAddress + selectedIndex, selectedDefaultToggleState);
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

  selectedActiveLedColour = localActiveLedColour[selectedIndex];
  selectedInactiveLedColour = localInactiveLedColour[selectedIndex];
  selectedLightingMode = localLightingMode[selectedIndex];
  selectedSwitchMode = localSwitchMode[selectedIndex];
  selectedDefaultToggleState = localDefaultToggleState[selectedIndex];
  selectedDelay = localDelay[selectedIndex];

  EEPROM.write(selectedIndexAddress, selectedIndex);
  #ifdef ESP32
  EEPROM.commit();
  #endif
}

void EEPROM_setup() {

  #ifdef ESP32
  EEPROM.begin(77);
  #endif

  bool factory_reset = false;

  #ifdef FACTORY_RESET
  factory_reset = true;
  #endif

  // This should only run for a fresh Sparkpad
  if (EEPROM.read(freshByteAddress) == 255 || factory_reset) {

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

      EEPROM.write(localActiveLedColourAddress + i, 0);
      #ifdef ESP32
      EEPROM.commit();
      #endif

      EEPROM.write(localInactiveLedColourAddress + i, 0);
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

      EEPROM.write(localDefaultToggleStateAddress + i, 0);
      #ifdef ESP32
      EEPROM.commit();
      #endif

      EEPROM.write(localDelayAddress + i, 0);
      #ifdef ESP32
      EEPROM.commit();
      #endif
    }

    EEPROM.write(knob_address, 0);
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

    localActiveLedColour[i] = EEPROM.read(localActiveLedColourAddress + i);
    localInactiveLedColour[i] = EEPROM.read(localInactiveLedColourAddress + i);
    localLightingMode[i] = EEPROM.read(localLightingModeAddress + i);
    localSwitchMode[i] = EEPROM.read(localSwitchModeAddress + i);
    localDefaultToggleState[i] = EEPROM.read(localDefaultToggleStateAddress + i);
    localDelay[i] = EEPROM.read(localDelayAddress + i);
  }

  selectedActiveLedColour = localActiveLedColour[selectedIndex];
  selectedInactiveLedColour = localInactiveLedColour[selectedIndex];
  selectedLightingMode = localLightingMode[selectedIndex];
  selectedSwitchMode = localSwitchMode[selectedIndex];
  selectedDefaultToggleState = localDefaultToggleState[selectedIndex];
  selectedDelay = localDelay[selectedIndex];

  knob_value = EEPROM.read(knob_address);

  // populate switchActive array
  for (int i = 0; i < 12; i++) {

    // toggle or group toggle
    if ((localSwitchMode[i] == 1) || (localSwitchMode[i] == 3)) switchActive[i] = localDefaultToggleState[i];
  }

}

void EEPROM_loop() {

  // save modified values to the local arrays
  if (selected_index_current != selectedIndex) {

    selected_index_current = selectedIndex;
    EEPROM_load();
  }

  localActiveLedColour[selectedIndex] = selectedActiveLedColour;
  localInactiveLedColour[selectedIndex] = selectedInactiveLedColour;
  localLightingMode[selectedIndex] = selectedLightingMode;
  localSwitchMode[selectedIndex] = selectedSwitchMode;
  localDefaultToggleState[selectedIndex] = selectedDefaultToggleState;
  localDelay[selectedIndex] = selectedDelay;
}

#endif