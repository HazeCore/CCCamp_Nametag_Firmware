#pragma once

#include <Arduino.h>
#include "lib8tion.h"
#include "Animation.h"
#include "NameTag.h"

// https://forum.arduino.cc/t/a-faster-sin-lookup-function/69173/8

// 91 x 2 bytes ==> 182 bytes
unsigned int isinTable16[] = { 
  0, 1144, 2287, 3430, 4571, 5712, 6850, 7987, 9121, 10252, 11380, 
  12505, 13625, 14742, 15854, 16962, 18064, 19161, 20251, 21336, 22414, 
  23486, 24550, 25607, 26655, 27696, 28729, 29752, 30767, 31772, 32768, 

  33753, 34728, 35693, 36647, 37589, 38521, 39440, 40347, 41243, 42125, 
  42995, 43851, 44695, 45524, 46340, 47142, 47929, 48702, 49460, 50203, 
  50930, 51642, 52339, 53019, 53683, 54331, 54962, 55577, 56174, 56755, 

  57318, 57864, 58392, 58902, 59395, 59869, 60325, 60763, 61182, 61583, 
  61965, 62327, 62671, 62996, 63302, 63588, 63855, 64103, 64331, 64539, 
  64728, 64897, 65047, 65176, 65286, 65375, 65445, 65495, 65525, 65535, 
  }; 

// 91 bytes 
uint8_t isinTable8[] = { 
  0, 4, 9, 13, 18, 22, 27, 31, 35, 40, 44, 
  49, 53, 57, 62, 66, 70, 75, 79, 83, 87, 
  91, 96, 100, 104, 108, 112, 116, 120, 124, 128, 

  131, 135, 139, 143, 146, 150, 153, 157, 160, 164, 
  167, 171, 174, 177, 180, 183, 186, 190, 192, 195, 
  198, 201, 204, 206, 209, 211, 214, 216, 219, 221, 

  223, 225, 227, 229, 231, 233, 235, 236, 238, 240, 
  241, 243, 244, 245, 246, 247, 248, 249, 250, 251, 
  252, 253, 253, 254, 254, 254, 255, 255, 255, 255, 
  }; 


float isin(long x)
{
  boolean pos = true;  // positive - keeps an eye on the sign.
  if (x < 0) 
  {
    x = -x;
    pos = !pos;  
  }  
  if (x >= 360) x %= 360;   
  if (x > 180) 
  {
    x -= 180;
    pos = !pos;
  }
  if (x > 90) x = 180 - x;
//  if (pos) return isinTable8[x] * 0.003921568627; // = /255.0
//  return isinTable8[x] * -0.003921568627 ;
  if (pos) return isinTable16[x] * 0.0000152590219; // = /65535.0
  return isinTable16[x] * -0.0000152590219 ;
}

float icos(long x)
{
  return isin(x+90);
}

float itan(long x) 
{
  return isin(x) / icos(x);
}

float fsin(float d)
{
  float a = isin(d);
  float b = isin(d+1);
  return a + (d-int(d)) * (b-a);
}

int16_t mySin(uint8_t x) {
    boolean pos = true;  // positive - keeps an eye on the sign.
    if (x < 0) {
        x = -x;
        pos = !pos;  
    }  
    if (x > 128) {
        x -= 128;
        pos = !pos;
    }
    if (x > 90) {
        x = 128 - x;
    }
    if (pos) return isinTable8[x];
    return -isinTable8[x];
}

int16_t myCos(uint8_t x) {
    return mySin(x + 64);
}

struct Twinkle : public Animation {

    Twinkle() {

    }

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