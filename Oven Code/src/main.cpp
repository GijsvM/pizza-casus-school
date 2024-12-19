#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include "JSONparse.h"
#include "HTTPClientHelper.h"

#define relayPin 26
#define encoderPin1 18
#define encoderPin2 19
#define encoderPushPin 5
#define tempPin 25
#define DHTTYPE DHT11 

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 
#define SCREEN_ADDRESS 0x3C 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(tempPin, DHTTYPE);
JSONparse jsonCode;
HTTPClientHelper HTTP;

const char* ssid = "NETLAB-OIL460";
const char* password = "Startsemester";

#define serverURL "http://192.168.155.31:8080/oven"

int encoderPos = 0;
int encoderPin1Last = LOW;
int n = LOW;
int buttonState = HIGH;
int lastButtonState = HIGH;


int temperature = 170;       
int timeMinutes = 0;         
int timeSeconds = 0;        
int totalTimeSeconds = 0;    
unsigned long lastUpdateMillis = 0; 
bool isCountingDown = false; 
bool isWarmup = false;      
int mode = 0;                
const int warmupTimeSeconds = 120;
int warmupRemainingSeconds = warmupTimeSeconds;

void setup() {
  pinMode(relayPin, OUTPUT);
  pinMode(encoderPin1, INPUT_PULLUP);
  pinMode(encoderPin2, INPUT_PULLUP);
  pinMode(encoderPushPin, INPUT_PULLUP);

  Serial.begin(115200);
  
  WiFi.config(IPAddress(192, 168, 68, 211), IPAddress(192, 168, 68, 1), IPAddress(255, 255, 255, 0));
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
    delay(1000);
  }
  Serial.println("Connected to WiFi");

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
  dht.begin();
}

void loop() {
  float temp = dht.readTemperature();

  n = digitalRead(encoderPin1);
  if ((encoderPin1Last == LOW) && (n == HIGH)) {
    if (digitalRead(encoderPin2) == LOW) {
      encoderPos--;
    } else {
      encoderPos++;
    }

    if (mode == 0) { 
      temperature = constrain(temperature + (encoderPos * 10), 170, 300);
    } else if (mode == 1) {
      timeMinutes = max(0, timeMinutes + encoderPos);
    }

    encoderPos = 0;
  }
  encoderPin1Last = n;


  buttonState = digitalRead(encoderPushPin);
  if (buttonState == LOW && lastButtonState == HIGH) {
    if (mode == 0) {
      mode = 1;
    } else if (mode == 1) {
      mode = 2;
      warmupRemainingSeconds = warmupTimeSeconds;
      lastUpdateMillis = millis();
      isWarmup = true;
      Serial.println(HTTP.POST(serverURL, "{\"status\": 1, \"minutesRemaining\": " + String(timeMinutes) + " , \"secondsRemaining\": " + String(timeSeconds) + ", \"setTemp\": " + String(temperature) + ", \"realTemp\": " + String(temp) + "}"));
      digitalWrite(relayPin, HIGH);
    }
  }
  lastButtonState = buttonState;


  if (isWarmup) {
    unsigned long currentMillis = millis();

    if (currentMillis - lastUpdateMillis >= 1000) {
      lastUpdateMillis = currentMillis; 
      warmupRemainingSeconds--;
      Serial.println(HTTP.POST(serverURL, "{\"status\": 1, \"minutesRemaining\": " + String(timeMinutes) + " , \"secondsRemaining\": " + String(timeSeconds) + ", \"setTemp\": " + String(temperature) + ", \"realTemp\": " + String(temp) + "}"));


      if (warmupRemainingSeconds <= 0) {
  
        isWarmup = false;
        mode = 3; 
        totalTimeSeconds = timeMinutes * 60;
        lastUpdateMillis = millis();
        isCountingDown = true; 
      }
    }
  }

  if (isCountingDown) {
    unsigned long currentMillis = millis();

    if (currentMillis - lastUpdateMillis >= 1000) {
      lastUpdateMillis = currentMillis; 
      totalTimeSeconds--; 
      Serial.println(HTTP.POST(serverURL, "{\"status\": 1, \"minutesRemaining\": " + String(timeMinutes) + " , \"secondsRemaining\": " + String(timeSeconds) + ", \"setTemp\": " + String(temperature) + ", \"realTemp\": " + String(temp) + "}"));
            

      if (totalTimeSeconds <= 0) {
        isCountingDown = false;
        mode = 0; 
        digitalWrite(relayPin, LOW); 
        timeMinutes = 0;
        timeSeconds = 0;
      } else {
    
        timeMinutes = totalTimeSeconds / 60; 
        timeSeconds = totalTimeSeconds % 60;
      }
    }
  }

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
