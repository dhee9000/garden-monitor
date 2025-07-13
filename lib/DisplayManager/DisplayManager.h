#pragma once
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "../Config/Config.h"

#if DISPLAY_TYPE == DISPLAY_TYPE_SSD1306
#include <Adafruit_SSD1306.h>
#elif DISPLAY_TYPE == DISPLAY_TYPE_GC9A01
#include <Adafruit_GC9A01A.h>
#endif

class DisplayManager {
public:
    DisplayManager(TwoWire* i2c = nullptr);
    bool begin();
    void showStartup();
    void showReadings(float temperature, float humidity, const char* ip, bool buttonPressed);
    void showWiFiConnecting(const char* ssid);
    void showWiFiStatus(const char* status, const char* ip = nullptr);
    void showInfluxStatus(bool connected, const char* message = nullptr);
    void clearDisplay();
    void drawCenteredText(const char* text, int16_t y, uint16_t color = 0xFFFF, uint8_t textSize = 1);
    
private:
#if DISPLAY_TYPE == DISPLAY_TYPE_SSD1306
    Adafruit_SSD1306* oled;
    TwoWire* i2c;
    static const uint16_t COLOR_WHITE = SSD1306_WHITE;
    static const uint16_t COLOR_BLACK = SSD1306_BLACK;
#elif DISPLAY_TYPE == DISPLAY_TYPE_GC9A01
    Adafruit_GC9A01A* tft;
    static const uint16_t COLOR_WHITE = GC9A01A_WHITE;
    static const uint16_t COLOR_BLACK = GC9A01A_BLACK;
    static const uint16_t COLOR_RED = GC9A01A_RED;
    static const uint16_t COLOR_GREEN = GC9A01A_GREEN;
    static const uint16_t COLOR_BLUE = GC9A01A_BLUE;
    static const uint16_t COLOR_YELLOW = GC9A01A_YELLOW;
    static const uint16_t COLOR_CYAN = GC9A01A_CYAN;
    static const uint16_t COLOR_MAGENTA = GC9A01A_MAGENTA;
#endif
    
    void setTextDefaults();
    int16_t getDisplayWidth();
    int16_t getDisplayHeight();
}; 