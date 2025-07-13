#include "WiFiManager.h"
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

WiFiManager::WiFiManager(const std::vector<const char*>& ssids, const std::vector<const char*>& passwords)
    : ssids(ssids), passwords(passwords) {}

bool WiFiManager::connect() {
#if defined(ESP32) || defined(ESP8266)
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(1000);
    
    for (size_t i = 0; i < ssids.size() && i < passwords.size(); ++i) {
        Serial.print("Connecting to ");
        Serial.print(ssids[i]);
        Serial.println("...");
        
        WiFi.begin(ssids[i], passwords[i]);
        int timeout = 20; // Increased timeout
        while (WiFi.status() != WL_CONNECTED && timeout > 0) {
            delay(500);
            Serial.print(".");
            timeout--;
        }
        
        if (WiFi.status() == WL_CONNECTED) {
            connectedIndex = i;
            Serial.println();
            Serial.print("Connected! IP: ");
            Serial.println(WiFi.localIP());
            return true;
        }
        
        Serial.println(" Failed");
        WiFi.disconnect();
        delay(1000);
    }
#endif
    return false;
}

const char* WiFiManager::getLocalIP() {
#if defined(ESP32) || defined(ESP8266)
    static String ipString;
    if (WiFi.status() == WL_CONNECTED) {
        ipString = WiFi.localIP().toString();
        return ipString.c_str();
    }
#endif
    return "0.0.0.0";
}

bool WiFiManager::isConnected() {
#if defined(ESP32) || defined(ESP8266)
    return WiFi.status() == WL_CONNECTED;
#else
    return false;
#endif
} 