// #define MILLIS_USE_TIMERA0 0
// #define MILLIS_USE_TIMERB0 1
// #undef MILLIS_USE_TIMERA0
#include <Arduino.h>
#include <avr/sleep.h>

#include "NameTag.h"
#include "Animation.h"
#include "Twinkle.h"

using namespace NameTag;

AnimationManager<1> animator;

void setup() {
    NameTag::setup();

    auto t = new Twinkle();

    animator.addAnimation(t);
    animator.activate(0);
}

void loop() {
    animator.update();
}
