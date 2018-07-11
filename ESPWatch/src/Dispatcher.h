//
// Created by Jonas Fuhrmann on 11.07.18.
//

#ifndef ESPWATCH_DISPATCHER_H
#define ESPWATCH_DISPATCHER_H

#include <BLECharacteristic.h>
#include <WString.h>
#include <freertos/timers.h>

class Decoder;
class ScreenQueue;

class Dispatcher : public BLECharacteristicCallbacks {
public:
    Dispatcher(Decoder *decoder, ScreenQueue *queue, TimerHandle_t minuteTimer);
    void onWrite(BLECharacteristic *pCharacteristic) override;
private:
    Decoder *pDecoder;
    ScreenQueue *pQueue;
    String packet;
    void decode();
    static TimerHandle_t pMinuteTimer;
    static void resetCallback(TimerHandle_t xTimer);
};


#endif //ESPWATCH_DISPATCHER_H
