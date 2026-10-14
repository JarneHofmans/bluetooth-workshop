#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"

void setup() {
  Serial.begin(115200);
  BLEDevice::init("ESP32-NoCallbackJarne");

  // Server aanmaken ZONDER callbacks
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  BLEDevice::startAdvertising();

  Serial.println("ESP32 gestart en zendt uit (Advertising)...");
}

void loop() {
  delay(1000);
}