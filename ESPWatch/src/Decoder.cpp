//
// Created by Jonas Fuhrmann on 10.07.18.
//

#include "Decoder.h"

#define NOTIFICATION_TYPE "notification"
#define TIME_TYPE "time"
#define REMOVE_TYPE "remove"

Decoder::Decoder() : buffer() {}

void Decoder::decode(String &message, struct parsed_t *result) {
    JsonObject &root = buffer.parseObject(message);
    if (!root.success()) {
        Serial.println("Failed to parse object: " + message);
        result->type = NONE;
        buffer.clear();
        return;
    }

    String type = root["type"];
    if (type.equals(NOTIFICATION_TYPE)) {
        result->type = NOTIFICATION;
        result->notification.message = root["message"].as<String>();
        result->notification.id = root["id"];
    } else if (type.equals(TIME_TYPE)) {
        result->type = TIME;
        result->time = root["time"];
    } else if (type.equals(REMOVE_TYPE)) {
        result->type = REMOVE;
        result->removeId = root["id"];
    } else {
        Serial.println("Invalid type parsed!");
        result->type = NONE;
    }

    buffer.clear();
}
