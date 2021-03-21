/*
 * File:   newmain.c
 * Author: stefan
 *
 * Created on March 21, 2021, 7:11 PM
 */

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)

#include <xc.h>

#define _XTAL_FREQ 500000

void main(void) {
    LATA = 0;
    TRISAbits.TRISA5 = 0; // RA5 - output
    TRISAbits.TRISA4 = 1; // RA4 - input
    OSCCONbits.SCS1 = 1; // select internal oscillator
    OSCCONbits.IRCF = 0b0111; // 500khz
    ANSELAbits.ANSA4 = 1; // RA4 is analog input
    OPTION_REGbits.nWPUEN = 1; // disable weak pull ups
    
    ADCON1bits.ADFM = 1; // right justified, read ADRESH, contains upper 2 bits
    ADCON1bits.ADCS = 0b011; // use internal clock
    ADCON1bits.ADPREF = 0b00; // use VDD as reference
    ADCON0bits.CHS = 0b0011; // select RA4 as input
    ADCON0bits.ADON = 1; // enable ADC
    
    while(1){
        ADCON0bits.GO = 1; // start AD conversion
        __delay_ms(1); // delay so it finishes
        if(ADRESH > 0b01){ // check if 9th bit is greater than 1 (basically if RA5 voltage is >2.5v)
            LATAbits.LATA5 = 1; // RA4 - high
        }
        
        __delay_ms(1000);
        LATAbits.LATA5 = 0; // RA4 - low
        __delay_ms(1000);
        
    }
    
    return;
}
