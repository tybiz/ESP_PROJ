#include "RC522Module.h"

RC522Module::RC522Module(uint8_t ssPin, uint8_t rstPin)
    : _rfid(ssPin, rstPin) {}

bool RC522Module::begin() {
    SPI.begin();
    _rfid.PCD_Init();
    return true;
}

RC522Data RC522Module::read() {
    if (!_rfid.PICC_IsNewCardPresent()) return {{}, 0, false};
    if (!_rfid.PICC_ReadCardSerial())   return {{}, 0, false};

    RC522Data data;
    data.uid_len = _rfid.uid.size;
    for (byte i = 0; i < data.uid_len; i++)
        data.uid[i] = _rfid.uid.uidByte[i];
    data.ok = true;

    _rfid.PICC_HaltA();
    _rfid.PCD_StopCrypto1();

    return data;
}

String RC522Data::uidString() const {
    String s;
    for (uint8_t i = 0; i < uid_len; i++) {
        if (uid[i] < 0x10) s += "0";
        s += String(uid[i], HEX);
        if (i < uid_len - 1) s += ":";
    }
    s.toUpperCase();
    return s;
}

const char* RC522Module::name() const { return "RC522"; }