#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

// Officiële Bluetooth SIG 16-bit UUID's
#define SERVICE_ENVIRONMENTAL_SENSING BLEUUID((uint16_t)0x181A)
#define CHAR_TEMPERATURE               BLEUUID((uint16_t)0x2A6E)
#define CHAR_HUMIDITY                  BLEUUID((uint16_t)0x2A6F)

BLECharacteristic *pTempCharacteristic;
BLECharacteristic *pHumCharacteristic;

void setup() {
  Serial.begin(115200);
  BLEDevice::init("ESP32-WeatherStation-Jarne");

  BLEServer *pServer = BLEDevice::createServer();

  // Maak de Environmental Sensing Service aan
  BLEService *pService = pServer->createService(SERVICE_ENVIRONMENTAL_SENSING);

  // Temperature Characteristic (Alleen READ rechten)
  pTempCharacteristic = pService->createCharacteristic(
                          CHAR_TEMPERATURE,
                          BLECharacteristic::PROPERTY_READ
                        );

  // Humidity Characteristic (Alleen READ rechten)
  pHumCharacteristic = pService->createCharacteristic(
                        CHAR_HUMIDITY,
                        BLECharacteristic::PROPERTY_READ
                      );

  pService->start();

  // Voeg service UUID toe aan advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_ENVIRONMENTAL_SENSING);
  pAdvertising->setScanResponse(true);
  BLEDevice::startAdvertising();

  Serial.println("BLE Weerstation actief. Wachten op verbinding...");
}

void loop() {
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 2000) {
    lastUpdate = millis();

    // Simuleer een temperatuur van 22.85 °C -> 2285 als int16_t (2 bytes)
    int16_t rawTemp = 2285 + random(-50, 50); 
    
    // Simuleer een luchtvochtigheid van 58.40 % -> 5840 als uint16_t (2 bytes)
    uint16_t rawHum = 5840 + random(-100, 100); 

    // Binaire data klaarzetten in het geheugen van de characteristic
    pTempCharacteristic->setValue((uint8_t*)&rawTemp, sizeof(rawTemp));
    pHumCharacteristic->setValue((uint8_t*)&rawHum, sizeof(rawHum));

    Serial.printf("Waarden in geheugen bijgewerkt -> Temp: %.2f °C | Hum: %.2f %%\n", rawTemp / 100.0, rawHum / 100.0);
  }
}