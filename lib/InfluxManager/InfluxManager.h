#pragma once
#include <Arduino.h>

class InfluxManager {
public:
    InfluxManager(const char* url, const char* org, const char* bucket, const char* token);
    bool begin();
    bool write(float temperature, float humidity);
    bool isConnected();
    const char* getLastError();
    void setDevice(const char* deviceName);
    bool syncTime();
    unsigned long getCurrentTimestamp();
    void setCertificate(const char* cert);
    
private:
    const char* url;
    const char* org;
    const char* bucket;
    const char* token;
    const char* device;
    const char* certificate;
    char lastError[128];
    bool connected;
    bool timeSync;
    unsigned long baseTimestamp;
    unsigned long baseMillis;
    
    void setError(const char* error);
    bool fetchTimeFromNTP();
}; 