#pragma once
#include <Arduino.h>

struct PMSA003Data {
    uint16_t pm1_0;
    uint16_t pm2_5;
    uint16_t pm10;
    bool     ok;
};

class PMSA003Module {
public:
    explicit PMSA003Module(uint8_t rxPin = 16, uint8_t txPin = 17);

    bool        begin();
    PMSA003Data read();
    const char* name() const;

private:
    uint8_t        _rx;
    uint8_t        _tx;
    HardwareSerial _serial;
};