// PWM 10 bit 
#include<htc.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#define _XTAL_FREQ 10000000

__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_ON & MCLRE_OFF & CP_OFF & CPD_OFF 
 & BOREN_ON & IESO_OFF & FCMEN_OFF & LVP_OFF & DEBUG_OFF);

unsigned int dem,ccp_input;
bit flag;
char  rxdata;
char *ptr1;
char ptr;
char chuoi[20];
char chuoi_ccp[10];

void Width_Pulse(int ccp);
void sendchar (char c);
void sendstring (const char* s);
void tx_init();
void rx_init();

//////////////////////////////////////////////////////
void load_pwm_10bit(unsigned int VAL)
{// 10 bit
CCPR1L = VAL/4;  // 
DC1B1 = 0;
DC1B0 = 0;
if ((VAL & 1)==1) DC1B0=1;
if ((VAL & 2)==2) DC1B1=1;
////////////////////////////////////////////////////
//8 bit 
//CCPR1L = VAL;
TMR2ON=1;
}

void main() 
{
ANSEL = ANSELH =0;
TRISC2=0;  // output
PR2=249;   // chu ky pwm
CCP1CON=0b00001100;///su dung tinh nang pwm muc thap ||0x0C->0b00001100: muc cao
TMR2ON=1;

tx_init();
rx_init();
load_pwm_10bit(500);
while(1)
{
	if(flag == 1)
		{
			ccp_input = strtol(chuoi_ccp,&ptr1,10);
			Width_Pulse(ccp_input);
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
	SPBRGH =0x01;
	SPBRG = 0x04;	
	BRGH=1;
	BRG16= 1;
	
	SYNC=0;
	SPEN=1;
	
	TX9=0;
	
	TXEN=1;
}
void rx_init(){

	SPBRGH =0x01;  // calculate -->f=10khz & n=260 <-> 
	SPBRG = 0x04;	
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

void Width_Pulse(int ccp_input)
{
		load_pwm_10bit(ccp_input);
		sprintf(chuoi,"CCPR1L:%d\n",CCPR1L);
		sendstring(chuoi);
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
