#include "../lib/Config/Config.h"
#include "../lib/SensorManager/SensorManager.h"
#include "../lib/DisplayManager/DisplayManager.h"
#include "../lib/WiFiManager/WiFiManager.h"
#include "../lib/ButtonManager/ButtonManager.h"
#include "../lib/InfluxManager/InfluxManager.h"
#include <vector>
#include <Wire.h>

// --- WiFi Credentials ---
const std::vector<const char*> KNOWN_SSIDS = { "NotResNet", "TP-Link_5E98" };
const std::vector<const char*> KNOWN_PASSWORDS = { "DY123456", "Dheeraj@1710" };

// --- InfluxDB Config ---
const char* INFLUX_URL = "https://influx.dhrj.io";
const char* INFLUX_ORG = "notresnet";
const char* INFLUX_BUCKET = "garden_dev";
const char* INFLUX_TOKEN = "UHxssyBFt4tcyYcAjjm34_t_NnmaPb9VKHUWTm66Wm5yi7RV2iNqmJkpfiYfzCgOJu1hdl8QsC7VkhIRi3TSlA==";

// Use default Wire instance for both platforms
SensorManager sensor(&Wire);
DisplayManager display(&Wire);
WiFiManager wifi(KNOWN_SSIDS, KNOWN_PASSWORDS);
ButtonManager button;
InfluxManager influx(INFLUX_URL, INFLUX_ORG, INFLUX_BUCKET, INFLUX_TOKEN);

unsigned long last_serial_print = 0;
unsigned long last_influx_write = 0;
unsigned long last_display_update = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("Garden Monitor v1.0.0");
    Serial.println("Initializing...");
    
    // Initialize LED
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
    
    // Initialize button
    button.begin();
    
    // Initialize I2C
    Wire.begin(I2C_SDA, I2C_SCL);
    
    // Initialize sensor
    if (!sensor.begin()) {
        Serial.println("ERROR: AHT sensor initialization failed!");
        while (1) {
            digitalWrite(LED_PIN, HIGH);
            delay(100);
            digitalWrite(LED_PIN, LOW);
            delay(100);
        }
    }
    Serial.println("AHT sensor initialized");
    
    // Initialize display
    if (!display.begin()) {
        Serial.println("ERROR: Display initialization failed!");
        while (1) {
            digitalWrite(LED_PIN, HIGH);
            delay(200);
            digitalWrite(LED_PIN, LOW);
            delay(200);
        }
    }
    Serial.println("Display initialized");
    
    // Show startup screen
    display.showStartup();
    delay(2000);
    
    // Initialize WiFi
    Serial.println("Connecting to WiFi...");
    display.showWiFiConnecting("Scanning...");
    
    if (wifi.connect()) {
        Serial.println("WiFi connected successfully");
        display.showWiFiStatus("Connected", wifi.getLocalIP());
    } else {
        Serial.println("WiFi connection failed");
        display.showWiFiStatus("Failed", nullptr);
    }
    delay(2000);
    
    // Initialize InfluxDB
    Serial.println("Initializing InfluxDB...");
    influx.setDevice("garden-monitor-v1");
    
    Serial.println("Syncing time...");
    display.showInfluxStatus(false, "Syncing time...");
    delay(1000);
    
    if (influx.begin()) {
        Serial.println("InfluxDB connected successfully");
        Serial.print("Last status: ");
        Serial.println(influx.getLastError());
        display.showInfluxStatus(true, "Ready");
    } else {
        Serial.print("InfluxDB connection failed: ");
        Serial.println(influx.getLastError());
        display.showInfluxStatus(false, influx.getLastError());
    }
    delay(2000);
    
    Serial.println("Setup complete - entering main loop");
}

void loop() {
    float temperature = 0, humidity = 0;
    
    // Read sensor data
    if (!sensor.read(temperature, humidity)) {
        Serial.println("Sensor read failed");
        temperature = -999;
        humidity = -999;
    }
    
    // Handle button
    button.poll();
    bool buttonPressed = button.isPressed();
    
    // Update LED based on button state
    digitalWrite(LED_PIN, buttonPressed ? HIGH : LOW);
    
    // Serial output every second
    if (millis() - last_serial_print > 1000) {
        last_serial_print = millis();
        Serial.print("Temp: ");
        Serial.print(temperature, 1);
        Serial.print("°C, Humidity: ");
        Serial.print(humidity, 1);
        Serial.print("%, WiFi: ");
        Serial.print(wifi.isConnected() ? "Connected" : "Disconnected");
        Serial.print(", InfluxDB: ");
        Serial.println(influx.isConnected() ? "Connected" : "Disconnected");
    }
    
    // Write to InfluxDB every 10 seconds
    if (wifi.isConnected() && millis() - last_influx_write > 10000) {
        last_influx_write = millis();
        Serial.print("Writing to InfluxDB... ");
        if (influx.write(temperature, humidity)) {
            Serial.println("Success!");
            Serial.print("Status: ");
            Serial.println(influx.getLastError());
        } else {
            Serial.println("Failed!");
            Serial.print("Error: ");
            Serial.println(influx.getLastError());
        }
    }
    
    // Update display every 500ms
    if (millis() - last_display_update > 500) {
        last_display_update = millis();
        display.showReadings(temperature, humidity, wifi.getLocalIP(), buttonPressed);
    }
    
    delay(50); // Small delay to prevent overwhelming the system
} 