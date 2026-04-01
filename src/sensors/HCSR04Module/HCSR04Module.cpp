#include "HCSR04Module.h"

#define SOUND_SPEED 0.034f

HCSR04Module::HCSR04Module(const uint8_t trigPin, const uint8_t echoPin)
    : _trig(trigPin), _echo(echoPin) {}

bool HCSR04Module::begin() const {
    pinMode(_trig, OUTPUT);
    pinMode(_echo, INPUT);
    return true;
}

HCSR04Data HCSR04Module::read() const {
    digitalWrite(_trig, LOW);
    delayMicroseconds(2);
    digitalWrite(_trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(_trig, LOW);

    const long duration = pulseIn(_echo, HIGH, 30000);
    if (duration == 0) return {0, false};

    const float cm   = duration * SOUND_SPEED / 2.0f;

    return {cm, true};
}
