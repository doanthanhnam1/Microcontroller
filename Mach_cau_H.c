#include<htc.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#define _XTAL_FREQ 20000000
__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_ON & MCLRE_OFF & CP_OFF & CPD_OFF 
 & BOREN_ON & IESO_OFF & FCMEN_OFF & LVP_OFF & DEBUG_OFF);

char  rxdata;
void sendchar (char c);
void sendstring (const char* s);
void tx_init();
void rx_init();
char ptr;
char chuoi[15];


void main() 
{
	ANSEL = ANSELH =0;
	TRISC2=0;
	//set cac cong dao chieu dong co
	TRISC5=0;
	TRISC4=0;
	PR2=124;   // chu ky pwm 10khz
	T2CKPS1 = 0;  // prescale 1:4
	T2CKPS0 = 1;
	CCP1CON=0b11001111;

	//CCP1CON.bits
	//P1M1= 1;	
	//P1M0= 1;
	//DC1B1=0;
	//DC1B0=0;
	//CCP1M3= 1;
	//CCP1M2= 1;
	//CCP1M1=	1;
	//CCP1M0= 1;

	CCPR1L= 100 ;  // set duty cycle  80%
	TMR2ON=1;
	RC4 =0; RC5 = 0;
	//////////////////
	tx_init();
	rx_init();
	while(1)
	{	
		
	if (rxdata == 'S') 
		{
			CCP1M3= 1;
			CCP1M2= 1;
			CCP1M1=	0;	
			CCP1M0= 1;
			sendstring("DC start\n");

		}
	else if (rxdata == 'P') 
		{
			CCP1M3= 1;
			CCP1M2= 1;
			CCP1M1=	1;
			CCP1M0= 1;
			sendstring("DC stop\n");
		
		}
	else if (rxdata == 'R')
		{
			CCP1M3= 1;
			CCP1M2= 1;
			CCP1M1=	1;
			CCP1M0= 0;
			sendstring("Ðao chieu R\n");
			
						
		}
	else if (rxdata == 'L')
		{
		
			CCP1M3= 1;
			CCP1M2= 1;
			CCP1M1=	0;
			CCP1M0= 1;
			sendstring("Ðao chieu L\n");
		}		
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
	BRG16= 1;
	
	SYNC=0;
	SPEN=1;
	
	TX9=0;
	
	TXEN=1;
}
void rx_init(){

	SPBRGH =0x02;  // calculate --> 520 <-> br:9600
	SPBRG = 0x08;	
	BRGH=1;
	BRG16= 1;
	
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
		RCIF=0;
		if( OERR==1)
		{
		  CREN=0;/// de xoa loi
		  CREN=1;/// cho hoat dong lai		
		}

}




