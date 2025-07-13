#include "DisplayManager.h"

DisplayManager::DisplayManager(TwoWire* i2c) {
#if DISPLAY_TYPE == DISPLAY_TYPE_SSD1306
    this->i2c = i2c;
    oled = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, i2c, OLED_RESET);
#elif DISPLAY_TYPE == DISPLAY_TYPE_GC9A01
    tft = new Adafruit_GC9A01A(SPI_CS, SPI_DC, SPI_RST);
#endif
}

bool DisplayManager::begin() {
#if DISPLAY_TYPE == DISPLAY_TYPE_SSD1306
    if (!oled->begin(SSD1306_SWITCHCAPVCC, 0x3C)) return false;
    oled->clearDisplay();
    setTextDefaults();
    oled->display();
    return true;
#elif DISPLAY_TYPE == DISPLAY_TYPE_GC9A01
    tft->begin();
    tft->fillScreen(COLOR_BLACK);
    setTextDefaults();
    return true;
#endif
}

void DisplayManager::setTextDefaults() {
#if DISPLAY_TYPE == DISPLAY_TYPE_SSD1306
    oled->setTextSize(1);
    oled->setTextColor(COLOR_WHITE);
    oled->setCursor(0, 0);
#elif DISPLAY_TYPE == DISPLAY_TYPE_GC9A01
    tft->setTextColor(COLOR_WHITE);
    tft->setTextSize(2);
    tft->setCursor(0, 0);
#endif
}

int16_t DisplayManager::getDisplayWidth() {
#if DISPLAY_TYPE == DISPLAY_TYPE_SSD1306
    return SCREEN_WIDTH;
#elif DISPLAY_TYPE == DISPLAY_TYPE_GC9A01
    return TFT_WIDTH;
#endif
}

int16_t DisplayManager::getDisplayHeight() {
#if DISPLAY_TYPE == DISPLAY_TYPE_SSD1306
    return SCREEN_HEIGHT;
#elif DISPLAY_TYPE == DISPLAY_TYPE_GC9A01
    return TFT_HEIGHT;
#endif
}

void DisplayManager::clearDisplay() {
#if DISPLAY_TYPE == DISPLAY_TYPE_SSD1306
    oled->clearDisplay();
#elif DISPLAY_TYPE == DISPLAY_TYPE_GC9A01
    tft->fillScreen(COLOR_BLACK);
#endif
}

void DisplayManager::drawCenteredText(const char* text, int16_t y, uint16_t color, uint8_t textSize) {
#if DISPLAY_TYPE == DISPLAY_TYPE_SSD1306
    oled->setTextSize(textSize);
    oled->setTextColor(color);
    int16_t x1, y1;
    uint16_t w, h;
    oled->getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    int16_t x = (getDisplayWidth() - w) / 2;
    oled->setCursor(x, y);
    oled->println(text);
#elif DISPLAY_TYPE == DISPLAY_TYPE_GC9A01
    tft->setTextSize(textSize);
    tft->setTextColor(color);
    int16_t x1, y1;
    uint16_t w, h;
    tft->getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    int16_t x = (getDisplayWidth() - w) / 2;
    tft->setCursor(x, y);
    tft->println(text);
#endif
}

void DisplayManager::showStartup() {
    clearDisplay();
#if DISPLAY_TYPE == DISPLAY_TYPE_SSD1306
    drawCenteredText("Garden Monitor", 0, COLOR_WHITE, 1);
    drawCenteredText("v1.0.0", 12, COLOR_WHITE, 1);
    drawCenteredText("Initializing...", 24, COLOR_WHITE, 1);
    oled->display();
#elif DISPLAY_TYPE == DISPLAY_TYPE_GC9A01
    drawCenteredText("Garden", 80, COLOR_WHITE, 3);
    drawCenteredText("Monitor", 110, COLOR_WHITE, 3);
    drawCenteredText("v1.0.0", 140, COLOR_CYAN, 2);
    drawCenteredText("Initializing...", 170, COLOR_YELLOW, 2);
#endif
}

void DisplayManager::showReadings(float temperature, float humidity, const char* ip, bool buttonPressed) {
    clearDisplay();
    
#if DISPLAY_TYPE == DISPLAY_TYPE_SSD1306
    oled->setCursor(0, 0);
    oled->setTextSize(1);
    oled->setTextColor(COLOR_WHITE);
    oled->println("Garden Monitor");
    
    oled->print("Temp: ");
    oled->print(temperature, 1);
    oled->println(" C");
    
    oled->print("Hum:  ");
    oled->print(humidity, 1);
    oled->println(" %");
    
    if (buttonPressed) {
        oled->println("Button Pressed");
    } else {
        oled->print("IP: ");
        oled->println(ip);
    }
    oled->display();
    
#elif DISPLAY_TYPE == DISPLAY_TYPE_GC9A01
    // Title
    drawCenteredText("Garden Monitor", 20, COLOR_WHITE, 2);
    
    // Temperature
    char tempStr[20];
    snprintf(tempStr, sizeof(tempStr), "%.1f C", temperature);
    tft->setTextColor(COLOR_RED);
    tft->setTextSize(3);
    tft->setCursor(20, 70);
    tft->print("Temp: ");
    tft->println(tempStr);
    
    // Humidity
    char humStr[20];
    snprintf(humStr, sizeof(humStr), "%.1f%%", humidity);
    tft->setTextColor(COLOR_BLUE);
    tft->setTextSize(3);
    tft->setCursor(20, 110);
    tft->print("Hum:  ");
    tft->println(humStr);
    
    // Status
    if (buttonPressed) {
        drawCenteredText("BUTTON PRESSED", 160, COLOR_YELLOW, 2);
    } else {
        tft->setTextColor(COLOR_GREEN);
        tft->setTextSize(1);
        tft->setCursor(10, 200);
        tft->print("WiFi: ");
        tft->println(ip);
    }
#endif
}

void DisplayManager::showWiFiConnecting(const char* ssid) {
    clearDisplay();
    
#if DISPLAY_TYPE == DISPLAY_TYPE_SSD1306
    oled->setCursor(0, 0);
    oled->setTextSize(1);
    oled->setTextColor(COLOR_WHITE);
    oled->println("Connecting WiFi...");
    oled->println(ssid);
    oled->display();
    
#elif DISPLAY_TYPE == DISPLAY_TYPE_GC9A01
    drawCenteredText("Connecting WiFi", 80, COLOR_YELLOW, 2);
    drawCenteredText(ssid, 120, COLOR_WHITE, 2);
    drawCenteredText("Please wait...", 160, COLOR_CYAN, 1);
#endif
}

void DisplayManager::showWiFiStatus(const char* status, const char* ip) {
    clearDisplay();
    
#if DISPLAY_TYPE == DISPLAY_TYPE_SSD1306
    oled->setCursor(0, 0);
    oled->setTextSize(1);
    oled->setTextColor(COLOR_WHITE);
    oled->println("WiFi Status:");
    oled->println(status);
    if (ip) {
        oled->println(ip);
    }
    oled->display();
    
#elif DISPLAY_TYPE == DISPLAY_TYPE_GC9A01
    drawCenteredText("WiFi Status", 60, COLOR_WHITE, 2);
    
    uint16_t statusColor = (strcmp(status, "Connected") == 0) ? COLOR_GREEN : COLOR_RED;
    drawCenteredText(status, 100, statusColor, 2);
    
    if (ip) {
        drawCenteredText(ip, 140, COLOR_CYAN, 1);
    }
#endif
}

void DisplayManager::showInfluxStatus(bool connected, const char* message) {
    clearDisplay();
    
#if DISPLAY_TYPE == DISPLAY_TYPE_SSD1306
    oled->setCursor(0, 0);
    oled->setTextSize(1);
    oled->setTextColor(COLOR_WHITE);
    oled->println("InfluxDB Status:");
    oled->println(connected ? "Connected" : "Disconnected");
    if (message) {
        oled->println(message);
    }
    oled->display();
    
#elif DISPLAY_TYPE == DISPLAY_TYPE_GC9A01
    drawCenteredText("InfluxDB Status", 60, COLOR_WHITE, 2);
    
    const char* statusText = connected ? "Connected" : "Disconnected";
    uint16_t statusColor = connected ? COLOR_GREEN : COLOR_RED;
    drawCenteredText(statusText, 100, statusColor, 2);
    
    if (message) {
        drawCenteredText(message, 140, COLOR_YELLOW, 1);
    }
#endif
} 