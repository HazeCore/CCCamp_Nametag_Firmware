#pragma once

#include <Arduino.h>
#include "lib8tion.h"
#include "NameTag.h"

void updatePixel(unsigned long time, uint8_t brightness) {
    using namespace NameTag;

    static uint16_t duration = 625;
    static unsigned long lastLedChange = 0;
    static uint8_t i = 0;

    uint16_t ledTime = time - lastLedChange;
    if (ledTime > duration) {
        i = random8(LED_COUNT + 8); // indexes larger than 8 will result in a downtime in the animation
        duration = random8() * 3 + 400;
        lastLedChange = time;
        ledTime = 0;
    }

    leds.fill(tinyNeoPixel::Color(1, 1, 3));

    if (i < LED_COUNT) {
        // calculate led with a sine wave going from dark to bright to dark again

        // inlining the map formula and cancelling out the zeros is actually larger that just using map
        fract8 p = map(ledTime, 0, duration, 0, 255);
        uint8_t v = cos8(p + 127);
        v = scale8(v, brightness >> 2);
        uint8_t d = scale8(v, 150);
        leds.setPixelColor(i, v, d, d);
    }

    leds.show();
};

void updatePanel(unsigned long time, uint8_t brightness) {
    using namespace NameTag;

    // get a value between 0 and 255 that increases over a span of 4095 milliseconds and then loops
    uint8_t progress = (time & 4095) >> 4;

    // get a nice sine wave for a smooth breathing effect
    uint8_t v = sin8(progress);
    // change the sine from a range of 0 to 255 to 50 to 255
    v = scale8(v, 205) + 50;
    v = scale8(v, brightness);
    setBlue(v);
    setRed(scale8(v, 150)); // red at a slightly lower brightness
};

void updateTwinkle(unsigned long time, uint8_t brightness) {
    brightness = dim8_lin(brightness);
    updatePixel(time, brightness);
    updatePanel(time, brightness);
};
