#ifndef SP_OBS
#define SP_OBS
#include <ArduinoJson.h>
#include <WiFi.h>
#include <FS.h>
#include "SPIFFS.h"
#include <WiFiClientSecure.h>

#include <WebSocketsClient.h>
String ObsconfigPath = "/ObsSettings.json";

void getOBSConfig() {
    if(SPIFFS.exists(ObsconfigPath)){
        StaticJsonDocument<200> doc;
        
        File config = SPIFFS.open(configPath, "r");
        DeserializationError error = deserializeJson(doc, config);
        if (error){
            Serial.println(F("Failed to read file"));
        }

        String obs_ip = doc["ip"];
        String obs_port = doc["port"];
        String obs_password = doc["pass"];

        Serial.println(obs_ip);
        Serial.println(obs_port);
        Serial.println(obs_password);
    } else {
        Serial.println("obs is not configured, please configure first");
    }
}
#endif