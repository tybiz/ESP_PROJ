#pragma once
#include <Arduino.h>

struct HCSR04Data {
    float dist_cm;
    bool  ok;
};

class HCSR04Module {
public:
    explicit HCSR04Module(uint8_t trigPin = 5, uint8_t echoPin = 18);

    [[nodiscard]] bool       begin() const;
    [[nodiscard]] HCSR04Data read() const;

private:
    uint8_t _trig;
    uint8_t _echo;
};