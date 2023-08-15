#pragma once

#include <Arduino.h>
#include <tinyNeoPixel_Static.h>

namespace NameTag {
    const uint8_t LED_COUNT = 8;
    extern tinyNeoPixel leds;

    void setup();
    bool isButtonPressed();
    void setRed(uint8_t);
    void setGreen(uint8_t);
    void setBlue(uint8_t);
    void setPanelColor(uint32_t);
    void setPanelColor(uint32_t, uint8_t);
    uint32_t gammaHSV(uint16_t, uint8_t);
}