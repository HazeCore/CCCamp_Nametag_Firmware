#pragma once
#include <Arduino.h>

namespace Store {
    void setup();

    void setPersonalHue(uint8_t);
    uint8_t getPersonalHue();
    void setAnimation(uint8_t);
    uint8_t getAnimation();
    void setBrightness(uint8_t);
    uint8_t getBrightness();
}