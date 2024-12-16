#include <Arduino.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include "JSONparse.h"
#include "HTTPClientHelper.h"

// Include the watchdog timer functionality
#include "esp_task_wdt.h"  // Include this for watchdog functions

Servo servo1;
Servo servo2;
Servo servo3;
JSONparse jsonCode;
HTTPClientHelper HTTP;

#define SERVO_PIN_1 16
#define SERVO_PIN_2 27
#define SERVO_PIN_3 14

const char* ssid = "NETLAB-OIL460";
const char* password = "Startsemester";

#define lowestOrderURL "http://192.168.155.31:8080/order/lowest"

unsigned long previousMillis = 0;
unsigned long interval = 5000;

void setup() {
  Serial.begin(115200);

  // Initialize the Watchdog Timer for task-based handling
  esp_task_wdt_init(5, true);  // 5 seconds timeout
  esp_task_wdt_add(NULL); // Add the current task (loop task) to the watchdog timer

  // WiFi connection setup
  WiFi.config(IPAddress(192, 168, 68, 211), IPAddress(192, 168, 68, 1), IPAddress(255, 255, 255, 0));
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
    delay(1000);
  }
  Serial.println("Connected to WiFi");

  // Attach servos to pins
  servo1.attach(SERVO_PIN_1);
  servo2.attach(SERVO_PIN_2);
  servo3.attach(SERVO_PIN_3);

  servo1.write(0);
  servo2.write(0); 
  servo3.write(0); 

  Serial.println("Servos initialized.");
}

void loop() {
  // Feed the watchdog to prevent reset
  esp_task_wdt_reset(); // Reset the watchdog timer

  if (WiFi.status() == WL_CONNECTED) {
    unsigned long currentMillis = millis();

    // Check if enough time has passed to get the next order
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      // Get the order data
      String response = HTTP.GET(lowestOrderURL);
      if (response.isEmpty()) {
        Serial.println("Empty response from server");
        servo1.write(0);
        servo2.write(0);
        servo3.write(0);
        return;
      }

      // Print the raw JSON response for debugging
      Serial.println("Response: " + response);

      // Parse JSON response for order_id
      String orderID = jsonCode.parse(response, "order_id");

      // Parse pizza toppings from the pizza object (directly access the keys inside "pizza")
      String cheeseStr = jsonCode.parse(response, "pizza.cheese");
      String pepperoniStr = jsonCode.parse(response, "pizza.pepperoni");
      String championsStr = jsonCode.parse(response, "pizza.champions");

      // Check for valid response from the parse function
      if (orderID == "Parsing failed" || orderID == "Key not found" ||
          cheeseStr == "Parsing failed" || cheeseStr == "Key not found" ||
          pepperoniStr == "Parsing failed" || pepperoniStr == "Key not found" ||
          championsStr == "Parsing failed" || championsStr == "Key not found") {
        Serial.println("Failed to parse JSON or missing fields");
          servo1.write(0);
        servo2.write(0);
        servo3.write(0);
        return;
      }

      // Debug output to check parsed values
      Serial.print("Order ID: "); Serial.println(orderID);
      Serial.print("Cheese: "); Serial.println(cheeseStr);
      Serial.print("Pepperoni: "); Serial.println(pepperoniStr);
      Serial.print("Champions: "); Serial.println(championsStr);

      // Convert parsed String to boolean
      bool cheese = (cheeseStr == "true");
      bool pepperoni = (pepperoniStr == "true");
      bool champions = (championsStr == "true");

      // Control servos based on toppings
      // Only update the servo position if the state has changed
      if (servo1.read() != (cheese ? 180 : 0)) {
        servo1.write(cheese ? 180 : 0);
      }
      if (servo2.read() != (pepperoni ? 180 : 0)) {
        servo2.write(pepperoni ? 180 : 0);
      }
      if (servo3.read() != (champions ? 180 : 0)) {
        servo3.write(champions ? 180 : 0);
      }
    }
  } else {
    Serial.println("WiFi Disconnected");
    // Try to reconnect after a delay
    delay(1000);
  }
}
