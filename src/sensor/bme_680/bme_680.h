//
// Created by Tymoteusz Biziuk on 21/04/2026.
//

#ifndef BME680_H
#define BME680_H

#include <Arduino.h>
#include <Adafruit_BME680.h>
#include "../ISensor.h"

// Użycie:
//   bme680::init(5);    // init(csPin) — MOSI/MISO/SCK fixed SPI
//   bme680::tick();     // w loop()
//   float t = bme680::getTemperature();

class bme680 : public ISensor {
public:
    static void init(int csPin);
    void init() override { /* nie używać, użyj init(csPin) */ }
    static void tick_s();
    void tick() override { tick_s(); }
    static String getReport_s();
    String getReport() override { return getReport_s(); }

    static float getTemperature();
    static float getHumidity();
    static float getPressure();
    static bool  isDataValid();

private:
    static Adafruit_BME680* _bme;
    static float _temperature;
    static float _humidity;
    static float _pressure;
    static bool  _valid;
};

#endif //BME680_H