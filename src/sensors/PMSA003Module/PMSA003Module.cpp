#include "PMSA003Module.h"

PMSA003Module::PMSA003Module(uint8_t rxPin, uint8_t txPin)
    : _rx(rxPin), _tx(txPin), _serial(2) {}

bool PMSA003Module::begin() {
    _serial.begin(9600, SERIAL_8N1, _rx, _tx);
    delay(3000);
    return true;
}

PMSA003Data PMSA003Module::read() {
    if (_serial.available() < 32)   return {0, 0, 0, false};
    if (_serial.read() != 0x42)     return {0, 0, 0, false};
    if (_serial.read() != 0x4D)     return {0, 0, 0, false};

    uint8_t buf[32];
    buf[0] = 0x42;
    buf[1] = 0x4D;
    for (int i = 2; i < 32; i++) buf[i] = _serial.read();

    uint16_t checksum = 0;
    for (int i = 0; i < 30; i++) checksum += buf[i];
    uint16_t received = (buf[30] << 8) | buf[31];
    if (checksum != received) return {0, 0, 0, false};

    return {
        (uint16_t)((buf[10] << 8) | buf[11]),
        (uint16_t)((buf[12] << 8) | buf[13]),
        (uint16_t)((buf[14] << 8) | buf[15]),
        true
    };
}

const char* PMSA003Module::name() const { return "PMSA003"; }