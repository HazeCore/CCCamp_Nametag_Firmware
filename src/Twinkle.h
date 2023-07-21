#pragma once

#include <Arduino.h>
#include "lib8tion.h"
#include "Animation.h"
#include "NameTag.h"

struct Twinkle : public Animation {

    Twinkle() {}

    void update(unsigned long time) override {
        updatePixel(time);
        updatePanel(time);
    };

    void updatePixel(unsigned long time) {
        using namespace NameTag;

        const uint8_t BRIGHTNESS = 50;

        static uint16_t duration = 625;
        static unsigned long lastLedChange = 0;
        static uint8_t i = 0;

        const uint16_t ledTime = time - lastLedChange;
        if (ledTime > duration) {
            i = random8(16);
            duration = random16(400, 1000);
            lastLedChange = time;
        }

        leds.fill(tinyNeoPixel::Color(1, 1, 3));

        if (i < 8) {
            fract8 p = map(ledTime % duration, 0, duration, 0, 255);
            uint8_t v = cos8(p + 127);
            v = scale8(v, BRIGHTNESS);
            leds.setPixelColor(i, scale8(v, 255), scale8(v, 150), scale8(v, 150));
        }

        leds.show();
    };

    void updatePanel(unsigned long time) {
        using namespace NameTag;

        static uint16_t DURATION = 5000;

        uint8_t progress = map(time % DURATION, 0, DURATION, 0, 255);

        uint8_t v = sin8(progress);
        v = map(v, 0, 255, 50, 255);
        setBlue(v);
        setRed(scale8(v, 150));
    };
};