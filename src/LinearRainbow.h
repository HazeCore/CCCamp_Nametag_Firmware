#pragma once

#include <Arduino.h>
#include "lib8tion.h"
#include "Animation.h"
#include "NameTag.h"

// 34, 41, 50, 50, 45, 41, 3, 3

#define pos(D) uint8_t(D)
const uint8_t ledPositions[] = { pos(34), pos(41), pos(50), pos(50), pos(45), pos(41), pos(3), pos(3) };

struct LinearRainbow : public Animation {

    LinearRainbow() {}

    void update(unsigned long time) override {
        using namespace NameTag;

        const uint16_t DURATION = 10000;
        uint16_t p = map(time % DURATION, 0, DURATION, 0, 65535);

        for (uint8_t i = 0; i < 8; i++) {
            leds.setPixelColor(i, tinyNeoPixel::gamma32(tinyNeoPixel::ColorHSV(uint16_t(ledPositions[i])*256 + p)));
            // leds.setPixelColor(i, tinyNeoPixel::ColorHSV(uint16_t(ledAngles[i])*256 + p));
        }

        setPanelColor(tinyNeoPixel::gamma32(tinyNeoPixel::ColorHSV(pos(20)*256 + p)));
        // setPanelColor(tinyNeoPixel::Color(0, 0, 0));

        leds.show();
    };
};