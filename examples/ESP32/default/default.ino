//
// *** N.B. This code is currently under construction! ***
//

/*
 * default firmware for the ESP32
 */

#define PROTOTYPE_PCB

/*
 * Files to include
 */

// Core files
#include "oled.h"
#include "knob.h"
#include "leds.h"
#include <Keypad.h>

// Extra files
#include <WiFi.h>
#include <WebServer.h>

/*
 * Definitions
 */

// Map keys for the buttons and knob here. For a list of available keys, check out:
// https://github.com/NicoHood/HID/blob/master/src/KeyboardLayouts/ImprovedKeylayouts.h

#define BUTTON_1 KEY_F13
#define BUTTON_2 KEY_F14
#define BUTTON_3 KEY_F15
#define BUTTON_4 KEY_F16
#define BUTTON_5 KEY_F17
#define BUTTON_6 KEY_F18
#define BUTTON_7 KEY_F19
#define BUTTON_8 KEY_F20
#define BUTTON_9 KEY_F21
#define BUTTON_10 KEY_RIGHT_SHIFT
#define BUTTON_11 KEY_F23
#define BUTTON_12 KEY_F24

#define KNOB_INCREASE MEDIA_VOLUME_UP
#define KNOB_DECREASE MEDIA_VOLUME_DOWN
#define KNOB_BUTTON MEDIA_VOLUME_MUTE

// Choose your OS here (Comment out incorrect OS) 
#define OS_WINDOWS10
//#define OS_UBUNTU

#ifdef OS_WINDOWS10
#define VOLUME_RANGE 50
#endif

#ifdef OS_UBUNTU
#define VOLUME_RANGE 18
#endif

const char* ssid = "sparkpad-test";
const char* pass = "123";

IPAddress localIP(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);

/*
 * Firmware begins here
 */

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'0','3','6','9'},
  {'1','4','7',':'},
  {'2','5','8',';'},
  {'A','B','C','D'}
};

#ifdef PROTOTYPE_PCB
byte rowPins[ROWS] = {23, 33, 32, 25};
byte colPins[COLS] = {14, 27, 26, 13};
#else
byte rowPins[ROWS] = {16, 26, 23, 19};
byte colPins[COLS] = {17, 18, 27, 4};
#endif

Keypad keypad = Keypad( makeKeymap(keys), colPins, rowPins, ROWS, COLS );

void keyEventListener(KeypadEvent key, KeyState kpadState) {
  if (kpadState == PRESSED) {
  
    switch (key) {
  
      case 'B': 
        nav.doNav(downCmd);
        nav.doOutput();
        break;
      case 'C': 
        nav.doNav(enterCmd);
        nav.doOutput();
        break;
      case 'D': 
        nav.doNav(upCmd);
        nav.doOutput();
        break;
      default: 
        Serial.print("Press: ");
        Serial.println(key);
        break;  
    }
  }
  
  else if (kpadState == RELEASED) {
  
    switch (key) {

      case 'B':
      case 'C':
      case 'D':
        break;
      default:
        Serial.print("Release: ");
        Serial.println(key);
        break;
    }
  }
}

void update_leds() {

  setupDisplay(true, ledBrightness);
  update_all_leds(ledColour);
}

String sendHtml()
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>LED Control</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #3498db;}\n";
  ptr += ".button-on:active {background-color: #2980b9;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>Sparkpad Web Server</h1>\n";
  ptr += "<h3>Using Access Point(AP) Mode</h3>\n";
  ptr += "<h1>Welcome to testing :D </h1>";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}

void handleNotFound()
{
  server.send(404, "text/plain", "You are in the wrong place buddy");
}

void handleOnConnect()
{
  server.send(200, "text/html", sendHtml());
  Serial.println("Client Connected");
}

void setup(){

  // EEPROM settings
  ledColour = EEPROM.read(ledColourAddress);
  ledBrightness = EEPROM.read(ledBrightnessAddress);
  led_colour_current = ledColour;
  led_brightness_current = ledBrightness;

  Serial.begin(115200);

  // Keyboard
  keypad.addStatedEventListener(keyEventListener);

  // OLED
  Wire.begin();
  oled.begin(&Adafruit128x64, 0x3C);
  oled.setFont(menuFont);
  oled.clear();

  // If commented out, screen initialises blank
//  nav.doOutput();

  // LEDs
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(strobePin, OUTPUT);
  
  digitalWrite(strobePin, HIGH);
  digitalWrite(clockPin, HIGH);

  update_leds();

  Serial.println("Starting Wifi Server (AP)");
  WiFi.softAP(ssid);
  delay(100);

  IPAddress ip = WiFi.softAPIP();
  Serial.print("AP IP Address: ");
  Serial.println(ip);

  server.on("/", handleOnConnect);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("Server started!");
}

void loop() {

  keypad.getKeys();

  long knob_value_new = knob.read();

  if (knob_value_new != knob_value) {

    if (knob_value_new > knob_value) Serial.println("Knob increase");
    else Serial.println("Knob decrease");

    bar_value = _min(_max(0, bar_value + (knob_value_new - knob_value)),VOLUME_RANGE);
    knob_value = knob_value_new;
    float translated = bar_value*(10.0/VOLUME_RANGE);
    update_bar(round(translated));
  }

  if (led_colour_current != ledColour) {

    led_colour_current = ledColour;
    update_leds();
  }

  if (led_brightness_current != ledBrightness) {

    led_brightness_current = ledBrightness;
    update_leds();
  }

  // Wifi Server
  server.handleClient();
}
