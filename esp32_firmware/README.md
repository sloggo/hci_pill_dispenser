# ESP32 Pill Dispenser Firmware

This firmware controls the pill dispenser hardware using an ESP32 microcontroller. It provides a web server interface that communicates with the main web application and controls servo motors for dispensing pills.

## Hardware Requirements

- ESP32 development board (e.g., ESP32 DevKit C)
- 3x Servo motors (e.g., SG90 or MG90S)
- Power supply for servo motors (5V)
- USB cable for programming
- Jumper wires and breadboard (optional)

## Pin Connections

- Servo 1: GPIO 13
- Servo 2: GPIO 12
- Servo 3: GPIO 14

## Setup Instructions

1. Install PlatformIO IDE in VS Code:
   - Open VS Code
   - Go to Extensions (Ctrl+Shift+X)
   - Search for "PlatformIO IDE"
   - Install the extension

2. Configure WiFi:
   - Open `src/main.cpp`
   - Update the WiFi credentials:
     ```cpp
     const char* ssid = "YOUR_WIFI_SSID";
     const char* password = "YOUR_WIFI_PASSWORD";
     ```

3. Install Dependencies:
   - PlatformIO will automatically install the required libraries:
     - ArduinoJson
     - ESPAsyncWebServer
     - AsyncTCP
     - ESP32Servo

## Flashing Instructions

1. Connect your ESP32 to your computer via USB cable

2. In VS Code:
   - Open the `esp32_firmware` folder
   - Wait for PlatformIO to initialize
   - Click the "Build" button (checkmark icon) to compile the firmware
   - Click the "Upload" button (right arrow icon) to flash the firmware

3. Monitor the output:
   - Click the "Serial Monitor" button (plug icon) to view debug output
   - The ESP32 will print its IP address when connected to WiFi

## Testing

1. After flashing, open the Serial Monitor to verify:
   - WiFi connection
   - IP address assignment
   - "Pill Dispenser Ready!" message

2. The ESP32 will create a web server that:
   - Listens for dispense requests on `/dispense`
   - Provides Server-Sent Events on `/events`
   - Controls servo motors based on received commands

## Troubleshooting

1. If the ESP32 doesn't connect to WiFi:
   - Check your WiFi credentials
   - Ensure the ESP32 is within range of your router
   - Try power cycling the ESP32

2. If servos don't move:
   - Check servo connections
   - Verify power supply
   - Check servo pin assignments in the code

3. If upload fails:
   - Press and hold the BOOT button while clicking EN
   - Release BOOT button after upload begins
   - Try a different USB cable or port 