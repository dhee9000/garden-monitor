#pragma once

#if defined(ESP32)
// ESP32 WROOM-32 Pinout
#define I2C_SDA 21
#define I2C_SCL 22
#define SPI_MOSI 23
#define SPI_SCK 18
#define SPI_CS 5
#define SPI_DC 16
#define SPI_RST 17
#define BUTTON_PIN 4
#define LED_PIN 2
#elif defined(ESP8266)
// ESP8266 ESP-12E Pinout
#define I2C_SDA D2
#define I2C_SCL D1
#define SPI_MOSI D7
#define SPI_SCK D5
#define SPI_CS D8
#define SPI_DC D3
#define SPI_RST D4
#define BUTTON_PIN D6
#define LED_PIN D0
#else
#error "Unknown Platform..."
#endif

// Display type selection
#define DISPLAY_TYPE_SSD1306 0
#define DISPLAY_TYPE_GC9A01  1
#define DISPLAY_TYPE DISPLAY_TYPE_SSD1306 // Change to DISPLAY_TYPE_GC9A01 for round display

// SSD1306 OLED Display Settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32 // Change to 64 if your display is 128x64
#define OLED_RESET -1    // No reset pin

// GC9A01A Round TFT Display Settings
#define TFT_WIDTH 240
#define TFT_HEIGHT 240 