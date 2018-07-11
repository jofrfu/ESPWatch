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

void timerCallback(TimerHandle_t xTimer);

ScreenQueue *screenQueue;

void setup() {
    Serial.begin(115200);
    delay(5000);

    auto screen = new Screen();
    screenQueue = new ScreenQueue(screen);
    auto decoder = new Decoder();
    auto dispatcher = new Dispatcher(decoder, screenQueue);
    auto bleHandler = new BLEHandler(dispatcher);

    pthread_t screenThread;

    if(pthread_create(&screenThread, nullptr, screenQueue->threadFunction, screenQueue)) {
        Serial.println("Error creating thread screenThread.");
    }

    TimerHandle_t timer = xTimerCreate("ClockTimer", pdMS_TO_TICKS(60000), pdTRUE, (void*)0, timerCallback);
    if(xTimerStart(timer, 10) != pdPASS) {
        Serial.println("Error starting timer.");
    }

    pthread_join(screenThread, nullptr);
}

void timerCallback(TimerHandle_t xTimer) {
    ScreenQueue::parameter_t parameter;
    parameter.type = ScreenQueue::UPDATE;
    screenQueue->addFunctionCall(parameter);
}

void loop() {}
