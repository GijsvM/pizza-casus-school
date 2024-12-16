// TrafficLight.cpp
#include "TrafficLight.h"
#include <Adafruit_NeoPixel.h>

TrafficLight::TrafficLight(int pin, int numPixels) {
    // Initialize Adafruit NeoPixel object only once in the constructor
    pixels = Adafruit_NeoPixel(numPixels, pin, NEO_GRB + NEO_KHZ800);
    pixels.begin();
}

int TrafficLight::setColor(String color) {
    if (color == "green") {
        pixels.setPixelColor(0, pixels.Color(0, 255, 0));
        pixels.setPixelColor(1, pixels.Color(0, 255, 0));
        pixels.setPixelColor(2, pixels.Color(0, 255, 0));
        pixels.show();
        return 1;
    } else if (color == "yellow") {
        pixels.setPixelColor(0, pixels.Color(255, 69, 0));
        pixels.setPixelColor(1, pixels.Color(255, 69, 0));
        pixels.setPixelColor(2, pixels.Color(0, 0, 0));
        pixels.show();
        return 2;
    } else if (color == "red") {
        pixels.setPixelColor(0, pixels.Color(255, 0, 0));
        pixels.setPixelColor(1, pixels.Color(0, 0, 0));
        pixels.setPixelColor(2, pixels.Color(0, 0, 0));
        pixels.show();
        return 3;
    }
    return -1; // Return error if color is not recognized
}
