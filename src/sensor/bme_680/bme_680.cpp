//
// Created by Tymoteusz Biziuk on 21/04/2026.
//

#include "bme_680.h"

Adafruit_BME680* bme680::_bme         = nullptr;
float            bme680::_temperature = 0.0f;
float            bme680::_humidity    = 0.0f;
float            bme680::_pressure    = 0.0f;
bool             bme680::_valid       = false;

void bme680::init(int csPin) {
    _bme = new Adafruit_BME680(csPin);
    if (!_bme->begin()) {
        Serial.println("[BME680] ERROR: Nie znaleziono sensora!");
        return;
    }
    _bme->setTemperatureOversampling(BME68X_OS_8X);
    _bme->setHumidityOversampling(BME68X_OS_2X);
    _bme->setPressureOversampling(BME68X_OS_4X);
    _bme->setIIRFilterSize(BME68X_FILTER_SIZE_3);
    Serial.println("[BME680] Initialized");
}

void bme680::tick_s() {
    if (!_bme) return;
    _valid = _bme->performReading();
    if (_valid) {
        _temperature = _bme->temperature;
        _humidity    = _bme->humidity;
        _pressure    = _bme->pressure / 100.0f;
        Serial.printf("[BME680] Temp: %.2f°C | Hum: %.2f%% | Pres: %.2f hPa\n",
                      _temperature, _humidity, _pressure);
    } else {
        Serial.println("[BME680] ERROR: Odczyt nieudany");
    }
}

String bme680::getReport_s() {
    return String("[BME680] ") + _temperature + "°C | " + _humidity + "% | " + _pressure + " hPa";
}

float bme680::getTemperature() { return _temperature; }
float bme680::getHumidity()    { return _humidity;    }
float bme680::getPressure()    { return _pressure;    }
bool  bme680::isDataValid()    { return _valid;       }