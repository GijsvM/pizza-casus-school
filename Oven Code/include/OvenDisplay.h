#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

class Timer {
public:
    JSONparse();
    String parse(const String& jsonResponse, const char* key);
};

#endif
