//
// Created by Tymoteusz Biziuk on 21/04/2026.
//

#include "rfid.h"

MFRC522* rfid_reader::_rfid        = nullptr;
bool     rfid_reader::_cardPresent = false;
String   rfid_reader::_uid         = "";

void rfid_reader::init(int ssPin, int rstPin) {
    SPI.begin();
    _rfid = new MFRC522(ssPin, rstPin);
    _rfid->PCD_Init();
    Serial.println("[RFID] RC522 gotowy — przyłóż kartę");
}

void rfid_reader::tick_s() {
    _cardPresent = false;

    if (!_rfid->PICC_IsNewCardPresent()) return;
    if (!_rfid->PICC_ReadCardSerial())   return;

    _cardPresent = true;
    _uid = "";

    for (byte i = 0; i < _rfid->uid.size; i++) {
        if (_rfid->uid.uidByte[i] < 0x10) _uid += "0";
        _uid += String(_rfid->uid.uidByte[i], HEX);
        if (i < _rfid->uid.size - 1) _uid += ":";
    }

    Serial.printf("[RFID] UID: %s\n", _uid.c_str());

    _rfid->PICC_HaltA();
    _rfid->PCD_StopCrypto1();
}

String rfid_reader::getReport_s() {
    return _cardPresent ? "[RFID] UID: " + _uid : "[RFID] Brak karty";
}

bool   rfid_reader::isCardPresent() { return _cardPresent; }
String rfid_reader::getUID()        { return _uid;         }