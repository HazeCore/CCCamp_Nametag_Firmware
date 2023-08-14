#pragma once

#include <Arduino.h>
#include "lib8tion.h"
#include "NameTag.h"

void updateCamp(unsigned long time, uint8_t brightness) {
    using namespace NameTag;

    brightness = dim8_lin(brightness);

    setPanelColor(tinyNeoPixel::gamma32(tinyNeoPixel::Color(0xFB, 0x48, 0xC4)), brightness);

    brightness = brightness >> 2;

    // rocket
    uint8_t b = random8(2, brightness);
    leds.setPixelColor(0, b, b >> 2, 0);
    b = random8(2, brightness);
    leds.setPixelColor(1, b, b >> 2, 0);

    // house
    leds.setPixelColor(4, tinyNeoPixel::Color(0, 0, scale8(sin8(time>>4), brightness)+1));
    leds.setPixelColor(5, tinyNeoPixel::Color(0, scale8(sin8(time>>3), brightness)+1, 0));

    leds.show();
};
