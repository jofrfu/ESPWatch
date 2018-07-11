//
// Created by Jonas Fuhrmann on 30.06.18.
//

#include <Arduino.h>
#include <Screen.h>
#include <sys/time.h>
#include "Decoder.h"
#include "Dispatcher.h"
#include "BLEHandler.h"

void timerInterrupt();
void* threadFunction(void*);

volatile boolean doUpdate;
portMUX_TYPE timerMUX = portMUX_INITIALIZER_UNLOCKED;

pthread_mutex_t mutex;
pthread_cond_t condition;

void setup() {
    Serial.begin(115200);

    auto timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, timerInterrupt, true);
    timerAlarmWrite(timer, 60000000, true);
    timerAlarmEnable(timer);


}

void* threadFunction(void*) {
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&condition, nullptr);
    doUpdate = false;

    while(true) {
        pthread_mutex_lock(&mutex);
        while(!doUpdate) {
            pthread_cond_wait(&condition, &mutex);
        }

        // TODO: add update
        portENTER_CRITICAL(&timerMUX);
        doUpdate = false;
        portEXIT_CRITICAL(&timerMUX);
    }
}

void IRAM_ATTR timerInterrupt() {
    portENTER_CRITICAL(&timerMUX);
    doUpdate = true;
    portEXIT_CRITICAL(&timerMUX);
    pthread_cond_signal(&condition);
}

void loop() {}
