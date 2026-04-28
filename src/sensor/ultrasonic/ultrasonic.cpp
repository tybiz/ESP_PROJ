//
// Created by Tymoteusz Biziuk on 21/04/2026.
//

#include "ultrasonic.h"

int   ultrasonic::_trigPin      = -1;
int   ultrasonic::_echoPin      = -1;
float ultrasonic::_distanceCm   = 0.0f;
float ultrasonic::_distanceInch = 0.0f;

void ultrasonic::init(int trigPin, int echoPin) {
    _trigPin = trigPin;
    _echoPin = echoPin;
    pinMode(_trigPin, OUTPUT);
    pinMode(_echoPin, INPUT);
    Serial.println("[ULTRASONIC] Initialized");
}

void ultrasonic::tick_s() {
    digitalWrite(_trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(_trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(_trigPin, LOW);

    long duration  = pulseIn(_echoPin, HIGH);
    _distanceCm    = duration * SOUND_SPEED / 2.0f;
    _distanceInch  = _distanceCm * CM_TO_INCH;

    Serial.printf("[ULTRASONIC] %.2f cm / %.2f inch\n", _distanceCm, _distanceInch);
}

String ultrasonic::getReport_s() {
    return String("[ULTRASONIC] ") + _distanceCm + " cm / " + _distanceInch + " inch";
}

float ultrasonic::getDistanceCm()   { return _distanceCm;   }
float ultrasonic::getDistanceInch() { return _distanceInch; }