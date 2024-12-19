#include "JSONparse.h"
#include <Arduino_JSON.h>

JSONparse::JSONparse() {}

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

    // Handle dot notation for nested keys
    String keyStr(key);
    int dotIndex = keyStr.indexOf('.');
    
    // If there is a dot in the key, it means we're dealing with a nested object
    if (dotIndex != -1) {
        // Split the key into parent and child keys
        String parentKey = keyStr.substring(0, dotIndex);
        String childKey = keyStr.substring(dotIndex + 1);

        // Verify parent key exists
        if (!myObject.hasOwnProperty(parentKey.c_str())) {
            return "Key not found";
        }

        // Get the nested JSON object
        JSONVar nestedObject = myObject[parentKey.c_str()];
        if (JSON.typeof(nestedObject) != "object") {
            return "Key not found";
        }

        // Recursively parse the nested object for the child key
        JSONparse nestedParser;
        return nestedParser.parse(JSON.stringify(nestedObject), childKey.c_str());
    }

    // Handle top-level keys
    if (myObject.hasOwnProperty(key)) {
        // Convert the value into a String and return it
        return String(JSON.stringify(myObject[key]));  // Convert JSONVar to String
    }

    return "Key not found";
}
