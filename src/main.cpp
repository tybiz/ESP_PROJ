#include <Arduino.h>
#include "util/sensor_manager/sensor_manager.h"
#include "util/mqtt_publisher/mqtt_publisher.h"

static const char* WIFI_SSID  = "raspwifi";
static const char* WIFI_PASS  = "12345678";
static const char* MQTT_HOST  = "192.168.4.1";

void setup() {
    Serial.begin(115200);
    delay(1000);

    sensor_manager::begin();
    mqtt_publisher::begin(WIFI_SSID, WIFI_PASS, MQTT_HOST);
}

void loop() {
    sensor_manager::tick();
    mqtt_publisher::tick();
    delay(1000);
}