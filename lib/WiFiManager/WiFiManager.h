#pragma once
#include <vector>
#include <Arduino.h>

class WiFiManager {
public:
    WiFiManager(const std::vector<const char*>& ssids, const std::vector<const char*>& passwords);
    bool connect();
    const char* getLocalIP();
    bool isConnected();
private:
    std::vector<const char*> ssids;
    std::vector<const char*> passwords;
    int connectedIndex = -1;
}; 