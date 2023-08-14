#include <EEPROM.h>
#include "NameTag.h"
#include "lib8tion.h"

using namespace NameTag;

byte pixels[8 * 3];
tinyNeoPixel NameTag::leds = tinyNeoPixel(LED_COUNT, PIN_PA3, NEO_GRB, pixels);

void NameTag::setup() {
    // Setup PWM on timer A (TCA0)
    takeOverTCA0(); // Take over the timer from it's default use by the Arduino core. It also disables and resets the timer.
    TCA0.SINGLE.CTRLD = TCA_SPLIT_ENABLE_bm; // Enable split mode
    // In split mode the timer operates as two 8 bit timers with each having three compare channels for PWM.
    TCA0.SPLIT.LPER = 254; // If 0 and 255 turn PWM off, you want to count to 254 not 255.
    TCA0.SPLIT.HPER = 254;
    // TCA0.SPLIT.CTRLB = TCA_SPLIT_HCMP0EN_bm; // Enable compare 0 on high counter, disable compare 0, 1 and 2 on low counter
    TCA0.SPLIT.INTCTRL = 0; // Disable all interrupts
    TCA0.SPLIT.CTRLA = TCA_SPLIT_CLKSEL_DIV64_gc | TCA_SPLIT_ENABLE_bm;
    PORTMUX.CTRLC = PORTMUX_TCA00_ALTERNATE_gc; // Connect Wave Output 0 to it's alternative pin PA7
    
    pinModeFast(PIN_PA0, OUTPUT); // updi
    pinModeFast(PIN_PA1, OUTPUT); // Green backlight
    pinModeFast(PIN_PA2, OUTPUT); // Red backlight
    pinModeFast(PIN_PA3, OUTPUT); // ws2812 data out
    pinModeFast(PIN_PA6, INPUT_PULLUP); // button
    pinModeFast(PIN_PA7, OUTPUT); // Blue backlight
    
    ADC0.CTRLA = 0; // disable ADC to conserve power

    leds.begin();
    // setBrightness(getBrightness());
    leds.fill(0);
    leds.show();
}

// returns true if the button is pressed
bool NameTag::isButtonPressed() {
    return !digitalReadFast(PIN_PA6);
}

void setPWM(uint8_t enable_bm, uint8_t pin_bm, register8_t *cmpReg, uint8_t duty) {
    if (duty == 0) {
        // Turn the pin off
        TCA0.SPLIT.CTRLB &= ~enable_bm;
        PORTA.OUTCLR = pin_bm; // set pin to low
    } else if (duty == 255) {
        // Turn the pin on
        TCA0.SPLIT.CTRLB &= ~enable_bm;
        PORTA.OUTSET = pin_bm; // set pin to high
    } else {
        // Set compare register to duty cycle
        *cmpReg = duty;
        TCA0.SPLIT.CTRLB |= enable_bm;
    }
}

void setPWMSmall(uint8_t bm, uint8_t duty) {
    // This does basically the same as setPWM but takes slightly less flash
    uint8_t pin_bm = bm;
    if (bm == 1) pin_bm = 0x80;
    if (duty == 0) {
        // Turn the pin off
        TCA0.SPLIT.CTRLB &= ~bm;
        PORTA.OUTCLR = pin_bm; // set pin to low
    } else if (duty == 255) {
        // Turn the pin on
        TCA0.SPLIT.CTRLB &= ~bm;
        PORTA.OUTSET = pin_bm; // set pin to high
    } else {
        // Set compare register to duty cycle
        *((&TCA0.SPLIT.LCMP0 + (pin_bm & 0xF))) = duty;
        TCA0.SPLIT.CTRLB |= bm;
    }
}

void NameTag::setBlue(uint8_t duty) {
    // Waveform Output 0 (WO0) is connected to pin A7
    setPWMSmall(0x01, duty);
}

void NameTag::setGreen(uint8_t duty) {
    // Waveform Output 1 (WO1) is connected to pin A1
    setPWMSmall(0x02, duty);
}

void NameTag::setRed(uint8_t duty) {
    // Waveform Output 2 (WO2) is connected to pin A2
    setPWMSmall(0x04, duty);
}

void NameTag::setPanelColor(uint32_t color) {
    setRed(color >> 16);
    setGreen(color >> 8);
    setBlue(color);
}

void NameTag::setPanelColor(uint32_t color, uint8_t brightness) {
    uint8_t r = color >> 16;
    uint8_t g = color >> 8;
    uint8_t b = color;
    nscale8x3(r, g, b, brightness);
    setRed(r);
    setGreen(g);
    setBlue(b);
}
