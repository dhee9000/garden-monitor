# Garden Monitor: ESP32/ESP8266 + AHT Sensor + SSD1306/GC9A01A Display

Monitor temperature and humidity using an AHT sensor and display the readings on either an SSD1306 OLED or GC9A01A round display, powered by ESP32 or ESP8266. Data is automatically sent to InfluxDB for remote monitoring and visualization. Modular codebase with full Adafruit library support for easy extension and cross-platform compatibility.

## Features
- **Cross-platform**: Full ESP32 and ESP8266 support
- **Dual display support**: SSD1306 OLED (I2C) or GC9A01A round TFT (SPI)
- **Real-time monitoring**: Temperature and humidity with AHT10/AHT20 sensor
- **InfluxDB integration**: Automatic data logging with error handling
- **WiFi management**: Multi-network support with connection status
- **Button interface**: Ready for menu/mode selection (UI coming soon)
- **Modular design**: Hardware abstraction for easy maintenance
- **Adafruit ecosystem**: Uses official Adafruit libraries throughout

## Hardware Required
- ESP32 WROOM-32 or ESP8266 ESP-12E board
- AHT10/AHT20 temperature & humidity sensor (I2C)
- SSD1306 OLED (I2C) **OR** GC9A01A round TFT display (SPI)
- Jumper wires and breadboard
- Button (for menu/mode selection)

## Wiring & Pinouts

### ESP32 WROOM-32 Pinout
| Function   | Pin | Notes |
|------------|-----|-------|
| I2C SDA    | 21  | For AHT sensor & SSD1306 |
| I2C SCL    | 22  | For AHT sensor & SSD1306 |
| SPI MOSI   | 23  | For GC9A01A only |
| SPI SCK    | 18  | For GC9A01A only |
| SPI CS     | 5   | For GC9A01A only |
| SPI DC     | 16  | For GC9A01A only |
| SPI RST    | 17  | For GC9A01A only |
| Button     | 4   | Pull-up, active low |
| LED        | 2   | Status indicator |

### ESP8266 ESP-12E Pinout
| Function   | Pin | Notes |
|------------|-----|-------|
| I2C SDA    | D2  | For AHT sensor & SSD1306 |
| I2C SCL    | D1  | For AHT sensor & SSD1306 |
| SPI MOSI   | D7  | For GC9A01A only |
| SPI SCK    | D5  | For GC9A01A only |
| SPI CS     | D8  | For GC9A01A only |
| SPI DC     | D3  | For GC9A01A only |
| SPI RST    | D4  | For GC9A01A only |
| Button     | D6  | Pull-up, active low |
| LED        | D0  | Status indicator |

**Note**: Button pin avoids SPI conflicts. Adjust pins as needed for your specific board.

## Project Structure
```
garden-monitor/
├── README.md
├── platformio.ini
├── src/
│   └── main.cpp                 # Main application logic
├── lib/
│   ├── Config/
│   │   └── Config.h             # Pin definitions & platform config
│   ├── SensorManager/
│   │   ├── SensorManager.h
│   │   └── SensorManager.cpp    # AHT sensor abstraction
│   ├── DisplayManager/
│   │   ├── DisplayManager.h
│   │   └── DisplayManager.cpp   # SSD1306/GC9A01A display abstraction
│   ├── WiFiManager/
│   │   ├── WiFiManager.h
│   │   └── WiFiManager.cpp      # WiFi connection management
│   ├── ButtonManager/
│   │   ├── ButtonManager.h
│   │   └── ButtonManager.cpp    # Button handling with debouncing
│   └── InfluxManager/
│       ├── InfluxManager.h
│       └── InfluxManager.cpp    # InfluxDB client with error handling
└── .gitignore
```

## Dependencies (Adafruit Libraries)
All dependencies are automatically managed by PlatformIO:

```ini
lib_deps =
    dlloydev/Toggle                              # Button debouncing
    adafruit/Adafruit AHTX0                     # AHT sensor support
    adafruit/Adafruit GFX Library               # Graphics primitives
    adafruit/Adafruit SSD1306                   # OLED display driver
    adafruit/Adafruit GC9A01A                   # Round TFT display driver
    tobiasschuerg/InfluxDB-Client-for-Arduino   # InfluxDB integration
```

## Configuration

### 1. Select Display Type
In `lib/Config/Config.h`, change the display type:
```cpp
// For SSD1306 OLED (default)
#define DISPLAY_TYPE DISPLAY_TYPE_SSD1306

// For GC9A01A round TFT
#define DISPLAY_TYPE DISPLAY_TYPE_GC9A01
```

### 2. WiFi Credentials
In `src/main.cpp`, update your WiFi networks:
```cpp
const std::vector<const char*> KNOWN_SSIDS = { "YourWiFi", "BackupWiFi" };
const std::vector<const char*> KNOWN_PASSWORDS = { "password1", "password2" };
```

### 3. InfluxDB Configuration
In `src/main.cpp`, configure your InfluxDB connection:
```cpp
const char* INFLUX_URL = "http://your-influxdb-server:8086";
const char* INFLUX_ORG = "your-organization";
const char* INFLUX_BUCKET = "your-bucket";
const char* INFLUX_TOKEN = "your-access-token";
```

## Getting Started

### 1. Install Development Environment
- Install [Visual Studio Code](https://code.visualstudio.com/)
- Install the [PlatformIO IDE extension](https://platformio.org/install/ide?install=vscode)

### 2. Clone and Setup
```bash
git clone https://github.com/yourusername/garden-monitor.git
cd garden-monitor
```

### 3. Open in VSCode
- Open VSCode
- File → Open Folder → Select the `garden-monitor` directory
- PlatformIO will automatically install dependencies

### 4. Configure Hardware
- Wire your components according to the pinout tables
- Select your display type in `Config.h`
- Update WiFi and InfluxDB credentials in `main.cpp`

### 5. Build and Upload
- Connect your ESP32/ESP8266 via USB
- Select your environment in PlatformIO (`esp32dev` or `esp8266dev`)
- Click "Build" to compile
- Click "Upload" to flash firmware
- Click "Monitor" to view serial output

## Display Features

### SSD1306 OLED (128x32 or 128x64)
- Monochrome display with high contrast
- Low power consumption
- Perfect for basic status information
- I2C interface (only 2 wires needed)

### GC9A01A Round TFT (240x240)
- Full-color circular display
- Beautiful graphics with multiple colors
- Temperature in red, humidity in blue
- Status messages with color coding
- SPI interface for fast updates

## InfluxDB Integration
- Automatic data logging every 10 seconds
- Device tagging for organization
- Error handling with status reporting
- Connection validation and retry logic
- Timestamps for accurate data correlation

## Button Interface
- Debounced button handling using Toggle library
- Visual feedback on display and LED
- Ready for future menu/mode implementation
- Pin selected to avoid SPI conflicts

## Serial Monitor Output
```
Garden Monitor v1.0.0
Initializing...
AHT sensor initialized
Display initialized
Connecting to WiFi...
WiFi connected successfully
InfluxDB connected successfully
Setup complete - entering main loop
Temp: 23.5°C, Humidity: 45.2%, WiFi: Connected, InfluxDB: Connected
```

## Troubleshooting

### Common Issues
1. **Display not working**: Check wiring and display type setting in `Config.h`
2. **Sensor not found**: Verify I2C connections (SDA/SCL pins)
3. **WiFi connection fails**: Check credentials and signal strength
4. **InfluxDB errors**: Verify server URL, credentials, and network connectivity

### Pin Conflicts
- SSD1306 uses I2C (no conflicts)
- GC9A01A uses SPI pins - ensure button doesn't use SPI pins
- LED pin should not conflict with other peripherals

## Future Enhancements
- [ ] Menu system with button navigation
- [ ] WiFi configuration portal
- [ ] Data visualization on display
- [ ] Multiple sensor support
- [ ] OTA (Over-The-Air) updates
- [ ] MQTT support as alternative to InfluxDB
- [ ] Battery power optimization
- [ ] Enclosure design files

## References
- [Adafruit AHTX0 Library](https://github.com/adafruit/Adafruit_AHTX0)
- [Adafruit GC9A01A Library](https://github.com/adafruit/Adafruit_GC9A01A)
- [Adafruit SSD1306 Library](https://github.com/adafruit/Adafruit_SSD1306)
- [InfluxDB Client for Arduino](https://github.com/tobiasschuerg/InfluxDB-Client-for-Arduino)

---

**Built with ❤️ using Adafruit libraries and PlatformIO** 