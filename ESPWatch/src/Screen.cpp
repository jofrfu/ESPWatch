//
// Created by Jonas Fuhrmann on 10.07.18.
//

#include "Screen.h"
#include <sys/time.h>
#include <GxGDEW0154Z04/GxGDEW0154Z04.cpp>
#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>

#define CLOCK_WIDTH 200
#define CLOCK_HEIGHT 180

#define NOTIFICATION_WIDTH 200
#define NOTIFICATION_HEIGHT 20

#define NOTIFICATION_START 180

Screen::Screen()
    : io(SPI, /*CS=5*/ SS, /*DC=*/ 22, /*RST=*/ 21)
    , display(io, /*RST=*/ 21, /*BUSY=*/ 4)
    , notificationList() {

    Serial.println("Screen() entered.");
    display.init(115200); // enable diagnostic output on Serial
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    display.setFont(&FreeMonoBold18pt7b);
    display.setCursor(0, 100);
    display.println(" Welcome!");
    display.update();
    Serial.println("Screen() exited.");
}

void Screen::update() {
    Serial.println("update() entered.");
    struct timeval tv{};

    if(gettimeofday(&tv, nullptr)) {
        Serial.println("Couldn't get time of day.");
        return;
    }

    time_t seconds = tv.tv_sec;
    time_t minutes = seconds / 60;
    time_t hours = minutes / 60;
    minutes -= hours * 60;

    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    display.setFont(&FreeMonoBold24pt7b);
    display.setCursor(0, CLOCK_HEIGHT/2);
    display.printf(" %02ld", (long)hours);
    display.setTextColor(GxEPD_RED);
    display.print(":");
    display.setTextColor(GxEPD_BLACK);
    display.printf("%02ld", (long)minutes);
    // ------------------------------------
    if(!notificationList.empty()) {
        display.drawFastHLine(0, NOTIFICATION_START-NOTIFICATION_HEIGHT, 200, GxEPD_RED);
        display.drawFastHLine(0, NOTIFICATION_START-NOTIFICATION_HEIGHT+1, 200, GxEPD_RED);
        notification_t newest = notificationList.back();
        display.setCursor(0, NOTIFICATION_START-5);
        display.setFont(&FreeMonoBold9pt7b);
        display.setTextColor(GxEPD_DARKGREY);
        display.print(newest.message);
    }

    display.update();
    Serial.println("update() exited.");
}

void Screen::setNewestNotification(notification_t &notification) {
    notificationList.push_back(notification);
    update();
}

void Screen::showAllNotifications() {

}

void Screen::removeNotification(int32_t id) {
    for(auto iterator = notificationList.begin(); iterator != notificationList.end(); ++iterator) {
        if((*iterator).id == id) {
            boolean updateScreen = false;
            if(next(iterator) == notificationList.end()) {
                updateScreen = true;
            }
            notificationList.erase(iterator);
            if(updateScreen) update();
            return;
        }
    }
}
