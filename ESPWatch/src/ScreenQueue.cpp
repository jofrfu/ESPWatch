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
    functionQueue.push(parameter);
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&condition);
}

void* ScreenQueue::threadFunction(void *) {
    while(true) {
        pthread_mutex_lock(&mutex);
        while(functionQueue.empty()) {
            pthread_cond_wait(&condition, &mutex);
        }

        parameter_t param = functionQueue.front();
        switch(param.type) {
            case UPDATE:
                pScreen->update();
                break;
            case SET_NOTIFICATION:
                pScreen->setNewestNotification(param.notification);
                break;
            case SHOW_ALL:
                pScreen->showAllNotifications();
                break;
            case REMOVE_NOTIFICATION:
                pScreen->removeNotification(param.removeId);
                break;
        }
    }
}
