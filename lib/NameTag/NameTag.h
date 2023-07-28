#pragma once

#include <Arduino.h>
#include <tinyNeoPixel_Static.h>

namespace NameTag {
    extern tinyNeoPixel leds;

    void setup();
    void custom_delay(unsigned long long ms);
    void waitRTC();
    void waitPIT();
    uint8_t getBrightness();
    void setBrightness(uint8_t);
    void setRed(uint8_t);
    void setGreen(uint8_t);
    void setBlue(uint8_t);
    void setPanelColor(uint32_t);
}