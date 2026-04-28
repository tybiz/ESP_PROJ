//
// Created by Tymoteusz Biziuk on 21/04/2026.
//

#ifndef PMS_H
#define PMS_H

#include <Arduino.h>

// Użycie:
//   pms::init(16, 17);          // init(rxPin, txPin)
//   pms::tick();                // w loop(), odczytuje gdy dane dostępne
//   uint16_t pm = pms::getPM25();

class pms {
public:
    static void init(int rxPin, int txPin);
    static void tick();

    static uint16_t getPM1_0();
    static uint16_t getPM2_5();
    static uint16_t getPM10();
    static bool isDataValid();

private:
    struct PMS_DATA {
        uint16_t pm1_0;
        uint16_t pm2_5;
        uint16_t pm10;
    };

    static PMS_DATA _data;
    static bool     _valid;

    static bool _read();
};

#endif //PMS_H