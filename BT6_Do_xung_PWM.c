#include<htc.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include "lcd(16).h"
#define _XTAL_FREQ 20000000

__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_ON & MCLRE_OFF & CP_OFF & CPD_OFF 
 & BOREN_ON & IESO_OFF & FCMEN_OFF & LVP_OFF & DEBUG_OFF);

long float t,tan_so;
long int CCPR1old=0;
unsigned int dem ;

char rxdata;

void sendchar (char c);
void sendstring (const char* s);
void tx_init();
void rx_init();

char buffer[6];
char ptr;
char chuoi[20];

void main()
{
	
	/// THIET LAP NGO RA VA LCD
	ANSEL = ANSELH=0;
	lcd_init();

	TRISC2=1;
	
	////////////////////////////////////////////////////////////
    //// config timer 1 - timer
	TMR1CS = 0; // timer 
    T1CKPS1 = 0;  // prescale 1:1
	T1CKPS1 = 0;
    T1SYNC  = 1;
    TMR1GE  = 0; //non-control gate

	TMR1H = 0;  //clear TMR1
	TMR1L = 0;
    
	//Khoi tao CCP1
	CCP1CON= 0b00000101;  // x?y ra s? ki?n khi có 1 c?nh lên
	CCPR1H = CCPR1L = 0; // reset hai thanh ghi khoi CCPx


	CCP1IE=1; //Cho phép ngat CCP1	
	CCP1IF=0; //Reset co ngat;	
	PEIE=1; //Cho phép ngat ngoai vi	
	GIE=1; //Cho phép ngat toàn cuc. 
  	////enable timter 1
    TMR1ON = 1;

	TRISC1=0;
	PR2=98;   // chu ky pwm // tru -1 de hien LCD chinh xac
	T2CKPS1 = 0;  // prescale 1:1
	T2CKPS0 = 0;
	CCP2CON=0b00001100;
	CCPR2L=32; // set duty cycle  66%
	TMR2ON=1;

	tx_init();
	rx_init();

	while(1)
		{   
			tan_so = 1.0/((dem*       (1.0/(20000000.0/4.0)))         /1.0) ; //1 o cuoi la do CCP1 o che do capture 1 canh lên 0101
			lcd_gotoxy(0,0);
			printf("f:%3.1f Hz",tan_so);
			sprintf(chuoi,"Tan so:%3.1f Hz\n",tan_so);
			sendstring(chuoi);

			
			}
		}
	

void interrupt ngat()
	{	if(CCP1IE && CCP1IF)
		{	
			dem =CCPR1 - CCPR1old ;   // So xung (tham chieu cua timer 1) khi che do capture timer 2 xay ra su kien 1 lan (1 T):T= 1/( 20 MHz/4)
			CCPR1old = CCPR1;				 
		}
		CCP1IF=0;
		if( RCIE && RCIF)
		{
		 rxdata=RCREG;
			}   
		RCIF=0;
		if( OERR==1)
		{
		  CREN=0;/// de xoa loi
		  CREN=1;/// cho hoat dong lai		
		}

}
void putch(char c){	
	lcd_putc(c);		
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

