#ifndef MQTT_PUBLISHER_H
#define MQTT_PUBLISHER_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "../sensor_manager/sensor_manager.h"
#include "../../sensor/ultrasonic/ultrasonic.h"
#include "../../sensor/rfid/rfid.h"
#include "../../sensor/bme_680/bme_680.h"
#include "../../sensor/pms/pms.h"

class mqtt_publisher {
public:
    static void begin(const char* ssid, const char* password,
                      const char* broker, int port = 1883);
    static void tick();

private:
    static void   _connectWifi();
    static void   _connectMqtt();
    static String _buildJson();
    static const char* _topic();

    static WiFiClient   _wifi;
    static PubSubClient _client;
    static const char*  _ssid;
    static const char*  _password;
    static const char*  _broker;
    static int          _port;
};

#endif