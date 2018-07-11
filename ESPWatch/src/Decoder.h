//
// Created by Jonas Fuhrmann on 10.07.18.
//

#ifndef ESPWATCH_DECODER_H
#define ESPWATCH_DECODER_H

#include <ArduinoJson.h>
#include "Screen.h"

class Decoder {
public:
    enum parse_type {
        NOTIFICATION, TIME, REMOVE, NONE
    };
    struct parsed_t {
        parse_type type{};
        Screen::notification_t notification{};
        time_t time{};
        int32_t removeId{};
    };

    Decoder();
    void decode(String &message, struct parsed_t *result);
private:
    DynamicJsonBuffer buffer;
};


#endif //ESPWATCH_DECODER_H
