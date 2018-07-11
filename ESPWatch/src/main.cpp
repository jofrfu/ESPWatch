//
// Created by Jonas Fuhrmann on 30.06.18.
//

#include <Arduino.h>
#include <freertos/timers.h>
#include "Screen.h"
#include "ScreenQueue.h"
#include "Decoder.h"
#include "Dispatcher.h"
#include "BLEHandler.h"

ScreenQueue *screenQueue;

void setup() {
    Serial.begin(115200);
    delay(5000);

    auto screen = new Screen();
    screenQueue = new ScreenQueue(screen);
    auto decoder = new Decoder();

    auto callback = [](TimerHandle_t xTimer){
        ScreenQueue::parameter_t parameter;
        parameter.type = ScreenQueue::UPDATE;
        screenQueue->addFunctionCall(parameter);
    };

    TimerHandle_t timer = xTimerCreate("MinuteTimer", pdMS_TO_TICKS(60000), pdTRUE, (void*)0, callback);

    auto dispatcher = new Dispatcher(decoder, screenQueue, timer);
    auto bleHandler = new BLEHandler(dispatcher);

    pthread_t screenThread;

    if(pthread_create(&screenThread, nullptr, screenQueue->threadFunction, screenQueue)) {
        Serial.println("Error creating thread screenThread.");
    }

    if(xTimerStart(timer, 0) != pdPASS) {
        Serial.println("Error starting MinuteTimer.");
    }

    pthread_join(screenThread, nullptr);
    while(true); // won't reach this statement - just in case something goes wrong
}

void loop() {}
