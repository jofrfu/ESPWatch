//
// Created by Jonas Fuhrmann on 11.07.18.
//

#include "Dispatcher.h"
#include "ScreenQueue.h"
#include "Decoder.h"
#include <sys/time.h>

Dispatcher::Dispatcher(Decoder *decoder, ScreenQueue *queue)
    : pDecoder(decoder)
    , pQueue(queue) {}

void Dispatcher::onWrite(BLECharacteristic *pCharacteristic) {
    String message(pCharacteristic->getValue().c_str());

    if(message.charAt(0) == '{') {
        packet = message;
    } else {
        packet += message;
    }

    if(packet.charAt(packet.length()-1) == '}') {
        decode();
    }
}

void Dispatcher::decode() {
    Decoder::parsed_t decoded;
    pDecoder->decode(packet, &decoded);

    struct timeval tv{};
    ScreenQueue::parameter_t param;
    switch(decoded.type) {
        case Decoder::NONE:
            Serial.println("Unknown message from BLEHandler.");
            return;
        case Decoder::NOTIFICATION:
            Serial.println("New notification is:<message>:" + decoded.notification.message + "<id>:" + String(decoded.notification.id));
            param.type = ScreenQueue::SET_NOTIFICATION;
            param.notification = decoded.notification;
            break;
        case Decoder::TIME:
            tv.tv_usec = 0;
            tv.tv_sec = decoded.time;
            Serial.printf("New time is %ld\n", tv.tv_sec);
            settimeofday(&tv, nullptr);
            param.type = ScreenQueue::UPDATE;
            break;
        case Decoder::REMOVE:
            Serial.printf("New remove id is %d\n", decoded.removeId);
            param.type = ScreenQueue::REMOVE_NOTIFICATION;
            param.removeId = decoded.removeId;
            break;
    }
    pQueue->addFunctionCall(param);
}
