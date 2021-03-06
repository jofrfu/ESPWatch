//
// Created by Jonas Fuhrmann on 11.07.18.
//

#ifndef ESPWATCH_SCREENQUEUE_H
#define ESPWATCH_SCREENQUEUE_H

#include <Arduino.h>
#include <list>
#include "Screen.h"

class ScreenQueue {
public:
    enum function_type {UPDATE, SET_NOTIFICATION, SHOW_ALL, REMOVE_NOTIFICATION};
    struct parameter_t {
        function_type type{};
        Screen::notification_t notification{};
        int32_t removeId{};
    };
    ScreenQueue(Screen *screen);
    void addFunctionCall(parameter_t &parameter);
    static void* threadFunction(void* param);
private:
    Screen *pScreen;
    std::list<parameter_t> functionQueue;
    pthread_mutex_t mutex;
    pthread_cond_t condition;
};


#endif //ESPWATCH_SCREENQUEUE_H
