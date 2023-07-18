#pragma once

#include <Arduino.h>
#include <tinyNeoPixel_Static.h>

namespace NameTag {
    extern tinyNeoPixel leds;

    void setup();
    // void delay(unsigned long long ms);
    void waitRTC();
    void waitPIT();
    void setRed(uint8_t);
    void setGreen(uint8_t);
    void setBlue(uint8_t);
}