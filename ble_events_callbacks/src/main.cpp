#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"

// Server Callback klasse aanmaken
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) override {
      Serial.println("[EVENT] Client is verbonden!");
    }

    void onDisconnect(BLEServer* pServer) override {
      Serial.println("[EVENT] Client is ontkoppeld! Advertising wordt direct herstart...");
      pServer->startAdvertising(); // Belangrijk: radio-antenne weer aanzetten!
    }
};

void setup() {
  Serial.begin(115200);
  BLEDevice::init("ESP32-WithCallback");

  BLEServer *pServer = BLEDevice::createServer();
  
  // Registreer de callback bij de server
  pServer->setCallbacks(new MyServerCallbacks());

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