//
// Created by Jonas Fuhrmann on 10.07.18.
//

#ifndef ESPWATCH_BLEHANDLER_H
#define ESPWATCH_BLEHANDLER_H

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEService.h>
#include <BLEAdvertising.h>
#include <BLECharacteristic.h>
#include <WString.h>

class BLEHandler {
public:
    BLEHandler(BLECharacteristicCallbacks *callbacks);
    void sendMessage(String message);
private:
    BLEServer *pServer;
    BLEService *pService;
    BLECharacteristic *pCharacteristic;
    BLEAdvertising *pAdvertising;
};


#endif //ESPWATCH_BLEHANDLER_H
