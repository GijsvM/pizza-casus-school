#ifndef JSONparse_H
#define JSONparse_H

#include <Arduino.h>

class JSONparse {
public:
    JSONparse();
    String parse(const String& jsonResponse, const char* key);
};

#endif
