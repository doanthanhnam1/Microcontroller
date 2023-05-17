// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "lcd.h"
#include <stdio.h>
#include <stdlib.h>
#define _XTAL_FREQ 20000000
int dem, xung;
float v, f, t, d;
void tx_init();
void main(void) {
    ANSEL = ANSELH = 0; // Diasble analog I/O
    LCD_Initialize();
    xung = 30202;
    char str[20];
    TRISC2 = 0;
    TRISE0 = 0;
    TRISE1 = 0;
    //timer 0 setup - timer mode
    T0CS = 0;
    //timer 1 setup - counter mode
    TRISC0 = 1;
    TMR1CS = 0; //Timer mode
    T1CKPS1 = 0;
    T1CKPS0 = 0; //Prescale 1:1
    TMR1H = 0;
    TMR1L = 0;
    TMR1ON = 1; //Timer1 on
    TMR1GE = 0;
    
    GIE = 1; //Global Interrupt Enable
    PEIE = 1;
    PIE1 = 1;
    
    TMR1IF = 0;
    
    //timer 2 set up
    T2CKPS1 = 0;
    T2CKPS0 = 1;
    TMR2 = 0;
    PR2 = 125;
    CCP1CON = 0b00001110;
    CCPR1L = 100;
    TMR2ON = 1;
    tx_init();
    while (1) {
        if(RCREG == '1') {RA3=RA4 = 0;send_string(" Number 1 ");}
		if(RCREG == '2') {RA3=RA4 = 1;send_string(" Number 2 ");}
		if(RCREG == '3') {RA3=1; RA4 = 0;send_string(" Number 3 ");}
		if(RCREG == '4') {RA3=0; RA4 = 1;send_string(" Number 4 ");}
        if(RCREG != '1' && RCREG != '2' && RCREG != '3' && RCREG != '4')
			{
				RA3=RA4=1;
				__delay_ms(10);
				RA3=RA4=0;
				__delay_ms(10);
				send_string(" Other numbers ");
			}

           
}

void __interrupt() ngat() {
    if (T0IF) {
        dem++;
        if (dem == 250) {
            dem = 0;
            xung = TMR1;
            if (v < 20.0) {
                CCPR1L = CCPR1L + 10;
            } else {
                CCPR1L = CCPR1L - 10;
                TMR1 = 0;
            }
        }
        TMR0 = 7;
        T0IF = 0;
    }
}
void tx_init()
{
    SPBRGH = 0x02;
    SPBRG = 0x81;
    SYNC = 0; BRGH = 1; BRG16 = 0; //truyen toc do cao, 16bit, khong dong bo
    TX9 = 0;
    TXEN = 1;
    SPEN = 1;
}
void send_string (const char *s)
{
    while(*s){send_char(*s++);}
}
