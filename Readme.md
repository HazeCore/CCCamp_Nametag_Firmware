# CCCamp 2023 Nametag Firmware

This is the firmware for our Nametag Badge for Chaos Communication Camp 2023.
You can find more information about the project at [https://hazeco.re/cccamp-nametag/](https://hazeco.re/cccamp-nametag/).

The stock firmware delivered with the badge is tagged as version "v1.0".

This is a PlatformIO project with [megaTinyCore](https://github.com/SpenceKonde/megaTinyCore).
The entry point is located in "src/main.cpp".
You can find the typical setup and loop functions there that you might know from Arduino.

## About the hardware and software decisions

The Nametag uses the ATtiny402 microprocessor. Be careful when looking for datasheets, there are different versions with varying amounts of information.

It contains 4KB of flash which is a significant limiting factor for the amount and fidelity of animations we can run on the badge.
The MCU is configured to run at 8MHz. This is achieved by using the internal 16Mhz oscillator with a prescaler of 2. There is no need to run any faster and it draws less power. 

### LEDs

The backlight contains three LEDs for red, green and blue. They are connected to pins A2, A1 and A7 respectively and are controlled through PWM.

The individual LEDs are WS2812b compatible and are controlled via pin A3. We use the **[tinyNeoPixel](https://github.com/SpenceKonde/megaTinyCore/tree/master/megaavr/libraries/tinyNeoPixel_Static)** library contained in megaTinyCore in it's "static" version which uses less flash. It is an adaptation of the [Adafruit NeoPixel Library](https://github.com/adafruit/Adafruit_NeoPixel).

We use and include the **lib8tion** library of FastLED. FastLED itself is not compatible with the ATtiny402 thus we copied this specific part of the code.
The library provides a range of useful math functions related to LEDs and color manipulation. Documentation about the available functions is available on [The FastLED Website](http://fastled.io/docs/group__lib8tion.html).
The lib8tion library is included under it's original MIT license. (See lib/lib8tion/Readme.md)

### Timers

The ATtiny402 contains two timers.

**Timer A (TCA0)** can operate in two modes:
- In "normal" mode it has a 16 bit counter with 3 compare channels.
- In "split" mode it has two 8 bit counters with 3 compare channels each.  
  Though as far as I can tell only 4 pwm outputs ("Waveform outputs") can be used.
We operate in split mode, although normal mode would probably work as well as we only need three pwm outputs.
Unfortunately this timer doesn't operate in Standby mode.

**Timer B (TCB0)** has a 16 bit counter as well but only a single compare channel.
Thus we can't use it for our three pwm outputs and it's instead configured to run the Arduino millis and delay functions.


The **Real Time Counter (RTC)** (not be be confused with real-time-clock) is used with it's Periodic Interrupt Timer (PIT) to
wake the MCU from sleep every ~32ms to update the animations at a rate of about 32fps.
It contains a 16-bit counter as well and uses the internal 1kHz clock source.

### Sleep

The ATtiny402 has three sleep modes:
- Idle: only the CPU is stopped, everything else keeps running
- Standby: CPU and some selected peripherals stop
- Power Down: Almost everything stops and only a select few wake up sources exist

As Timer A stops in standby mode the deepest sleep mode we can use with pwm is idle. Doing that between frames saves about 2mA. (Other changes might have affected the actual difference in the meantime)

To improve power draw we disable Timer B interrupts between frames which breaks millis. As a replacement we provide a rough_millis function that works the same but operates at a resolution of 32ms, matching the frame rate. Animations get the time since they where started as a parameter and should thus not need to call millis themselves.

## Writing software

The lib folder includes a "NameTag" library that contains a bunch of functions to setup and control the hardware itself without a specific link to the animation software itself. If you wan't to write your own fully custom software you may want to keep that.  

### Modifying or creating your own animations

The animations are each defined in header files included in the main.cpp file. It contains an array with all animations. You can easily modify the code of the existing animations. Keep in mind the strict flash limitations, the current firmware is very tight and even small additions might push it over the edge. To free up memory you can for example remove animations that you don't like. If you want to create a new animation you need to add it to the array to include it in the rotation.

### Programming

The Attiny402 is programmed through UPDI. To do that you have two options:
- Use an Arduino (or maybe similar) with the jtag2updi software
- Use a standard USB UART adapter (FTDI or similar) with some passive components to adapt UART to UPDI

Platformio contains two environments for both programming options.

For detailed information about this checkout the [software section](https://hazeco.re/cccamp-nametag/software/) on the website.

## Contributions

We welcome contributions of any kind. New animations are welcome but please understand that we can't enable them in the stock firmware due to the flash limitations. We are also open for code improvements generally, especially when it comes to saving flash. But there is always a balance as we would still like to keep the code decently readable.
