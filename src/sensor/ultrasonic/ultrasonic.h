//
// Created by Tymoteusz Biziuk on 21/04/2026.
//

#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>
#include "../ISensor.h"

// Użycie:
//   ultrasonic::init(26, 27);   // init(trigPin, echoPin)
//   ultrasonic::tick();         // w loop()
//   float cm = ultrasonic::getDistanceCm();

class ultrasonic : public ISensor {
public:
    static void init(int trigPin, int echoPin);
    void init() override { /* nie używać, użyj init(trig, echo) */ }
    static void tick_s();
    void tick() override { tick_s(); }
    static String getReport_s();
    String getReport() override { return getReport_s(); }

    static float getDistanceCm();
    static float getDistanceInch();

private:
    static int   _trigPin;
    static int   _echoPin;
    static float _distanceCm;
    static float _distanceInch;

    static constexpr float SOUND_SPEED = 0.034f;
    static constexpr float CM_TO_INCH  = 0.393701f;
};

#endif //ULTRASONIC_H