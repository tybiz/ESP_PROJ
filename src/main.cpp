#include "sensors/HCSR04Module/HCSR04Module.h"
#include "sensors/PMSA003Module/PMSA003Module.h"
#include "sensors/RC522Module/RC522Module.h"

HCSR04Module  hcsr(5, 18);
PMSA003Module pms(16, 17);
RC522Module   rfid(5, 22);

void setup() {
    Serial.begin(115200);
    hcsr.begin();
    pms.begin();
    rfid.begin();
}

void loop() {
    auto dist = hcsr.read();
    if (dist.ok) Serial.printf("Odległość: %.1f cm\n", dist.dist_cm);

    auto air = pms.read();
    if (air.ok) Serial.printf("PM2.5: %d µg/m³\n", air.pm2_5);

    auto card = rfid.read();
    if (card.ok) Serial.printf("Karta: %s\n", card.uidString().c_str());

    delay(1000);
}