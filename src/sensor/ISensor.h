//
// Created by Tymoteusz Biziuk on 21/04/2026.
//

#ifndef ISENSOR_H
#define ISENSOR_H

#include <Arduino.h>

class ISensor {
public:
    virtual void init() = 0;
    virtual void tick() = 0;
    virtual String getReport() = 0;
    virtual ~ISensor() = default;
};

#endif //ISENSOR_H