#include <Arduino.h>
#include <tinyNeoPixel.h>

#define NUMLEDS 8

tinyNeoPixel leds = tinyNeoPixel(NUMLEDS, PIN_PA3, NEO_GRB);

// Pinout:

// A6 - WS2812B
// A7 PWM
// A1 PWM
// A2 PWM

// WO0 equals A7 in this configuration
void analogWriteBlue(uint8_t duty) {
  if (duty == 0) {
    TCA0.SPLIT.CTRLB &= ~TCA_SPLIT_LCMP0EN_bm; // Turn off PWM if passed   0 duty cycle
    /* you probably also want to digitalWrite() or digitalWriteFast() the pin LOW */
  } else if (duty == 255) {
    TCA0.SPLIT.CTRLB &= ~TCA_SPLIT_LCMP0EN_bm; // Turn off PWM if passed 255 duty cycle
    /* you probably also want to digitalWrite() or digitalWriteFast() the pin HIGH */
  } else {
    TCA0.SPLIT.LCMP0  =  duty;                 // Turn set the duty cycle for WO0
    TCA0.SPLIT.CTRLB |=  TCA_SPLIT_LCMP0EN_bm; // Turn on PWM
  }
}

// WO1 equals A1 in this configuration
void analogWriteGreen(uint8_t duty) {
  if (duty == 0) {
    TCA0.SPLIT.CTRLB &= ~TCA_SPLIT_LCMP1EN_bm; // Turn off PWM if passed   0 duty cycle
    /* you probably also want to digitalWrite() or digitalWriteFast() the pin LOW */
  } else if (duty == 255) {
    TCA0.SPLIT.CTRLB &= ~TCA_SPLIT_LCMP1EN_bm; // Turn off PWM if passed 255 duty cycle
    /* you probably also want to digitalWrite() or digitalWriteFast() the pin HIGH */
  } else {
    TCA0.SPLIT.LCMP1  =  duty;                 // Turn set the duty cycle for WO1
    TCA0.SPLIT.CTRLB |=  TCA_SPLIT_LCMP1EN_bm; // Turn on PWM
  }
}

// WO2 equals A2 in this configuration
void analogWriteRed(uint8_t duty) {
  if (duty == 0) {
    TCA0.SPLIT.CTRLB &= ~TCA_SPLIT_LCMP2EN_bm; // Turn off PWM if passed   0 duty cycle
    /* you probably also want to digitalWrite() or digitalWriteFast() the pin LOW */
  } else if (duty == 255) {
    TCA0.SPLIT.CTRLB &= ~TCA_SPLIT_LCMP2EN_bm; // Turn off PWM if passed 255 duty cycle
    /* you probably also want to digitalWrite() or digitalWriteFast() the pin HIGH */
  } else {
    TCA0.SPLIT.LCMP2  =  duty;                 // Turn set the duty cycle for WO2
    TCA0.SPLIT.CTRLB |=  TCA_SPLIT_LCMP2EN_bm; // Turn on PWM
  }
}

// // For WO3 and up, they use the high bytes of the CMPn registers
// // WO3 equals A3 in this configuration
// void analogWriteWO3(uint8_t duty) {
//   if (duty == 0) {
//     TCA0.SPLIT.CTRLB &= ~TCA_SPLIT_HCMP0EN_bm; // Turn off PWM if passed   0 duty cycle
//     /* you probably also want to digitalWrite() or digitalWriteFast() the pin LOW */
//   } else if (duty == 255) {
//     TCA0.SPLIT.CTRLB &= ~TCA_SPLIT_HCMP0EN_bm; // Turn off PWM if passed 255 duty cycle
//     /* you probably also want to digitalWrite() or digitalWriteFast() the pin HIGH */
//   } else {
//     TCA0.SPLIT.HCMP0  =  duty;                 // Turn set the duty cycle for WO3
//     TCA0.SPLIT.CTRLB |=  TCA_SPLIT_HCMP0EN_bm; // Turn on PWM
//   }
// }



void setup() {
  takeOverTCA0(); // take over TCA0 so digitalWrite() on alt pins won't mess up alternate pin PWM output.
  TCA0.SINGLE.CTRLD = 1;    // Enable split mode.
  TCA0.SPLIT.LPER   = 254;  // or as required by your application. If 0 and 255 turn PWM off, you want to count to 254 not 255.
  TCA0.SPLIT.HPER   = 254;
  TCA0.SPLIT.CTRLA  = TCA_SPLIT_CLKSEL_DIV64_gc | TCA_SPLIT_ENABLE_bm; // same as the core configures by default
  PORTMUX.CTRLC     = PORTMUX_TCA00_ALTERNATE_gc;  /* 0/1-series - You can OR together as many alternate TCA0 groupcodes as needed */
  //PORTMUX.TCAROUTEA = PORTMUX_TCA00_ALT1_gc; /* 2-series has different, less stupid, names. */
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A6, OUTPUT);
  pinMode(A7, OUTPUT);


  leds.begin();
}

// delay by certain amount using nop when the clock speed is 20MHz
// without using millis
void asmdelay(unsigned long long ms) {
  for (unsigned long long i = 0; i < ms; i++) {
    for (unsigned long long j = 0; j < 2500; j++) { 
      asm volatile ("nop");
    }
  }
}


void loop() {
  for (uint16_t d = 0; d < 255; d += 1) {
    analogWriteBlue(d); // A7 - D4
    analogWriteGreen(d); // A1 - D5
    analogWriteRed(d); // A2 - D1
    // analogWriteWO3(d+189); // A3 - D2

    leds.setBrightness(10);
    leds.setPixelColor(0, d, 0, 0);
    leds.setPixelColor(1, d, d, 0);
    leds.setPixelColor(2, 0, d, 0);
    leds.setPixelColor(3, 0, d, d);
    leds.setPixelColor(4, 0, 0, d);
    leds.setPixelColor(5, d, 0, d);
    leds.setPixelColor(6, d, 0, 0);
    leds.setPixelColor(7, d, 0, 0);

    leds.show();

    asmdelay(3);
  }

  for (uint16_t d = 255; d > 0; d -= 1) {
    analogWriteBlue(d); // A7 - D5
    analogWriteGreen(d); // A1 - D4
    analogWriteRed(d); // A2 - D3
    // analogWriteWO3(d+189); // A3 - D2

    leds.setBrightness(10);
    leds.setPixelColor(0, d, 0, 0);
    leds.setPixelColor(1, d, d, 0);
    leds.setPixelColor(2, 0, d, 0);
    leds.setPixelColor(3, 0, d, d);
    leds.setPixelColor(4, 0, 0, d);
    leds.setPixelColor(5, d, 0, d);
    leds.setPixelColor(6, d, 0, 0);
    leds.setPixelColor(7, d, 0, 0);

    leds.show();

    asmdelay(3);
  }

  
}