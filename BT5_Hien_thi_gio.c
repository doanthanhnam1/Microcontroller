#include<htc.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include "lcd(16).h"
#define _XTAL_FREQ 20000000

__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_ON & MCLRE_OFF & CP_OFF & CPD_OFF 
 & BOREN_ON & IESO_OFF & FCMEN_OFF & LVP_OFF & DEBUG_OFF);

int h=10,m=58,s=0,h1,m1,s1;
int dem=0;
char hien_thi=0;

char rxdata;

void sendchar (char c);
void sendstring (const char* s);
void tx_init();
void rx_init();

char buffer[6];
char ptr;
char chuoi[20];
void Display_terminal();
void main()
{
	
	/// THIET LAP NGO RA VA LCD
	ANSEL = ANSELH=0;
	lcd_init();

	////////////////////////////////////////////////////////////
    //// config timer 1 - timer
	TMR1CS = 0; // timer
    T1CKPS1 = 0;  // prescale 1:1
	T1CKPS1 = 0;
    T1SYNC  = 1;
    TMR1GE  = 0; //non-control gate

	TMR1=15535;
    
    /////////////// Ngat timer1 
	TMR1IF=0;
	TMR1IE=1;
	PEIE=1;
	GIE=1;
	
	
  ////enable timter 1
    TMR1ON = 1;
///////////
	tx_init();
	rx_init();

/// set LCD

	while(1)
		{
				lcd_gotoxy(0,0);
				printf("%02d:%02d:%02d",h,m,s);
		}						
			
			
		
}	




void sendchar (char c){		
	while (TXIF==0){}
	TXREG = c;
}

void sendstring (const char* s){
	while(*s){
		sendchar(*s++);
	}
}
void tx_init(){
	SPBRGH =0x02;
	SPBRG = 0x08;	

	BRGH=1;  
	BRG16= 1;   /// 8bit     coong thuc /4
	SYNC=0;
	SPEN=1;
	
	TX9=0;
	
	TXEN=1;
}
void rx_init(){

	SPBRGH =0x02;  // calculate -->n= 520 <-> br:9600
	SPBRG = 0x08;
	
	BRGH=1;  
	BRG16= 1;   /// 8bit     coong thuc /4
	SYNC=0;

	SPEN=1;
	
	RCIF=0;
	RCIE=1;
	PEIE=1;
	GIE=1;
	
	RX9=0;
	
	CREN=1;	
}


void interrupt ngat()
	{	
	if( RCIE && RCIF)
	{
	 rxdata=RCREG;
	
	}   
	//// kiem tra loi
	if( OERR==1)
	{
	  CREN=0;/// de xoa loi
	  CREN=1;/// cho hoat dong lai		
	}
 	RCIF=0;
	if(TMR1IF && TMR1IE)
		{	
			dem++;
			if(dem == 100)  // 1s
			{   
				s+=1;
			
				dem=0;
				}
			if (s==59) 
			{
				m+=1;
				s=0;
				}
			if (m==59) 
				{h+=1;
				m=0;
				}
			if (h==23) 
				{s+=1;
				h=0;	
				}	
       	TMR1=15535;
		TMR1IF  = 0; // clear flag int
		}		
}

void putch(char c){	
	lcd_putc(c);		
}