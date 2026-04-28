//
// Created by Tymoteusz Biziuk on 21/04/2026.
//

#include "pms.h"

static HardwareSerial pmsSerial(2);

pms::PMS_DATA pms::_data  = {0, 0, 0};
bool          pms::_valid = false;

void pms::init(int rxPin, int txPin) {
    pmsSerial.begin(9600, SERIAL_8N1, rxPin, txPin);
    delay(3000);
    Serial.println("[PMS] PMSA003-C gotowy");
}

void pms::tick() {
    _valid = _read();
    if (_valid) {
        Serial.printf("[PMS] PM1.0: %u | PM2.5: %u | PM10: %u µg/m³\n",
                      _data.pm1_0, _data.pm2_5, _data.pm10);
    }
}

bool pms::_read() {
    if (pmsSerial.available() < 32) return false;

    uint8_t buf[32];

    if (pmsSerial.read() != 0x42) return false;
    if (pmsSerial.read() != 0x4D) return false;

    for (int i = 2; i < 32; i++) {
        buf[i] = pmsSerial.read();
    }

    uint16_t checksum = 0x42 + 0x4D;
    for (int i = 2; i < 30; i++) checksum += buf[i];
    uint16_t received = (buf[30] << 8) | buf[31];
    if (checksum != received) return false;

    _data.pm1_0 = (buf[10] << 8) | buf[11];
    _data.pm2_5 = (buf[12] << 8) | buf[13];
    _data.pm10  = (buf[14] << 8) | buf[15];

    return true;
}

uint16_t pms::getPM1_0()    { return _data.pm1_0; }
uint16_t pms::getPM2_5()    { return _data.pm2_5; }
uint16_t pms::getPM10()     { return _data.pm10;  }
bool     pms::isDataValid() { return _valid;       }