#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

struct RC522Data {
    uint8_t uid[7];
    uint8_t uid_len;
    bool    ok;

    String uidString() const;
};

class RC522Module {
public:
    explicit RC522Module(uint8_t ssPin = 5, uint8_t rstPin = 22);

    bool       begin();
    RC522Data  read();
    const char* name() const;

private:
    MFRC522 _rfid;
};