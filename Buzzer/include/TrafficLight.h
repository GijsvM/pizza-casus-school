#ifndef TrafficLight_H
#define TrafficLight_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class TrafficLight {
private:
    Adafruit_NeoPixel pixels;
public:
    TrafficLight(int pin, int numPixels);
    int setColor(String color);
};

#endif
