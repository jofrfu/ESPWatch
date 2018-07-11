//
// Created by Jonas Fuhrmann on 10.07.18.
//

#ifndef ESPWATCH_SCREEN_H
#define ESPWATCH_SCREEN_H

#include <GxEPD.h>
#include <GxGDEW0154Z04/GxGDEW0154Z04.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <list>

class Screen {
public:
    struct notification_t {
        String message;
        int32_t id;
    };
    Screen();
    void update();
    void setNewestNotification(notification_t &notification);
    void showAllNotifications();
    void removeNotification(int32_t id);
private:
    GxIO_Class io;
    GxEPD_Class display;
    std::list<notification_t> notificationList;
};


#endif //ESPWATCH_SCREEN_H
