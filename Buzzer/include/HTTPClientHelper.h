#ifndef HTTPClientHelper_H
#define HTTPClientHelper_H

#include <WiFi.h>
#include <HTTPClient.h>

class HTTPClientHelper {
public:
    HTTPClientHelper();
    String GET(const char* serverName);
    String POST(const char* serverName, const String& payload);
};

#endif
