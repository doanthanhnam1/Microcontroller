#include<htc.h>
#include<stdio.h>
#include<math.h>
#include "lcd(16).h"
#define _XTAL_FREQ 20000000

__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_ON & MCLRE_OFF & CP_OFF & CPD_OFF 
 & BOREN_ON & IESO_OFF & FCMEN_OFF & LVP_OFF & DEBUG_OFF);

long float t,tan_so;
long int CCPR1old=0;
unsigned int dem ;

void main()
{
	
	/// THIET LAP NGO RA VA LCD
	ANSEL = ANSELH=0;
	lcd_init();
	
	////////////////////////////////////////////////////////////
    //// config timer 1 - counter
    TRISC2 = 1; //input CCP1
	TMR1CS = 0; // timer 
    T1CKPS1 = 0;  // prescale 1:1
	T1CKPS1 = 0;
    T1SYNC  = 1;
    TMR1GE  = 0; //non-control gate

	TMR1H = 0;  //clear TMR1
	TMR1L = 0;
    
	//Khoi tao CCP1
	CCP1CON= 0b00000101;
	CCPR1H = CCPR1L = 0; // reset hai thanh ghi khoi CCPx


	CCP1IE=1; //Cho phép ngat CCP	
	CCP1IF=0; //Reset co ngat;	
	PEIE=1; //Cho phép ngat ngoai vi	
	GIE=1; //Cho phép ngat toàn cuc. 
  	////enable timter 1
    TMR1ON = 1;
	
	while(1)
		{   
			tan_so= 1/(dem/5000000.0); // n?u 16 c?nh lên m?i capture thì 16/
			lcd_gotoxy(0,0);
			printf("f:%3.2f",tan_so);
			
			}
		}
	

void interrupt ngat()
	{	if(CCP1IE && CCP1IF)
		{	
			dem =CCPR1 - CCPR1old ;
			CCPR1old = CCPR1;				
		}
		CCP1IF=0;
}
void putch(char c){	
	lcd_putc(c);		
}