#include<htc.h>
#include<stdio.h>
#include<math.h>
#include "lcd(16).h"


#define _XTAL_FREQ 800000

__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_ON & MCLRE_OFF & CP_OFF & CPD_OFF 
 & BOREN_ON & IESO_OFF & FCMEN_OFF & LVP_OFF & DEBUG_OFF);

float k;
float var;
/*void goc_quay()
{
	CCPR1L= 13;//(var/180.0)*0.5*0.1;

}*/
void putch(char c)
	{lcd_putc(c);}

void main()
{	
	lcd_init();
	ANSEL=ANSELH=0;
    TRISC2 = 0; 
    	
	PR2 = 249;
	CCP1CON=0b00001110;///su dung tinh nang pwm muc thap ||0x0C->0b00001100: muc cao
	CCPR1L= 15;
	TMR2IF=0;
	
	T2CKPS1 = 1;  // prescale 1:16
	T2CKPS0 = 1;


	TMR2ON=1;
	while(1)
		{
			
			/*lcd_gotoxy(0,0);
			printf("CCPR1L: %3.2f",var);*/

		}
	}
