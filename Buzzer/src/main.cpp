#include <Arduino.h>
#include <WiFi.h>
#include <Arduino_JSON.h>
#include "TrafficLight.h"
#include "Buzzer.h"
#include "HTTPClientHelper.h"
#include "JSONparse.h"

// WiFi credentials
// const char* ssid = "NETLAB-OIL460";
// const char* password = "Startsemester";

const char* ssid = "Netlab-OIL430";
const char* password = "DesignChallenge";

// Server URL
const char* serverURL = "http://192.168.155.31:8080/order/2";

// Pins
#define LEDpin 23
#define numPixels 3
// Variables for timing
unsigned long previousMillis = 0;
unsigned long interval = 5000; // 5 seconds interval between actions

// WiFi connection timeout
unsigned long wifiTimeout = 15000; // 15 seconds timeout for WiFi connection
unsigned long startAttemptTime;

// Components
TrafficLight myTrafficLight(LEDpin, numPixels);
HTTPClientHelper HTTP;
JSONparse jsonCode;

String status;
unsigned long orderStatus;

void setup() {
    Serial.begin(115200);
    Serial.println("Starting ESP...");
    
    WiFi.config(IPAddress(192,168,68,210),IPAddress(192,168,68,1),IPAddress(255,255,255,0));
    WiFi.begin(ssid, password);
    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 15000) {
        Serial.println("Connecting to WiFi...");
        delay(1000);
    }

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Failed to connect to WiFi");
    } else {
        Serial.println("Connected to WiFi");
    }
}

void loop() {
    // Check if WiFi is connected
    if (WiFi.status() == WL_CONNECTED) {
        // Get the current time
        unsigned long currentMillis = millis();

        // Perform HTTP request only if enough time has passed
        if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;  // Save the last time you did the action

            String response = HTTP.GET(serverURL);
            if (response.isEmpty()) {
                Serial.println("Empty response from server");
                return;
            }

            status = jsonCode.parse(response, "status");
            if (status == "Parsing failed" || status.isEmpty()) {
                Serial.println("Failed to parse JSON or empty status");
                return;
            }

            orderStatus = status.toInt();
            Serial.print("Order status: ");
            Serial.println(orderStatus);

            switch (orderStatus) {
                case 0:
                    Serial.println(myTrafficLight.setColor("red"));
                    break;
                case 1:
                    Serial.println(myTrafficLight.setColor("yellow"));
                    break;
                case 2:
                    Serial.println(myTrafficLight.setColor("green"));
                    break;
                default:
                    Serial.println("Unknown status");
            }
        }
    } else {
        Serial.println("WiFi Disconnected");
    }
}