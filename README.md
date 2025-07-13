# Garden Monitor: ESP32-S3 + AHT Sensor + GC9A01 Display

Monitor temperature and humidity using an AHT sensor and display the readings on a GC9A01 round display, powered by an ESP32-S3. This project is beginner-friendly and uses PlatformIO in VSCode for easy development and uploading.

## Hardware Required
- ESP32-S3 board (e.g., LilyGo T-Display S3, or similar)
- AHT10/AHT20 temperature & humidity sensor (I2C)
- GC9A01 round TFT display (SPI)
- Jumper wires
- Breadboard (optional)

## Wiring Diagram
*To be added: Please refer to your board's pinout and sensor/display datasheets.*

## Getting Started

### 1. Install VSCode
Download and install [Visual Studio Code](https://code.visualstudio.com/).

### 2. Install PlatformIO Extension
- Open VSCode
- Go to Extensions (Ctrl+Shift+X)
- Search for "PlatformIO IDE" and install it

### 3. Clone This Repository
```
git clone https://github.com/yourusername/garden-monitor.git
cd garden-monitor
```

### 4. Open the Project in VSCode
- Open VSCode
- Go to File > Open Folder and select the `garden-monitor` folder

### 5. Install Dependencies
PlatformIO will automatically install all required libraries when you build the project.

### 6. Select Your Board
This project is configured for ESP32-S3. If you use a different ESP32-S3 variant, update the `platformio.ini` accordingly.

### 7. Build and Upload
- Connect your ESP32-S3 board via USB
- In VSCode, click the PlatformIO icon in the sidebar
- Click "Build" to compile
- Click "Upload" to flash the firmware
- Click "Monitor" to view serial output

## Project Structure
```
garden-monitor/
├── README.md
├── platformio.ini
├── src/
│   └── main.cpp
├── lib/
└── .gitignore
```

## Future Plans
- Add support for 0.91" I2C display
- Send data to InfluxDB
- Add WiFi configuration

---

*Happy hacking!* 