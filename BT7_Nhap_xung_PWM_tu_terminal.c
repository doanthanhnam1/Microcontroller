#include<htc.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#define _XTAL_FREQ 20000000


__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_ON & MCLRE_OFF & CP_OFF & CPD_OFF 
 & BOREN_ON & IESO_OFF & FCMEN_OFF & LVP_OFF & DEBUG_OFF);

unsigned int dem,ccp_input;
bit flag;
char  rxdata;
char *ptr1;
void Width_Pulse();

void sendchar (char c);
void sendstring (const char* s);
void tx_init();
void rx_init();

char ptr;
char chuoi[15];
char chuoi_ccp[6];

void main() 
{
ANSEL = ANSELH =0;
TRISC2 =0;  // output CCP1
PR2=249;   // chu ky pwm
CCPR1L=49;
CCP1CON=0b00001100;///su dung tinh nang pwm muc thap ||0x0C->0b00001100: muc cao
TMR2ON=1;

tx_init();
rx_init();
while(1)
{

		if(flag == 1)
		{
			ccp_input = strtol(chuoi_ccp,&ptr1,10);
			Width_Pulse();
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
		rxdata = RCREG;
		chuoi_ccp[ptr] = rxdata;	// Neu khong phai la 'A' hoac 'B' thi cong don	
		ptr++;
	    if(ptr == 3){
		flag =1;
		ptr=0;			
	}			
 	RCIF=0;	
	}   
	//// kiem tra loi
	if( OERR==1)
	{
	  CREN=0;/// de xoa loi
	  CREN=1;/// cho hoat dong lai		
	}

}
void Width_Pulse()
{
	    CCPR1L= ccp_input*249/100;
		sprintf(chuoi,"CCPR1L:%d ",CCPR1L);
		
		sendstring("\n");
		sendstring(chuoi);
}
