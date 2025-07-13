#include <Wire.h>
#include <AHTxx.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// --- Pin Definitions ---
// AHT10 Sensor (I2C bus 0)
#define AHT10_SDA 32
#define AHT10_SCL 33
// OLED Display (I2C bus 1)
#define OLED_SDA 34
#define OLED_SCL 35

// --- OLED Display Setup ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32 // Change to 64 if your display is 128x64
#define OLED_RESET    -1 // No reset pin
#define OLED_ADDR     0x3C // Most common I2C address for SSD1306

TwoWire I2C_OLED = TwoWire(1);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_OLED, OLED_RESET);

// --- AHT10 Sensor Setup ---
AHTxx aht10(AHTXX_ADDRESS_X38, AHT1x_SENSOR); // AHT10, address 0x38

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Initialize I2C for AHT10 sensor
  Wire.begin(AHT10_SDA, AHT10_SCL);
  // Initialize I2C for OLED display
  I2C_OLED.begin(OLED_SDA, OLED_SCL, 400000); // 400kHz for fast display

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Garden Monitor");
  display.display();
  delay(1000);

  // Initialize AHT10 sensor
  while (!aht10.begin()) {
    Serial.println("AHT10 not connected or initialization failed. Retrying in 2s...");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("AHT10 not found!");
    display.display();
    delay(2000);
  }
  Serial.println("AHT10 sensor initialized!");
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("AHT10 Ready!");
  display.display();
  delay(1000);
}

void loop() {
  float temperature = aht10.readTemperature();
  float humidity = aht10.readHumidity();

  // Print to serial
  Serial.print("Temperature: ");
  Serial.print(temperature, 2);
  Serial.print(" C, Humidity: ");
  Serial.print(humidity, 2);
  Serial.println(" %");

  // Display on OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Garden Monitor");
  display.setTextSize(2);
  display.setCursor(0, 16);
  display.print(temperature, 1);
  display.print(" C");
  display.setCursor(80, 16);
  display.print(humidity, 1);
  display.print(" %");
  display.display();

  delay(2000);
} 