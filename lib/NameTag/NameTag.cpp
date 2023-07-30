
#include "NameTag.h"
#include "lib8tion.h"

using namespace NameTag;

#define NUMLEDS 8

byte pixels[8 * 3];
tinyNeoPixel NameTag::leds = tinyNeoPixel(NUMLEDS, PIN_PA3, NEO_GRB, pixels);
uint8_t brightness = 150;

void NameTag::setup() {
    takeOverTCA0(); // take over TCA0 so digitalWrite() on alt pins won't mess up alternate pin PWM output.
    TCA0.SINGLE.CTRLD = 1;    // Enable split mode.
    TCA0.SPLIT.LPER   = 254;  // or as required by your application. If 0 and 255 turn PWM off, you want to count to 254 not 255.
    TCA0.SPLIT.HPER   = 254;
    TCA0.SPLIT.CTRLA  = TCA_SPLIT_CLKSEL_DIV64_gc | TCA_SPLIT_ENABLE_bm; // same as the core configures by default
    PORTMUX.CTRLC     = PORTMUX_TCA00_ALTERNATE_gc;  /* 0/1-series - You can OR together as many alternate TCA0 groupcodes as needed */
    //PORTMUX.TCAROUTEA = PORTMUX_TCA00_ALT1_gc; /* 2-series has different, less stupid, names. */
    pinModeFast(PIN_PA0, OUTPUT);
    pinModeFast(PIN_PA1, OUTPUT);
    pinModeFast(PIN_PA2, OUTPUT);
    pinModeFast(PIN_PA3, OUTPUT);
    pinModeFast(PIN_PA6, INPUT_PULLUP);
    pinModeFast(PIN_PA7, OUTPUT);

    ADC0.CTRLA = 0; // disable ADC

    leds.begin();
    leds.fill(0);
    leds.show();
}

// delay by certain amount using nop when the clock speed is 20MHz
// without using millis
void NameTag::custom_delay(unsigned long long ms) {
    for (unsigned long long i = 0; i < ms; i++) {
        for (unsigned long long j = 0; j < 2500; j++) {
            asm volatile("nop");
        }
    }
}

void NameTag::waitRTC() {
    // Wait for all registers to be synchronized
    while (RTC.STATUS > 0) {};
}

void NameTag::waitPIT() {
    // Wait for all registers to be synchronized
    while (RTC.PITSTATUS > 0) {};
}

uint8_t NameTag::getBrightness() {
    return brightness;
}

void NameTag::setBrightness(uint8_t v) {
    brightness = v;
    v = tinyNeoPixel::gamma8(v);
    leds.setBrightness(v);
}

void NameTag::setBlue(uint8_t duty) {
    duty = scale8(duty, brightness);
    // WO0 equals A7 in this configuration
    if (duty == 0) {
        TCA0.SPLIT.CTRLB &= ~TCA_SPLIT_LCMP0EN_bm;  // Turn off PWM if passed   0 duty cycle
                                                    /* you probably also want to digitalWrite() or digitalWriteFast() the pin LOW */
        PORTA.OUTCLR = PIN7_bm; // set pin to low
    } else if (duty == 255) {
        TCA0.SPLIT.CTRLB &= ~TCA_SPLIT_LCMP0EN_bm;  // Turn off PWM if passed 255 duty cycle
                                                    /* you probably also want to digitalWrite() or digitalWriteFast() the pin HIGH */
        PORTA.OUTSET = PIN7_bm; // set pin to high
    } else {
        TCA0.SPLIT.LCMP0 = duty;                   // Turn set the duty cycle for WO0
        TCA0.SPLIT.CTRLB |= TCA_SPLIT_LCMP0EN_bm;  // Turn on PWM
    }
}

void NameTag::setGreen(uint8_t duty) {
    duty = scale8(duty, brightness);
    // WO1 equals A1 in this configuration
    if (duty == 0) {
        TCA0.SPLIT.CTRLB &= ~TCA_SPLIT_LCMP1EN_bm;  // Turn off PWM if passed   0 duty cycle
                                                    /* you probably also want to digitalWrite() or digitalWriteFast() the pin LOW */
        PORTA.OUTCLR = PIN1_bm; // set pin to low
    } else if (duty == 255) {
        TCA0.SPLIT.CTRLB &= ~TCA_SPLIT_LCMP1EN_bm;  // Turn off PWM if passed 255 duty cycle
                                                    /* you probably also want to digitalWrite() or digitalWriteFast() the pin HIGH */
        PORTA.OUTSET = PIN1_bm; // set pin to high
    } else {
        TCA0.SPLIT.LCMP1 = duty;                   // Turn set the duty cycle for WO1
        TCA0.SPLIT.CTRLB |= TCA_SPLIT_LCMP1EN_bm;  // Turn on PWM
    }
}

void NameTag::setRed(uint8_t duty) {
    duty = scale8(duty, brightness);
    // WO2 equals A2 in this configuration
    if (duty == 0) {
        TCA0.SPLIT.CTRLB &= ~TCA_SPLIT_LCMP2EN_bm;  // Turn off PWM if passed   0 duty cycle
                                                    /* you probably also want to digitalWrite() or digitalWriteFast() the pin LOW */
        PORTA.OUTCLR = PIN2_bm; // set pin to low
    } else if (duty == 255) {
        TCA0.SPLIT.CTRLB &= ~TCA_SPLIT_LCMP2EN_bm;  // Turn off PWM if passed 255 duty cycle
                                                    /* you probably also want to digitalWrite() or digitalWriteFast() the pin HIGH */
        PORTA.OUTSET = PIN2_bm; // set pin to high
    } else {
        TCA0.SPLIT.LCMP2 = duty;                   // Turn set the duty cycle for WO2
        TCA0.SPLIT.CTRLB |= TCA_SPLIT_LCMP2EN_bm;  // Turn on PWM
    }
}

void NameTag::setPanelColor(uint32_t color) {
    setRed(color >> 16);
    setGreen(color >> 8);
    setBlue(color);
}
