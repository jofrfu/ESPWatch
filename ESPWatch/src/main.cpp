//
// Created by Jonas Fuhrmann on 30.06.18.
//

#include <Arduino.h>
#include "Screen.h"
#include "ScreenQueue.h"
#include "Decoder.h"
#include "Dispatcher.h"
#include "BLEHandler.h"
#include <freertos/timers.h>

void setup() {
    Serial.begin(115200);
    delay(5000);

    auto screen = new Screen();
    auto screenQueue = new ScreenQueue(screen);
    auto decoder = new Decoder();
    auto dispatcher = new Dispatcher(decoder, screenQueue);
    auto bleHandler = new BLEHandler(dispatcher);

    pthread_t screenThread;

    if(pthread_create(&screenThread, nullptr, screenQueue->threadFunction, screenQueue)) {
        Serial.println("Error creating thread screenThread.");
    }

    TimerCallbackFunction_t callback = [screenQueue](TimerHandle_t xTimer){
        ScreenQueue::parameter_t parameter;
        parameter.type = ScreenQueue::UPDATE;
        screenQueue->addFunctionCall(parameter);
    };

    TimerHandle_t timer = xTimerCreate("ClockTimer", pdMS_TO_TICKS(60000), pdTRUE, (void*)0, callback);
    if(xTimerStart(timer, 10) != pdPASS) {
        Serial.println("Error starting timer.");
    }

    pthread_join(screenThread, nullptr);
}

void loop() {}
