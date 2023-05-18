#include<htc.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#define _XTAL_FREQ 20000000
__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_ON & MCLRE_OFF & CP_OFF & CPD_OFF 
 & BOREN_ON & IESO_OFF & FCMEN_OFF & LVP_OFF & DEBUG_OFF);

char dem;
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
	CCP1CON=0b00001100;///su dung tinh nang pwm muc thap ||0x0C->0b00001100: muc cao
	CCPR1L= 100 ;  // set duty cycle  80%
	TMR2ON=1;
	RC4 =0; RC5 = 0;
	//////////////////
	tx_init();
	rx_init();
	dem=0;
	while(1)
	{	
		
	if (rxdata == 'S') 
		{
			RC4 =1; RC5 = 0;
			sendstring("DC start\n");
			dem=0;
		}
	else if (rxdata == 'P') 
		{
			RC4 = RC5 = 0;
			sendstring("DC stop\n");
			dem=0;
		}
	else if (rxdata == 'R')
		{
			if (dem==0){
			RC4 =! RC4;RC5 =! RC5;
			sendstring("Ðao chieu R\n");
			dem++;
			}			
		}
	else if (rxdata == 'L')
		{
			if (dem !=0){
			RC4 =! RC4;RC5 =! RC5;
			sendstring("Ðao chieu L\n");
			dem=0;
			}		
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




