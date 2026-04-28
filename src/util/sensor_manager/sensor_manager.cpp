//
// Created by Tymoteusz Biziuk on 21/04/2026.
//

#include "sensor_manager.h"

#include "../../sensor/ultrasonic/ultrasonic.h"
#include "../../sensor/rfid/rfid.h"
#include "../../sensor/bme_680/bme_680.h"

ISensor*   sensor_manager::_sensor = nullptr;
SensorType sensor_manager::_type   = SensorType::NONE;
Preferences sensor_manager::_prefs;

// ── piny dla ESP32 DevKit V1 ──────────────────────────────────────────────────
// SPI (wspólne dla RFID i BME680 — nie podłączaj jednocześnie!):
//   SCK  -> GPIO18  (pin 30 na płytce, oznaczony SCK/CLK)
//   MISO -> GPIO19  (pin 31, oznaczony MISO/SDO)
//   MOSI -> GPIO23  (pin 37, oznaczony MOSI/SDI)
//
// RFID RC522:
//   SDA(SS) -> GPIO15  (pin 23, oznaczony D15)
//   RST     -> GPIO4   (pin 24, oznaczony D4)
//   GND     -> GND
//   3.3V    -> 3V3
//   IRQ     -> niepodłączony
//
// BME680 (SPI):
//   CS      -> GPIO21  (pin 33, oznaczony D21)
//   GND     -> GND
//   3.3V    -> 3V3
//
// HC-SR04 Ultrasonic:
//   TRIG    -> GPIO26  (pin 11, oznaczony D26)
//   ECHO    -> GPIO27  (pin 12, oznaczony D27)
//   VCC     -> VIN (5V) lub 3V3 (zależy od modułu)
//   GND     -> GND
// ─────────────────────────────────────────────────────────────────────────────

static constexpr int PIN_ULTRASONIC_TRIG = 26;
static constexpr int PIN_ULTRASONIC_ECHO = 27;
static constexpr int PIN_RFID_SS         = 15;
static constexpr int PIN_RFID_RST        = 4;
static constexpr int PIN_BME680_CS       = 21;

void sensor_manager::begin() {
    _checkResetButton();  // check before loading flash

    SensorType saved = _loadFromFlash();

    if (saved != SensorType::NONE) {
        Serial.printf("[SENSOR_MGR] Wczytano z flash: %d\n", (int)saved);
        _initSensor(saved);
        return;
    }

    _prompt();
}

void sensor_manager::_checkResetButton() {
    pinMode(PIN_BOOT_BTN, INPUT_PULLUP);

    if (digitalRead(PIN_BOOT_BTN) != LOW) return;  // not held, skip

    Serial.println("[SENSOR_MGR] Boot button trzymany — czekam 3s na reset...");

    uint32_t start = millis();
    while (digitalRead(PIN_BOOT_BTN) == LOW) {
        if (millis() - start >= HOLD_MS) {
            _saveToFlash(SensorType::NONE);
            Serial.println("[SENSOR_MGR] Flash wyczyszczony! Wybierz sensor.");
            return;
        }
        delay(10);
    }

    // released before 3s
    Serial.println("[SENSOR_MGR] Za krótko — reset anulowany.");
}

void sensor_manager::_prompt() {
    Serial.println("\n[SENSOR_MGR] Wybierz czujnik:");
    Serial.println("  1 - HC-SR04 Ultrasonic");
    Serial.println("  2 - RC522 RFID");
    Serial.println("  3 - BME680");

    while (!Serial.available()) delay(10);
    char c = Serial.read();

    SensorType chosen = SensorType::NONE;
    if      (c == '1') chosen = SensorType::ULTRASONIC;
    else if (c == '2') chosen = SensorType::RFID;
    else if (c == '3') chosen = SensorType::BME680;
    else {
        Serial.println("[SENSOR_MGR] Nieznany wybór, domyślnie Ultrasonic.");
        chosen = SensorType::ULTRASONIC;
    }

    _saveToFlash(chosen);
    _initSensor(chosen);
}

void sensor_manager::_initSensor(SensorType type) {
    _type = type;
    switch (type) {
        case SensorType::ULTRASONIC:
            ultrasonic::init(PIN_ULTRASONIC_TRIG, PIN_ULTRASONIC_ECHO);
            _sensor = new ultrasonic();
            Serial.println("[SENSOR_MGR] Aktywny: Ultrasonic");
            break;
        case SensorType::RFID:
            rfid_reader::init(PIN_RFID_SS, PIN_RFID_RST);
            _sensor = new rfid_reader();
            Serial.println("[SENSOR_MGR] Aktywny: RFID");
            break;
        case SensorType::BME680:
            bme680::init(PIN_BME680_CS);
            _sensor = new bme680();
            Serial.println("[SENSOR_MGR] Aktywny: BME680");
            break;
        default:
            Serial.println("[SENSOR_MGR] Brak sensora.");
            break;
    }
}

void sensor_manager::tick() {
    if (_sensor) _sensor->tick();
}

SensorType sensor_manager::getActiveSensor() { return _type; }

SensorType sensor_manager::_loadFromFlash() {
    _prefs.begin("sensor_mgr", true);
    uint8_t val = _prefs.getUChar("type", 0);
    _prefs.end();
    return static_cast<SensorType>(val);
}

void sensor_manager::_saveToFlash(SensorType type) {
    _prefs.begin("sensor_mgr", false);
    _prefs.putUChar("type", static_cast<uint8_t>(type));
    _prefs.end();
    Serial.printf("[SENSOR_MGR] Zapisano do flash: %d\n", (int)type);
}