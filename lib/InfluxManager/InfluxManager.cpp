#include "InfluxManager.h"
#include <InfluxDbClient.h>

#if defined(ESP32)
#include <WiFi.h>
#include <HTTPClient.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#endif

InfluxDBClient* client = nullptr;
Point sensorData("environment");

InfluxManager::InfluxManager(const char* url, const char* org, const char* bucket, const char* token)
    : url(url), org(org), bucket(bucket), token(token), device("garden-monitor"), 
      certificate(nullptr), connected(false), timeSync(false), baseTimestamp(0), baseMillis(0) {
    strcpy(lastError, "Not initialized");
}

void InfluxManager::setCertificate(const char* cert) {
    certificate = cert;
}

bool InfluxManager::begin() {
    if (client) {
        delete client;
    }
    
    client = new InfluxDBClient(url, org, bucket, token);
    
    // Configure for HTTPS connections
    if (strstr(url, "https://")) {
        Serial.println("HTTPS URL detected - using insecure mode to avoid SSL issues");
        // Use insecure mode to bypass SSL certificate verification
        // This avoids the start_ssl_client -1 errors
        client->setInsecure(true);
        setError("Using insecure HTTPS mode");
        
        // Set longer timeout for HTTPS connections
        client->setHTTPOptions(HTTPOptions().httpReadTimeout(15000));
    } else {
        Serial.println("HTTP URL detected - no SSL configuration needed");
        client->setHTTPOptions(HTTPOptions().httpReadTimeout(10000));
    }
    
    // Add device tag
    sensorData.addTag("device", device);
    sensorData.addTag("location", "garden");
    
    // Sync time first
    if (!syncTime()) {
        setError("Time sync failed - continuing anyway");
        // Continue anyway, but timestamps will be inaccurate
    }
    
    // Test connection with timeout
    Serial.println("Testing InfluxDB connection...");
    if (client->validateConnection()) {
        connected = true;
        setError("Connected successfully");
        return true;
    } else {
        connected = false;
        String errorMsg = "Connection failed: " + client->getLastErrorMessage();
        setError(errorMsg.c_str());
        return false;
    }
}

bool InfluxManager::syncTime() {
    if (WiFi.status() != WL_CONNECTED) {
        setError("WiFi not connected for time sync");
        return false;
    }
    
    return fetchTimeFromNTP();
}

bool InfluxManager::fetchTimeFromNTP() {
#if defined(ESP32)
    HTTPClient http;
    http.begin("http://worldtimeapi.org/api/timezone/Etc/UTC");
    http.setTimeout(5000);
    
    int httpCode = http.GET();
    Serial.print("WorldTimeAPI HTTP response: ");
    Serial.println(httpCode);
    
    if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.print("WorldTimeAPI response: ");
        Serial.println(payload);
        http.end();
        
        // Parse the JSON response to get unixtime
        int unixTimeStart = payload.indexOf("\"unixtime\":") + 11;
        if (unixTimeStart > 10) {
            int unixTimeEnd = payload.indexOf(",", unixTimeStart);
            String unixTimeStr = payload.substring(unixTimeStart, unixTimeEnd);
            
            baseTimestamp = unixTimeStr.toInt();
            baseMillis = millis();
            timeSync = true;
            
            Serial.print("Time sync successful!");
            Serial.print(" Base timestamp: ");
            Serial.print(baseTimestamp);
            Serial.print(" (");
            Serial.print(unixTimeStr);
            Serial.print("), Base millis: ");
            Serial.println(baseMillis);
            
            char timeMsg[64];
            snprintf(timeMsg, sizeof(timeMsg), "Time synced: %lu", baseTimestamp);
            setError(timeMsg);
            return true;
        } else {
            Serial.println("Failed to parse unixtime from response");
        }
    } else {
        Serial.print("WorldTimeAPI request failed with code: ");
        Serial.println(httpCode);
    }
    http.end();
    
#elif defined(ESP8266)
    WiFiClient wifiClient;
    HTTPClient http;
    http.begin(wifiClient, "http://worldtimeapi.org/api/timezone/Etc/UTC");
    http.setTimeout(5000);
    
    int httpCode = http.GET();
    Serial.print("WorldTimeAPI HTTP response: ");
    Serial.println(httpCode);
    
    if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.print("WorldTimeAPI response: ");
        Serial.println(payload);
        http.end();
        
        // Parse the JSON response to get unixtime
        int unixTimeStart = payload.indexOf("\"unixtime\":") + 11;
        if (unixTimeStart > 10) {
            int unixTimeEnd = payload.indexOf(",", unixTimeStart);
            String unixTimeStr = payload.substring(unixTimeStart, unixTimeEnd);
            
            baseTimestamp = unixTimeStr.toInt();
            baseMillis = millis();
            timeSync = true;
            
            Serial.print("Time sync successful!");
            Serial.print(" Base timestamp: ");
            Serial.print(baseTimestamp);
            Serial.print(" (");
            Serial.print(unixTimeStr);
            Serial.print("), Base millis: ");
            Serial.println(baseMillis);
            
            char timeMsg[64];
            snprintf(timeMsg, sizeof(timeMsg), "Time synced: %lu", baseTimestamp);
            setError(timeMsg);
            return true;
        } else {
            Serial.println("Failed to parse unixtime from response");
        }
    } else {
        Serial.print("WorldTimeAPI request failed with code: ");
        Serial.println(httpCode);
    }
    http.end();
#endif
    
    Serial.println("Time sync failed - will use system time(nullptr) as fallback");
    setError("Time sync failed - using current time");
    return false;
}

unsigned long InfluxManager::getCurrentTimestamp() {
    if (!timeSync) {
        unsigned long fallbackTime = time(nullptr);
        Serial.print("No time sync - using fallback time: ");
        Serial.println(fallbackTime);
        return fallbackTime;
    }
    
    // Calculate current timestamp based on millis offset
    unsigned long currentMillis = millis();
    unsigned long elapsedSeconds = (currentMillis - baseMillis) / 1000;
    unsigned long calculatedTimestamp = baseTimestamp + elapsedSeconds;
    
    Serial.print("Timestamp calculation: currentMillis=");
    Serial.print(currentMillis);
    Serial.print(", baseMillis=");
    Serial.print(baseMillis);
    Serial.print(", elapsedSeconds=");
    Serial.print(elapsedSeconds);
    Serial.print(", baseTimestamp=");
    Serial.print(baseTimestamp);
    Serial.print(", calculatedTimestamp=");
    Serial.println(calculatedTimestamp);
    
    return calculatedTimestamp;
}

bool InfluxManager::write(float temperature, float humidity) {
    if (!client) {
        setError("Client not initialized");
        return false;
    }
    
    Serial.println("=== InfluxDB Write Debug ===");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("°C, Humidity: ");
    Serial.print(humidity);
    Serial.println("%");
    
    sensorData.clearFields();
    sensorData.addField("temperature", temperature);
    sensorData.addField("humidity", humidity);
    
    // Use our synchronized timestamp
    unsigned long timestamp = getCurrentTimestamp();
    sensorData.setTime(timestamp);
    
    Serial.print("Setting timestamp: ");
    Serial.print(timestamp);
    Serial.print(" (");
    
    // Convert timestamp to human readable for debugging
    time_t rawtime = timestamp;
    struct tm * timeinfo = gmtime(&rawtime);
    char timeStr[64];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S UTC", timeinfo);
    Serial.print(timeStr);
    Serial.println(")");
    
    Serial.print("Data point: ");
    Serial.print("measurement=environment,device=");
    Serial.print(device);
    Serial.print(",location=garden temperature=");
    Serial.print(temperature);
    Serial.print(",humidity=");
    Serial.print(humidity);
    Serial.print(" ");
    Serial.println(timestamp);
    
    Serial.print("Sending to InfluxDB... ");
    if (client->writePoint(sensorData)) {
        connected = true;
        char successMsg[64];
        snprintf(successMsg, sizeof(successMsg), "Write OK (ts: %lu)", timestamp);
        setError(successMsg);
        Serial.println("SUCCESS!");
        Serial.println("=== End InfluxDB Write ===");
        return true;
    } else {
        connected = false;
        String errorMsg = client->getLastErrorMessage();
        setError(errorMsg.c_str());
        Serial.print("FAILED! Error: ");
        Serial.println(errorMsg);
        Serial.println("=== End InfluxDB Write ===");
        return false;
    }
}

bool InfluxManager::isConnected() {
    return connected;
}

const char* InfluxManager::getLastError() {
    return lastError;
}

void InfluxManager::setDevice(const char* deviceName) {
    device = deviceName;
    if (client) {
        sensorData.clearTags();
        sensorData.addTag("device", device);
        sensorData.addTag("location", "garden");
    }
}

void InfluxManager::setError(const char* error) {
    strncpy(lastError, error, sizeof(lastError) - 1);
    lastError[sizeof(lastError) - 1] = '\0';
} 