#include "HTTPClientHelper.h"
#include <WiFiClient.h>
#include <HTTPClient.h>

HTTPClientHelper::HTTPClientHelper() {}



String HTTPClientHelper::GET(const char* serverName) {
    WiFiClient client;
    HTTPClient http;
    http.begin(client, serverName);

    int httpResponseCode = http.GET();
    String payload = "{}";

    if (httpResponseCode > 0) {
        payload = http.getString();
        if (payload.isEmpty()) {
            Serial.println("Empty response received");
            http.end();
            return "{}";  // Return a default value in case of empty response
        }
    } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
        http.end();
        return "{}";  // Return a default empty response on error
    }

    http.end();
    return payload;
}

// String HTTPClientHelper::GET(const char* serverName) {
//     WiFiClient client;
//     HTTPClient http;
//     http.begin(client, serverName);

//     int httpResponseCode = http.GET();
//     String payload = "{}";

//     if (httpResponseCode > 0) {
//         payload = http.getString();
//     } else {
//         Serial.print("Error code: ");
//         Serial.println(httpResponseCode);
//     }

//     http.end();
//     return payload;
// }

String HTTPClientHelper::POST(const char* serverName, const String& payload) {
    WiFiClient client;
    HTTPClient http;
    http.begin(client, serverName);

    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(payload);

    String response = "{}";
    if (httpResponseCode > 0) {
        response = http.getString();
    } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }

    http.end();
    return response;
}
