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

#define _XTAL_FREQ 8000000


void enablePWM4(){
    PWM4CON = 0; // start pwm register from 0, PWM4 is RA5
    PR2 = 255; // set PWM period to ADC lower bits
    PWM4DCH = 0; // high bits to 0
    PWM4DCLbits.PWM4DCL0 = 0; PWM4DCLbits.PWM4DCL1 = 0; // low bits to 0
    PIR1bits.TMR2IF = 0; // no timer 2 interrupt
    T2CONbits.T2CKPS = 0; // prescaler is 1
    T2CONbits.TMR2ON = 1; // turn timer2 on
    PWM4CONbits.PWM4EN = 1; // enable PWM
    while(!PIR1bits.TMR2IF){
        __delay_ms(1);
    }
    TRISAbits.TRISA5 = 0; // enable TRISA output
    PWM4CONbits.PWM4OE = 1; // enable output for PWM
}

void enablePWM2(){
    PWM2CON = 0; // start pwm register from 0, PWM2 is RA0
    PR2 = 255; // set PWM period to ADC lower bits
    PWM2DCH = 0; // high bits to 0
    PWM2DCLbits.PWM2DCL0 = 0; PWM2DCLbits.PWM2DCL1 = 0; // low bits to 0
    PIR1bits.TMR2IF = 0; // no timer 2 interrupt
    T2CONbits.T2CKPS = 0; // prescaler is 1
    T2CONbits.TMR2ON = 1; // turn timer2 on
    PWM2CONbits.PWM2EN = 1; // enable PWM
    while(!PIR1bits.TMR2IF){
        __delay_ms(1);
    }
    TRISAbits.TRISA0 = 0; // enable TRISA output
    PWM2CONbits.PWM2OE = 1; // enable output for PWM
}

void enablePWM1(){
    PWM1CON = 0; // start pwm register from 0, PWM1 is RA2
    PR2 = 255; // set PWM period to ADC lower bits
    PWM1DCH = 0; // high bits to 0
    PWM1DCLbits.PWM1DCL0 = 0; PWM1DCLbits.PWM1DCL1 = 0; // low bits to 0
    PIR1bits.TMR2IF = 0; // no timer 2 interrupt
    T2CONbits.T2CKPS = 0; // prescaler is 1
    T2CONbits.TMR2ON = 1; // turn timer2 on
    PWM1CONbits.PWM1EN = 1; // enable PWM
    while(!PIR1bits.TMR2IF){
        __delay_ms(1);
    }
    TRISAbits.TRISA2 = 0; // enable TRISA output
    PWM1CONbits.PWM1OE = 1; // enable output for PWM
}

void enableADC(){
    ADCON1bits.ADFM = 0; // left justified, for PWM compatibility
    ADCON1bits.ADCS = 0b011; // use internal clock for ADC
    ADCON1bits.ADPREF = 0b00; // use VDD as reference
    ADCON0bits.CHS = 0b0011; // select RA4 as input
    ADCON0bits.ADON = 1; // enable ADC
}



void main(void) {
    LATA = 0;
    TRISAbits.TRISA5 = 1; // RA5 - input, but becomes output after PWM setup
    TRISAbits.TRISA0 = 1; // RA0 - input, but becomes output after PWM setup
    TRISAbits.TRISA2 = 1; // RA2 - input, but becomes output after PWM setup
    TRISAbits.TRISA4 = 1; // RA4 - input
    TRISAbits.TRISA1 = 1; // RA1 - input
    ANSELAbits.ANSA1 = 0;
    
    OSCCONbits.SCS1 = 1; // select internal oscillator
    OSCCONbits.IRCF = 0b1110; // 8mhz
    ANSELAbits.ANSA4 = 1; // RA4 is analog input
    OPTION_REGbits.nWPUEN = 0; // enable weak pull ups
    WPUA = 0; // clear all pull ups
    WPUAbits.WPUA1 = 1;
    
    enableADC();
    enablePWM4();
    enablePWM2();
    enablePWM1();
    
    int curPWM = 0;
    while(1){
        ADCON0bits.GO = 1; // start AD conversion
        __delay_ms(1);
        switch(curPWM){
        case 0:
            PWM1DCH = ADRESH;
            PWM1DCL = ADRESL;
            break;
        case 1:
            PWM2DCH = ADRESH;
            PWM2DCL = ADRESL;
            break;
        case 2:
            PWM4DCH = ADRESH;
            PWM4DCL = ADRESL;
            break;
                
        }
        
        /*PWM4CONbits.PWM4OUT = 1;
        PWM1CONbits.PWM1OUT = 1;
        PWM2CONbits.PWM2OUT = 1;*/
        if(PORTAbits.RA1 == 0){
            __delay_ms(1000);
            curPWM++;
            if(curPWM > 2){
                curPWM = 0;
            }
        }
        __delay_ms(10);
        
    }
    
    return;
}
