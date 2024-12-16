#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Pin Definitions
#define relayPin 26
#define encoderPin1 18
#define encoderPin2 19
#define encoderPushPin 5

// OLED Display Settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1  // Reset pin (not used)
#define SCREEN_ADDRESS 0x3C  // I2C address for the OLED display

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Variables for Rotary Encoder
int encoderPos = 0;
int encoderPin1Last = LOW;
int n = LOW;
int buttonState = HIGH;
int lastButtonState = HIGH;

// Oven State Variables
int temperature = 170;       // Initial temperature (Celsius)
int timeMinutes = 0;         // Time in minutes
int timeSeconds = 0;         // Time in seconds
int totalTimeSeconds = 0;    // Total time in seconds for countdown
unsigned long lastUpdateMillis = 0; // Tracks the last time the countdown was updated
bool isCountingDown = false; // Countdown flag
bool isWarmup = false;       // Warmup flag
int mode = 0;                // 0: Set Temperature, 1: Set Time, 2: Warmup, 3: Countdown
const int warmupTimeSeconds = 120; // Warmup duration (2 minutes)
int warmupRemainingSeconds = warmupTimeSeconds;

void setup() {
  pinMode(relayPin, OUTPUT);
  pinMode(encoderPin1, INPUT_PULLUP);
  pinMode(encoderPin2, INPUT_PULLUP);
  pinMode(encoderPushPin, INPUT_PULLUP);

  Serial.begin(115200);

  if (!display.begin(SSD1306_PAGEADDR, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.display();

  display.println(F("Oven Controller"));
  display.display();
  delay(1000);
  display.clearDisplay();
}

void loop() {
  // Read the rotary encoder
  n = digitalRead(encoderPin1);
  if ((encoderPin1Last == LOW) && (n == HIGH)) {
    if (digitalRead(encoderPin2) == LOW) {
      encoderPos--;
    } else {
      encoderPos++;
    }

    if (mode == 0) { // Setting temperature
      temperature = constrain(temperature + (encoderPos * 10), 170, 300);
    } else if (mode == 1) { // Setting time in minutes
      timeMinutes = max(0, timeMinutes + encoderPos); // Adjust minutes only
    }

    encoderPos = 0;
  }
  encoderPin1Last = n;

  // Handle button press
  buttonState = digitalRead(encoderPushPin);
  if (buttonState == LOW && lastButtonState == HIGH) {
    if (mode == 0) {
      mode = 1; // Switch to setting time
    } else if (mode == 1) {
      mode = 2; // Start warmup
      warmupRemainingSeconds = warmupTimeSeconds; // Reset warmup timer
      lastUpdateMillis = millis();
      isWarmup = true;
      digitalWrite(relayPin, HIGH); // Turn on relay for warmup
    }
  }
  lastButtonState = buttonState;

  // Warmup logic
  if (isWarmup) {
    unsigned long currentMillis = millis();

    // Check if 1 second has elapsed
    if (currentMillis - lastUpdateMillis >= 1000) {
      lastUpdateMillis = currentMillis; // Reset last update time
      warmupRemainingSeconds--;        // Decrement warmup time

      if (warmupRemainingSeconds <= 0) {
        // Warmup finished
        isWarmup = false;
        mode = 3; // Switch to countdown mode
        totalTimeSeconds = timeMinutes * 60; // Convert minutes to seconds
        lastUpdateMillis = millis(); // Reset timer for countdown
        isCountingDown = true; // Start countdown
      }
    }
  }

  // Countdown logic
  if (isCountingDown) {
    unsigned long currentMillis = millis();

    // Check if 1 second has elapsed
    if (currentMillis - lastUpdateMillis >= 1000) {
      lastUpdateMillis = currentMillis; // Reset last update time
      totalTimeSeconds--;              // Decrement total time

      if (totalTimeSeconds <= 0) {
        // Countdown finished
        isCountingDown = false;
        mode = 0; // Reset to temperature mode
        digitalWrite(relayPin, LOW); // Turn off relay
        timeMinutes = 0;
        timeSeconds = 0;
      } else {
        // Update remaining time
        timeMinutes = totalTimeSeconds / 60; // Calculate remaining minutes
        timeSeconds = totalTimeSeconds % 60; // Calculate remaining seconds
      }
    }
  }

  // Update OLED display
  display.clearDisplay();
  if (mode == 0) {
    display.setCursor(0, 0);
    display.println("Set Temperature:");
    display.setTextSize(2);
    display.setCursor(0, 20);
    display.print(temperature);
    display.println(" C");
    display.setTextSize(1);
  } else if (mode == 1) {
    display.setCursor(0, 0);
    display.println("Set Time:");
    display.setTextSize(2);
    display.setCursor(0, 20);
    display.print(timeMinutes);
    display.print(" min");
    display.setTextSize(1);
  } else if (mode == 2) {
    display.setCursor(0, 0);
    display.println("Warming Up:");
    display.setTextSize(2);
    display.setCursor(0, 20);
    display.print(warmupRemainingSeconds / 60);
    display.print(" min ");
    display.print(warmupRemainingSeconds % 60);
    display.println(" sec");
    display.setTextSize(1);
  } else if (mode == 3) {
    display.setCursor(0, 0);
    display.println("Time Remaining:");
    display.setTextSize(2);
    display.setCursor(0, 20);
    display.print(timeMinutes);
    display.print(" min ");
    display.print(timeSeconds);
    display.println(" sec");
    display.setTextSize(1);
  }
  display.display();
}
