//
// Created by Tymoteusz Biziuk on 21/04/2026.
//

#ifndef RFID_H
#define RFID_H

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include "../ISensor.h"

// Użycie:
//   rfid_reader::init(15, 22);          // init(ssPin, rstPin)
//   rfid_reader::tick();                // w loop()
//   if (rfid_reader::isCardPresent()) {
//       String uid = rfid_reader::getUID();
//   }

class rfid_reader : public ISensor {
public:
    static void init(int ssPin, int rstPin);
    void init() override { /* nie używać, użyj init(ss, rst) */ }
    static void tick_s();
    void tick() override { tick_s(); }
    static String getReport_s();
    String getReport() override { return getReport_s(); }

    static bool   isCardPresent();
    static String getUID();

private:
    static MFRC522* _rfid;
    static bool     _cardPresent;
    static String   _uid;
};

#endif //RFID_H