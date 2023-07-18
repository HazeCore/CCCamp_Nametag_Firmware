#include <Arduino.h>
#include <avr/sleep.h>

#include "NameTag.h"

using namespace NameTag;

void waitRTC() {
    while (RTC.STATUS > 0) {
    ;                                   // Wait for all registers to be synchronized
  }
}

void waitPIT() {
    while (RTC.PITSTATUS > 0) {
    ;                                   // Wait for all registers to be synchronized
  }
}

void setup() {
    NameTag::setup();
    setBlue(255);
    NameTag::delay(1000);
    setBlue(0);


    // configure A6 as input pullup with interrupt on falling edge
    // PORTA.DIRCLR = PIN6_bm; // set pin to input
    // PORTA.PIN6CTRL |= PORT_PULLUPEN_bm; // enable pullup
    // PORTA.PIN6CTRL |= PORT_ISC_FALLING_gc; // set interrupt on falling edge

    // attachInterrupt(digitalPinToInterrupt(A6), []() {
    //     setRed(255);
    //     NameTag::delay(1000);
    //     setRed(0);
    // }, FALLING);
    

    // RTC

    // waitRTC();
    // RTC.CLKSEL = RTC_CLKSEL_INT32K_gc;
    // waitRTC();
    // RTC.CTRLA |= RTC_PRESCALER_DIV1_gc;
    // waitRTC();
    // RTC.CMP = 32000; // 1 second
    // waitRTC();
    // RTC.PER = 32000; // 1 second
    // waitRTC();
    // RTC.INTCTRL |= RTC_OVF_bm | RTC_CMP_bm; // enable overflow interrupt
    // waitRTC();
    // RTC.CTRLA |= RTC_RTCEN_bm | RTC_RUNSTDBY_bm; // enable RTC
    // waitRTC();

    // end

    // PIT

    // waitRTC();
    // RTC.CLKSEL = RTC_CLKSEL_INT32K_gc;
    // waitPIT();
    // RTC.PITCTRLA |= RTC_PERIOD_CYC32768_gc;
    // waitPIT();
    // RTC.PITINTCTRL |= RTC_PI_bm;
    // waitPIT();
    // RTC.PITCTRLA |= RTC_PITEN_bm;
    // waitPIT();

    // PIT END


    // CPU_SREG &= ~CPU_I_bm;
    interrupts();

    // SLPCTRL_CTRLA = SLPCTRL_SMODE_PDOWN_gc | SLPCTRL_SEN_bm;
    // SLPCTRL_CTRLA = SLPCTRL_SMODE_STDBY_gc | SLPCTRL_SEN_bm;
    // sleep_mode();
    setBlue(0);
    

    // enable main clock during standby sleep
    // CLKCTRL.OSC20MCTRLA |= CLKCTRL_RUNSTDBY_bm;
    // CLKCTRL.OSC32KCTRLA |= CLKCTRL_RUNSTDBY_bm;
}

void loop() {
    // if (!digitalRead(A6)) {
    //     setRed(255);
    //     NameTag::delay(1000);
    //     setRed(0);
    // }
    NameTag::delay(10);

    // SLPCTRL_CTRLA = SLPCTRL_SMODE_IDLE_gc | SLPCTRL_SEN_bm;
    SLPCTRL_CTRLA = SLPCTRL_SMODE_STDBY_gc | SLPCTRL_SEN_bm;
    // SLPCTRL_CTRLA = SLPCTRL_SMODE_PDOWN_gc | SLPCTRL_SEN_bm;
    sleep_mode();
}

// ISR(PORTA_PORT_vect) {
//     // uart_flag = 1;              // signalisiere neue Daten
//     // uart_data = UDR;            // Daten auslesen, dadurch wird auch der Interrupt gelöscht
//     setRed(255);
//     NameTag::delay(1000);
//     setRed(0);
//     PORTA.INTFLAGS |= PORT_INT6_bm; // clear interrupt flag
// }

ISR(RTC_CNT_vect) {
    setRed(255);
    NameTag::delay(100);
    setRed(0);
    RTC.INTFLAGS |= RTC_OVF_bm | RTC_CMP_bm; // clear interrupt flag
}

ISR(RTC_PIT_vect) {
    setGreen(255);
    NameTag::delay(100);
    setGreen(0);
    RTC.PITINTFLAGS |= RTC_PI_bm; // clear interrupt flag
}