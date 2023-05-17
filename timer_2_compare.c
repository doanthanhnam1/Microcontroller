
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
#include <stdio.h>
#include <stdlib.h>
#define _XTAL_FREQ 20000000
int dem, A;


void main(void) {
    ANSEL = ANSELH = 0;//disable all analog I/O
    T2CKPS1 = 0; T2CKPS0 = 0; //Chon ti le bo chia truoc (prescaler) bang 1
    TOUTPS3 = 0; TOUTPS2 = 0; TOUTPS1 = 0; TOUTPS0 = 0; //Chon ti le bo chia sau (postscaler) bang 1
    //??t giá tri lon nhat cho giá tri thanh ghi ??m 
    TMR2 = 0;
    PR2  = 49; 
    TMR2ON=1;//Cho phép timer 2 h?at ??ng
    //Kh?i t?o ng?t timer 2 n?u có s? d?ng :
    TMR2IE=1;
    TMR2IF=0;
    PEIE=1;
    GIE=1; 
    TRISE0 = 0;
    RE0 = 0; 
    while (1){
        gocquay(70);
		__delay_ms(200);
		gocquay(135);
		__delay_ms(200);
    }
    
}
void __interrupt() ngat()
{
    if(TMR2IF)
    {
        dem++;
        if(dem == A){RE0 = ~RE0;}
        if(dem == 2000){dem =0;RE0 = ~RE0;}
    }
    TMR2IF = 0;
}
int gocquay(int goc)
{
  A = ((1000.0/180.0)*goc+1000.0)/20;
}