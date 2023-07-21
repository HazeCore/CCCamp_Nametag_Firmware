#pragma once

#include <Arduino.h>
#include "lib8tion.h"
#include "Animation.h"
#include "NameTag.h"

// 15°, 25°, 33°, 145°, 145°, 145°, 210°, 330°

#define degree(D) uint8_t(D*(255.0 / 360.0))
const uint8_t ledAngles[] = { degree(15), degree(25), degree(33), degree(145), degree(145), degree(145), degree(210), degree(330) };

struct Rainbow : public Animation {

    Rainbow() {}

    void update(unsigned long time) override {
        using namespace NameTag;

        const uint16_t DURATION = 10000;
        uint16_t p = map(time % DURATION, 0, DURATION, 0, 65535);

        for (uint8_t i = 0; i < 8; i++) {
            leds.setPixelColor(i, tinyNeoPixel::gamma32(tinyNeoPixel::ColorHSV(uint16_t(ledAngles[i])*256 + p)));
            // leds.setPixelColor(i, tinyNeoPixel::ColorHSV(uint16_t(ledAngles[i])*256 + p));
        }

        // setPanelColor(tinyNeoPixel::ColorHSV(degree(270)*256 + p));
        setPanelColor(tinyNeoPixel::Color(0, 0, 0));

        leds.show();
    };
};