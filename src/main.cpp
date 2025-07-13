#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Toggle.h>
#include "WiFi.h"

// --- WiFi Credentials ---
const char* KNOWN_SSIDS[] = {
  "NotResNet",
  "TP-Link_5E98"
};

const char* KNOWN_PASSWORDS[] = {
  "DY123456",
  "Dheeraj@1710"
};

// --- Pin Definitions ---
#define BUTTON_PIN 23
#define LED_PIN 2
#define I2C_SCL 22
#define I2C_SDA 21

// --- OLED Display Setup ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32 // Change to 64 if your display is 128x64
#define OLED_RESET    -1 // No reset pin

TwoWire I2C_PORT = TwoWire(1);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_PORT, OLED_RESET);

// --- AHT10 Sensor Setup ---
Adafruit_AHTX0 aht10;

Toggle button(BUTTON_PIN);

time_t last_serial_print = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  delay(1000);

  Serial.println("Garden Monitor v0.1.0");
  Serial.println("Initializing Firmware...");

  // Initialize I2C port
  I2C_PORT.begin(I2C_SDA, I2C_SCL);

  // Initialize I2C for AHT10 sensor
  if (!aht10.begin(&I2C_PORT)) {
    Serial.println("AHT10 sensor allocation failed");
    while (1);
  }
  Serial.println("AHT10 sensor initialized!");

  // Initialize I2C for OLED display
  
  Serial.println("OLED display initialized!");
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1);
  }

  // Initialize OLED display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Garden Monitor v0.1.0"));
  display.println(F("Initializing..."));
  display.display();
  delay(1000);

  // Initialize WiFi
  Serial.println("Initializing WiFi...");
  Serial.print(sizeof(KNOWN_SSIDS));
  Serial.println(" known networks");
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(F("Garden Monitor v0.1.0"));
  display.println(F("Initializing WiFi..."));
  display.print(sizeof(KNOWN_SSIDS));
  display.println(F(" known networks"));
  display.display();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);
  for (int i = 0; i < sizeof(KNOWN_SSIDS); i++) {
    Serial.print("Connecting to ");
    Serial.print(KNOWN_SSIDS[i]);
    Serial.println("...");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(F("Garden Monitor v0.1.0"));
    display.println(F("Initializing WiFi..."));
    display.print(F("Connecting to "));
    display.print(KNOWN_SSIDS[i]);
    display.println(F("..."));
    display.display();
    WiFi.begin(KNOWN_SSIDS[i], KNOWN_PASSWORDS[i]);
    int timeout = 10;
    while (WiFi.status() != WL_CONNECTED && timeout > 0) {
      delay(500);
      timeout--;
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Connected!");
      Serial.print("IP: ");
      Serial.println(WiFi.localIP());
      break;
    }
    if (WiFi.status() == WL_CONNECT_FAILED) {
      Serial.println("Connection failed!");
      continue;
    }
  }
  Serial.println("WiFi initialized!");

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(F("Garden Monitor v0.1.0"));
  display.println(F("Ready"));
  display.println(F("WiFi: "));
  display.println(WiFi.localIP());
  display.display();

  delay(1000);

}

void loop() {

  sensors_event_t humidity, temp;
  aht10.getEvent(&humidity, &temp);

  button.poll();

  if (button.isPressed()) {
    Serial.println("Button pressed");
    digitalWrite(LED_PIN, HIGH);
  }
  if (button.isReleased()) {
    Serial.println("Button released");
    digitalWrite(LED_PIN, LOW);
  }

  if (millis() - last_serial_print > 1000) {
    last_serial_print = millis();
    Serial.print("Temp: ");
    Serial.print(temp.temperature);
    Serial.print(" °C, Hum: ");
    Serial.print(humidity.relative_humidity);
    Serial.println(" %");
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(F("Garden Monitor"));
  display.print("Temp: ");
  display.print(temp.temperature, 1);
  display.println(" C");
  display.print("Hum:  ");
  display.print(humidity.relative_humidity, 1);
  display.println(" %");
  if (button.isPressed()) {
    display.println(F("Button Pressed"));
  } else {
    display.print(F("WiFi: "));
    display.println(WiFi.localIP());
  }
  display.display();

} 