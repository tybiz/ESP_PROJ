//
// Created by Tymoteusz Biziuk on 21/04/2026.
//

#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>
#include <Preferences.h>
#include "../../sensor/ISensor.h"

// Użycie:
//   sensor_manager::begin();   // w setup() — wczytuje z flash lub pyta przez serial
//   sensor_manager::tick();    // w loop()

enum class SensorType : uint8_t {
    NONE       = 0,
    ULTRASONIC = 1,
    RFID       = 2,
    BME680     = 3,
    PMS        = 4
};

class sensor_manager {
public:
    static void begin();
    static void tick();
    static SensorType getActiveSensor();

private:
    static void        _prompt();
    static void        _initSensor(SensorType type);
    static SensorType  _loadFromFlash();
    static void        _saveToFlash(SensorType type);

    static ISensor*   _sensor;
    static SensorType _type;
    static Preferences _prefs;

    static void _checkResetButton();
    static constexpr int PIN_BOOT_BTN    = 0;
    static constexpr uint32_t HOLD_MS   = 3000;
};

#endif //SENSOR_MANAGER_H