#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Servo.h>

// WiFi credentials
const char* ssid = "YOUR_WIFI_SSID";      // Replace with your WiFi name
const char* password = "YOUR_WIFI_PASSWORD"; // Replace with your WiFi password

// Servo configuration
const int SERVO_PINS[] = {13, 12, 14};  // GPIO pins for servo motors
const int NUM_SERVOS = 3;
Servo servos[NUM_SERVOS];

// Servo positions
const int CLOSED_POSITION = 0;
const int OPEN_POSITION = 90;

void setupWiFi() {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("\nWiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

void setupServos() {
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    
    for (int i = 0; i < NUM_SERVOS; i++) {
        servos[i].setPeriodHertz(50);
        servos[i].attach(SERVO_PINS[i], 500, 2400);
        servos[i].write(CLOSED_POSITION);
        delay(500);
    }
}

void testServos() {
    // Test each servo
    for (int i = 0; i < NUM_SERVOS; i++) {
        Serial.print("Testing Servo ");
        Serial.println(i + 1);
        
        // Open
        servos[i].write(OPEN_POSITION);
        delay(1000);
        
        // Close
        servos[i].write(CLOSED_POSITION);
        delay(1000);
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\nESP32 Pill Dispenser Starting...");
    
    setupServos();
    Serial.println("Servos initialized");
    
    setupWiFi();
    
    // Test servos after setup
    Serial.println("Testing servos...");
    testServos();
    
    Serial.println("Setup complete!");
}

void loop() {
    // For now, just test servos every 10 seconds
    testServos();
    delay(10000);
} 