#pragma once

#include <Arduino.h>
#include "lib8tion.h"
#include "NameTag.h"

void updateStaticColorPanel(unsigned long time, uint8_t brightness, uint8_t hue) {
    using namespace NameTag;

    setPanelColor(tinyNeoPixel::gamma32(tinyNeoPixel::ColorHSV(hue*256, 255, brightness)));
}

void updateStaticColorPanel(unsigned long time, uint8_t brightness) {
    uint16_t hue = Store::getPersonalHue();

    updateStaticColorPanel(time, brightness, hue);
}

void updateStaticColorAll(unsigned long time, uint8_t brightness, uint8_t hue) {
    using namespace NameTag;

    setPanelColor(tinyNeoPixel::gamma32(tinyNeoPixel::ColorHSV(hue*256, 255, max(brightness, 25))));
    leds.fill(tinyNeoPixel::gamma32(tinyNeoPixel::ColorHSV(hue*256, 255, max(scale8(brightness, 200), 25))));
    leds.show();
}

void updateStaticColorAll(unsigned long time, uint8_t brightness) {
    uint16_t hue = Store::getPersonalHue();

    updateStaticColorAll(time, brightness, hue);
}