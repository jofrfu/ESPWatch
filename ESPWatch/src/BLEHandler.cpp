//
// Created by Jonas Fuhrmann on 10.07.18.
//

#include <BLEDevice.h>
#include <Esp.h>
#include "BLEHandler.h"

#define SERVICE_UUID        "476d099e-1ff4-43c0-9e36-23f4280ec5f7"
#define CHARACTERISTIC_UUID "19f220b8-2adf-4c46-867c-c75b208ba652"

BLEHandler::BLEHandler(BLECharacteristicCallbacks *callbacks) {
    uint64_t MAC = ESP.getEfuseMac();
    char buffer[17];
    sprintf(buffer, "%016llx", MAC);
    std::string name = "ESPWatch" + std::string(buffer);
    BLEDevice::init(name);

    pServer = BLEDevice::createServer();
    pService = pServer->createService(SERVICE_UUID);
    pCharacteristic = pService->createCharacteristic(
            CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE
    );

    pCharacteristic->setCallbacks(callbacks);
    pService->start();

    pAdvertising = pServer->getAdvertising();
    pAdvertising->start();
}

void BLEHandler::sendMessage(String message) {
    pCharacteristic->setValue(message.c_str());
}
