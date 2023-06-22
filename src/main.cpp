// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#if (F_CPU>7370000) //neopixel library required 7.37MHz minimum clock speed; this line is used to skip this sketch in internal testing. It is not needed in your sketches.

// #include <tinyNeoPixel.h>
#include <Arduino.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            A6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      8

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
// tinyNeoPixel pixels = tinyNeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second

void setup() {

  // pixels.begin(); // This initializes the NeoPixel library.
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  // pinMode(A2, OUTPUT);

  // analogWrite(A1, 500);
  // analogWrite(A6, 100);
  // analogWrite(A7, 150);
  // digitalWrite(A7, HIGH);
  // digitalWrite(A3, HIGH);
  // digitalWrite(A1, HIGH);
}

void loop() {


  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  // pixels.setBrightness(20);
  // pixels.show();
  // for(int i=0;i<4;i++){

  //   // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
  //   pixels.setPixelColor(i, pixels.Color(251,72,196)); // Moderately bright green color.
  //   pixels.setPixelColor(i+4, pixels.Color(63,255,33)); // Moderately bright green color.

  //   pixels.show(); // This sends the updated pixel color to the hardware.

  //   delay(delayval); // Delay for a period of time (in milliseconds).

  // }
  
  while (1) {
    // delay(1000);
    // digitalWrite(A6, LOW);
    // delay(1000);
    // digitalWrite(A6, HIGH);

    // fade pin a1 up and down
    for (int i = 0; i < 255; i++) {
      analogWrite(A1, i);
      delay(10);
    }
    analogWrite(A1, 0);

    for (int i = 0; i < 255; i++) {
      analogWrite(A2, i);
      delay(10);
    }
    analogWrite(A2, 0);


  }
}
#else //neopixel library required 7.37MHz minimum clock speed; these and following lines are used to skip this sketch in internal testing. It is not needed in your sketches.
#warning "Neopixel control requires F_CPU > 7.37MHz"
void setup() {}
void loop() {}
#endif


// -----------------------------------------------------

