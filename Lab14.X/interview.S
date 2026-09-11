; =====================================================================
; File:   interview.S
; Author: <your names here>
;
; Lab 14 - The Systems Interview: Read, Count, Beat the Compiler
; ECE 3301L - Introduction to Microcontrollers Laboratory
;
; YOU WRITE the two routines below. The C harness (lab14.c, provided,
; do not modify) self-grades them:
;   - popcount_asm is checked against a C reference on all 256 inputs
;   - delay_100us_exact is measured with Timer1 (expect 400 ticks)
; RA4 solid ON = both pass. Also: annotate mystery.txt (graded!).
;
; Shared globals (declared in C):
;   volatile uint8_t pc_in, pc_out;
; =====================================================================

#include <xc.inc>

GLOBAL  _popcount_asm, _delay_100us_exact
GLOBAL  _pc_in, _pc_out

PSECT udata_acs
work:   DS 1
bits:   DS 1
dcnt:   DS 1

PSECT itext,class=CODE,reloc=2

; ---------------------------------------------------------------------
; popcount_asm - count the 1-bits in pc_in, result to pc_out.
;
; Suggested approach (shift-and-count, 8 fixed iterations):
;   1. movff _pc_in into `work`; clear `bits`; counter = 8
;   2. rrcf work        ; bit 0 falls into the CARRY flag
;   3. if carry set, increment `bits`  (btfsc STATUS,0,a)
;   4. loop 8 times (decfsz)
;   5. movff bits -> _pc_out
;
; Extra credit: do it with no loop (nibble lookup table).
; ---------------------------------------------------------------------
_popcount_asm:
    ; TODO - implement popcount
    return

; ---------------------------------------------------------------------
; delay_100us_exact - EXACTLY 400 instruction cycles at 16 MHz.
;
; At Fosc = 16 MHz the instruction rate is Fosc/4 = 4 MHz, so one
; cycle = 250 ns and 100 us = 400 cycles. Your budget must include
; the CALL (2 cycles, spent by the caller) and your RETURN (2).
;
; Cycle costs: most instructions 1; call/return/goto/bra 2;
; decfsz costs 1 normally, 2 when it skips.
;
; TODO: build a loop whose total comes to exactly 400. Show your
; arithmetic in comments - the cycle budget is part of the grade.
; A skeleton budget table:
;
;   call (caller)            2
;   movlw N                  1
;   movwf dcnt               1
;   loop: decfsz+bra       3*N - 1
;   nop padding              ?
;   return                   2
;   TOTAL                  400   -> solve for N and the padding
; ---------------------------------------------------------------------
_delay_100us_exact:
    ; TODO - implement the cycle-exact delay
    return

    END
