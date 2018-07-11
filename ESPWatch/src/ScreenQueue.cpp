//
// Created by Jonas Fuhrmann on 11.07.18.
//

#include <pthread.h>
#include "ScreenQueue.h"

ScreenQueue::ScreenQueue(Screen *screen)
    : pScreen(screen)
    , functionQueue() {
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&condition, nullptr);
}

void ScreenQueue::addFunctionCall(ScreenQueue::parameter_t &parameter) {
    pthread_mutex_lock(&mutex);
    Serial.println("Entered critical path <addFunctionCall>");
    functionQueue.push_back(parameter);
    pthread_cond_signal(&condition);
    pthread_mutex_unlock(&mutex);
    Serial.println("Exited critical path <addFunctionCall>");
}

void* ScreenQueue::threadFunction(void *param) {
    Serial.println("Screen thread started.");
    auto thisObject = (ScreenQueue*)param;
    while(true) {
        pthread_mutex_lock(&thisObject->mutex);
        while(thisObject->functionQueue.empty()) {
            Serial.println("Waiting for signaling.");
            pthread_cond_wait(&thisObject->condition, &thisObject->mutex);
        }
        Serial.println("Queue isn't empty - lock acquired.");

        parameter_t parameter = thisObject->functionQueue.front();
        thisObject->functionQueue.pop_front();
        pthread_mutex_unlock(&thisObject->mutex);
        switch(parameter.type) {
            case UPDATE:
                thisObject->pScreen->update();
                break;
            case SET_NOTIFICATION:
                thisObject->pScreen->setNewestNotification(parameter.notification);
                break;
            case SHOW_ALL:
                thisObject->pScreen->showAllNotifications();
                break;
            case REMOVE_NOTIFICATION:
                thisObject->pScreen->removeNotification(parameter.removeId);
                break;
        }
    }
}
