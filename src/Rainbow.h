#pragma once

#include <Arduino.h>
#include "lib8tion.h"
#include "NameTag.h"

// 34, 41, 50, 50, 45, 41, 3, 3

#define pos(D) D // position modifications could be performed here
const uint8_t ledPositions[NameTag::LED_COUNT] = { pos(34), pos(41), pos(50), pos(50), pos(45), pos(41), pos(3), pos(3) };

void updateRainbow(unsigned long time, uint8_t brightness) {
    using namespace NameTag;

    brightness = min(brightness + 20, 100);

    uint16_t p = time << 3;

    for (uint8_t i = 0; i < NameTag::LED_COUNT; i++) {
        leds.setPixelColor(i, tinyNeoPixel::gamma32(tinyNeoPixel::ColorHSV(uint16_t(ledPositions[i])*256 + p, 255, brightness)));
    }

    brightness += 10;

    setPanelColor(tinyNeoPixel::gamma32(tinyNeoPixel::ColorHSV(pos(20)*256 + p, 255, brightness)));

    leds.show();
};