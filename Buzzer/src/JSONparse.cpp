#include "JSONparse.h"
#include <Arduino_JSON.h>

JSONparse::JSONparse() {
    // Constructor
}

String JSONparse::parse(const String& jsonResponse, const char* key) {
    // Check for empty or invalid response
    if (jsonResponse == "{}" || jsonResponse == "") {
        return "Empty response";
    }

    // Parse the JSON string
    JSONVar myObject = JSON.parse(jsonResponse);
    if (JSON.typeof(myObject) == "undefined") {
        return "Parsing failed";
    }

    // Check if the key exists
    if (myObject.hasOwnProperty(key)) {
        // Handle the value based on its type
        JSONVar value = myObject[key];

        if (JSON.typeof(value) == "null") {
            return "Null value";
        } else if (JSON.typeof(value) == "boolean") {
            return value ? "true" : "false";
        } else if (JSON.typeof(value) == "number") {
            return String((int)value); // Convert number to string
        } else if (JSON.typeof(value) == "string") {
            return (const char*)value;
        } else {
            return "Unsupported value type";
        }
    }

    return "Key not found";
}
