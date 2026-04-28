#include "mqtt_publisher.h"

WiFiClient   mqtt_publisher::_wifi;
PubSubClient mqtt_publisher::_client(_wifi);
const char*  mqtt_publisher::_ssid     = nullptr;
const char*  mqtt_publisher::_password = nullptr;
const char*  mqtt_publisher::_broker   = nullptr;
int          mqtt_publisher::_port     = 1883;

void mqtt_publisher::begin(const char* ssid, const char* password,
                            const char* broker, int port) {
    _ssid     = ssid;
    _password = password;
    _broker   = broker;
    _port     = port;

    _connectWifi();
    _client.setServer(_broker, _port);
    _connectMqtt();
}

void mqtt_publisher::_connectWifi() {
    Serial.printf("[MQTT] Connecting to WiFi: %s\n", _ssid);
    WiFi.begin(_ssid, _password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.printf("\n[MQTT] WiFi OK — IP: %s\n", WiFi.localIP().toString().c_str());
}

void mqtt_publisher::_connectMqtt() {
    while (!_client.connected()) {
        Serial.printf("[MQTT] Connecting to broker %s...\n", _broker);
        if (_client.connect("esp32")) {
            Serial.println("[MQTT] Broker connected");
        } else {
            Serial.printf("[MQTT] Failed (rc=%d), retry in 2s\n", _client.state());
            delay(2000);
        }
    }
}

const char* mqtt_publisher::_topic() {
    switch (sensor_manager::getActiveSensor()) {
        case SensorType::ULTRASONIC: return "iot/sensors/ultrasonic";
        case SensorType::RFID:       return "iot/sensors/rfid";
        case SensorType::BME680:     return "iot/sensors/bme680";
        case SensorType::PMS:        return "iot/sensors/pms";
        default:                     return "iot/sensors/unknown";
    }
}

String mqtt_publisher::_buildJson() {
    String json;
    switch (sensor_manager::getActiveSensor()) {
        case SensorType::ULTRASONIC:
            json = "{\"distance_cm\":"  + String(ultrasonic::getDistanceCm(), 2) +
                   ",\"distance_in\":"  + String(ultrasonic::getDistanceInch(), 2) + "}";
            break;
        case SensorType::RFID:
            json = "{\"card_present\":" + String(rfid_reader::isCardPresent() ? "true" : "false") +
                   ",\"uid\":\""        + rfid_reader::getUID() + "\"}";
            break;
        case SensorType::BME680:
            json = "{\"temperature\":"  + String(bme680::getTemperature(), 2) +
                   ",\"humidity\":"     + String(bme680::getHumidity(), 2) +
                   ",\"pressure\":"     + String(bme680::getPressure(), 2) + "}";
            break;
        case SensorType::PMS:
            json = "{\"pm1_0\":"  + String(pms::getPM1_0()) +
                   ",\"pm2_5\":"  + String(pms::getPM2_5()) +
                   ",\"pm10\":"   + String(pms::getPM10()) + "}";
            break;
        default:
            json = "{\"error\":\"no sensor\"}";
            break;
    }
    return json;
}

void mqtt_publisher::tick() {
    if (!_client.connected()) _connectMqtt();
    _client.loop();

    String payload = _buildJson();
    _client.publish(_topic(), payload.c_str());
    Serial.printf("[MQTT] -> %s : %s\n", _topic(), payload.c_str());
}