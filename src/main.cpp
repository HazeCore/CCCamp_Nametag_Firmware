#include <Arduino.h>
#include <EEPROM.h>
#include <avr/sleep.h>
#include <Array.h>

#include "NameTag.h"
#include "Store.h"
#include "Twinkle.h"
#include "Rainbow.h"
#include "Camp.h"
#include "StaticColor.h"

// We are limiting the brightness to reduce power consumption.
// At full brightness the badge would only lasts about 30 minutes.
// Be careful when adjusting these, simply setting the maximum and minimum brightness to 255 and 0 respectively doesn't work
// as some animations add and subtract constant values which would then result in over- or underflows.
const uint8_t MAX_BRIGHTNESS = 127;
const uint8_t MIN_BRIGHTNESS = 30;
const unsigned int ANIMATION_COUNT = 5;
void (*animations[ANIMATION_COUNT])(unsigned long, uint8_t) = {updateRainbow, updateTwinkle, updateCamp, updateStaticColorPanel, updateStaticColorAll};
size_t currentAnimation = 0;
unsigned long startTime = 0; // time when the animation started
uint8_t brightness = 64;
unsigned long internalMillis = 0;

void checkButton();
unsigned long rough_millis();

void setup() {
    NameTag::setup();
    Store::setup();
    
    // if (RSTCTRL.RSTFR & RSTCTRL_BORF_bm) {
    //     // RSTCTRL.RSTFR |= RSTCTRL_BORF_bm;
    //     // MCU was reset due to a brownout, we will go into power down sleep mode.
    //     // We do this to prevent flickering that can happen in a reset loop.
    //     // Turning power off and on again will start up like normal.
    //     // This doesn't seem to work though :(.
    //     set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    //     sleep_enable();
    // }

    uint8_t anim = Store::getAnimation();
    if (anim >= ANIMATION_COUNT) {
        anim = 0;
    }
    currentAnimation = anim;

    brightness = Store::getBrightness();
    if (brightness <= MIN_BRIGHTNESS || brightness >= MAX_BRIGHTNESS) brightness = 64;

    uint8_t hue = Store::getPersonalHue();
    if (NameTag::isButtonPressed()) {
        currentAnimation = 3; // index of the static color animation
    }
    while (NameTag::isButtonPressed()) {
        NameTag::setPanelColor(NameTag::gammaHSV(hue*256, 255));
        hue += 1;

        delay(30);
    }
    Store::setPersonalHue(hue);

    // Configure the Periodic Interrupt Timer (PIT) to trigger an interrupt every ~32 ms
    noInterrupts();
    while (RTC.STATUS & RTC_CTRLABUSY_bm) {};
    RTC.CTRLA = RTC_RUNSTDBY_bm | RTC_PRESCALER_DIV1_gc | RTC_RTCEN_bm;
    RTC.CLKSEL = RTC_CLKSEL_INT1K_gc; // Use the internal 1kHz clock
    while (RTC.PITSTATUS & RTC_CTRLBUSY_bm) {};
    // RTC.PITCTRLA = RTC_PERIOD_CYC1024_gc; // 1 Hz
    RTC.PITCTRLA = RTC_RUNSTDBY_bm | RTC_PERIOD_CYC32_gc; // ~32 Hz period
    RTC.PITINTCTRL = RTC_PI_bm; // Enable PIT interrupt
    while (RTC.PITSTATUS & RTC_CTRLBUSY_bm) {};
    RTC.PITCTRLA |= RTC_PITEN_bm;
    interrupts();
}

volatile bool showFrame = false;

void loop() {
    // enter IDLE sleep between frames
    // it saves about 2mA
    TCB0.INTCTRL = 0; // disable Timer B interrupt (used by millis and delay)
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();
    sleep_cpu();
    sleep_disable();
    TCB0.INTCTRL |= 1; // reenable Timer B interrupt

    internalMillis += 32; // increment our own millis timer

    checkButton();

    animations[currentAnimation](rough_millis() - startTime, brightness);
}

void checkButton() {
    static bool lastBtnState = false; // true = pressed
    static unsigned long lastBtnChange = 0; // time when the last state change occurred
    static unsigned long lastBtnPress = 0; // time when the button was pressed last
    unsigned long timeSincePress = rough_millis() - lastBtnPress;

    bool state = NameTag::isButtonPressed();
    if (state != lastBtnState && rough_millis() - lastBtnChange > 100) { // debounce
        lastBtnChange = rough_millis();
        lastBtnState = state;

        if (lastBtnState) {
            // update if the button is pressed
            lastBtnPress = rough_millis();
        } else if (timeSincePress < 3000) {
            // the button was released and the press occurred less than 3 seconds ago
            // change the animation
            currentAnimation++;
            if (currentAnimation >= ANIMATION_COUNT) currentAnimation = 0;
            NameTag::setPanelColor(0);
            NameTag::leds.fill(0);
            NameTag::leds.show();
            startTime = rough_millis();
            Store::setAnimation(currentAnimation);
        } else {
            // the button was released and the press occurred more than 3 seconds ago 
            // the brightness was adjusted, save it
            Store::setBrightness(brightness);
        }
    }

    if (lastBtnState && timeSincePress > 3000) {
        if (timeSincePress <= 3050) {
            NameTag::setPanelColor(0);
            NameTag::leds.fill(0);
            NameTag::leds.show();
            delay(30);
        }
        // If the button is hold since more than 3 seconds, change the brightness
        static uint8_t dir = -1;
        brightness += dir;
        if (brightness <= MIN_BRIGHTNESS || brightness >= MAX_BRIGHTNESS) {
            dir = -dir;
            // blink once we reach the peak
            NameTag::setPanelColor(0);
            NameTag::leds.fill(0);
            NameTag::leds.show();
            delay(30);
        }
    }
}

unsigned long rough_millis() {
    return internalMillis;
}

ISR(RTC_PIT_vect) {
    RTC.PITINTFLAGS = RTC_PI_bm;
}
