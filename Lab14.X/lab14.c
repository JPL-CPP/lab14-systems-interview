/*
 * File:   lab14.c
 * Author: Jacky Li
 *
 * Lab 14 - The Systems Interview: Read, Count, Beat the Compiler
 * ECE 3301L - Introduction to Microcontrollers Laboratory
 *
 * Self-grading harness for the two asm routines in interview.S.
 *
 * Phase 1 (popcount): runs all 256 inputs through popcount_asm and
 *   checks each against the C reference. RD0-RD7 shows the number of
 *   failing inputs (0x00 = perfect score).
 * Phase 2 (delay): measures delay_100us_exact with Timer1 at
 *   Fosc/4 = 4 MHz (250 ns/tick): a perfect routine measures
 *   400 ticks. Tolerance +/-4 ticks (1 us) for measurement overhead.
 * Verdict: RA4 solid ON = both phases pass. Then the loop toggles
 *   RC0 every delay_100us_exact forever: a 5.000 kHz square wave on
 *   the scope is the cycle-counting proof.
 *
 * Pin Assignments:
 *   RD0-RD7 - popcount failure count / measured-tick display
 *   RA4     - PASS LED (solid = all tests green)
 *   RC0     - scope pin: 5 kHz square wave in phase 3
 */

#include <xc.h>
#include <stdint.h>
#include "PIC18F46K22-Config.h"

#define _XTAL_FREQ 16000000UL

/* ---- Shared state with interview.S ---- */
volatile uint8_t pc_in, pc_out;
extern void popcount_asm(void);
extern void delay_100us_exact(void);

/* C reference the asm must match */
static uint8_t popcount_ref(uint8_t v) {
    uint8_t n = 0;
    while (v) { n += v & 1; v >>= 1; }
    return n;
}

static void init(void) {
    OSCCONbits.IRCF = 0b111;    /* 16 MHz HFINTOSC */
    OSCCONbits.SCS  = 0b10;

    ANSELA = 0x00; ANSELC = 0x00; ANSELD = 0x00;
    TRISD = 0x00;  LATD = 0x00;
    TRISAbits.TRISA4 = 0;  LATAbits.LATA4 = 0;
    TRISCbits.TRISC0 = 0;  LATCbits.LATC0 = 0;
}

void main(void) {
    init();

    /* ---- Phase 1: popcount, all 256 inputs ---- */
    uint8_t fails = 0;
    uint16_t i;
    for (i = 0; i < 256; i++) {
        pc_in = (uint8_t)i;
        popcount_asm();
        if (pc_out != popcount_ref((uint8_t)i)) fails++;
    }
    LATD = fails;               /* 0x00 = perfect */
    __delay_ms(2000);

    /* ---- Phase 2: measure the delay with Timer1 ---- */
    T1CONbits.TMR1CS = 0b00;    /* Fosc/4 = 4 MHz */
    T1CONbits.T1CKPS = 0b00;    /* 1:1 -> 250 ns per tick */
    TMR1H = 0; TMR1L = 0;
    T1CONbits.TMR1ON = 1;
    delay_100us_exact();
    T1CONbits.TMR1ON = 0;
    uint16_t ticks = ((uint16_t)TMR1H << 8) | TMR1L;

    LATD = (uint8_t)(ticks & 0xFF);   /* expect 0x90 = 400 & 0xFF */
    __delay_ms(2000);

    /* ---- Verdict ---- */
    uint8_t delay_ok = (ticks >= 396) && (ticks <= 404);
    if (fails == 0 && delay_ok) {
        LATAbits.LATA4 = 1;     /* PASS */
        LATD = 0x00;
    } else {
        LATAbits.LATA4 = 0;     /* look at RD LEDs for which phase */
        LATD = fails ? fails : (uint8_t)(ticks & 0xFF);
    }

    /* ---- Phase 3: scope proof - toggle RC0 every 100 us.
     * Period = 200 us -> 5.000 kHz square wave, forever. ---- */
    while (1) {
        LATCbits.LATC0 ^= 1;
        delay_100us_exact();
    }
}
