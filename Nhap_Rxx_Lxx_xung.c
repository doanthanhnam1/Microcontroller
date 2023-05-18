#include<htc.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#define _XTAL_FREQ 20000000
__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_ON & MCLRE_OFF & CP_OFF & CPD_OFF 
 & BOREN_ON & IESO_OFF & FCMEN_OFF & LVP_OFF & DEBUG_OFF);

char i;
long int phan_tram;
bit flag;
char  rxdata;
char *ptr1;
void Do_rong_xung(int phan_tram);

void sendchar (char c);
void sendstring (const char* s);
void tx_init();
void rx_init();
void Xu_li_chuoi(char str[4]);



char chieu;
char xung[4];
char chuoi[20];
char chuoi_ccp[4];


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
	CCPR1L= 1 ;  // set duty cycle  80%
	TMR2ON=1;
	RC4 =0; RC5 = 0;
	//////////////////
	tx_init();
	rx_init();
	i=0;
	while(1)
	{	
		if(flag == 1)
		{
			Xu_li_chuoi(chuoi_ccp);
			phan_tram = strtol(xung,&ptr1,10);
			if (chieu == 'R')
				{
					RC4 =1;RC5 =0;
					sprintf(chuoi,"DC quay: R%d\n",phan_tram);
					sendstring(chuoi);	
					Do_rong_xung(phan_tram);
				}
			else if (chieu == 'L')
				{
					RC4 =0;RC5 =1;
					sprintf(chuoi,"DC quay: L%d\n",phan_tram);	
					sendstring(chuoi);
					Do_rong_xung(phan_tram);		
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
		rxdata = RCREG;
		chuoi_ccp[i] = rxdata;	
		i++;
	    if(i == 3){
		flag =1;
		i=0;			
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


void Do_rong_xung(int phan_tram)
{
	CCPR1L= phan_tram*124*0.01;
}
void Xu_li_chuoi(char str[4])
{
	chieu=str[0];
	xung[0]= str[1];
	xung[1]= str[2];
}


