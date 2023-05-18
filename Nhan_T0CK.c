/*#include<htc.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>

#include "lcd(16).h"

//#define _XTAL_FREQ 20000000

// Th?ch anh n?i 
#define _XTAL_FREQ 4000000
__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_ON & MCLRE_OFF & CP_OFF & CPD_OFF 
 & BOREN_ON & IESO_OFF & FCMEN_OFF & LVP_OFF & DEBUG_OFF);


// Th?ch anh n?i 4MHz 
__CONFIG(FOSC_INTRC_NOCLKOUT & WDTE_OFF & PWRTE_ON & MCLRE_OFF & CP_OFF & CPD_OFF & BOREN_ON & IESO_OFF & FCMEN_OFF & LVP_OFF & DEBUG_OFF);

char rxdata;
unsigned int dem;

void sendchar (char c);
void sendstring (const char* s);
void tx_init();
void rx_init();

char buffer[6];
char ptr;
char chuoi[10];

void main()
{
lcd_init();
ANSEL=ANSELH=0;

OSCCON = 0b01100001; // khai báo th?ch anh n?i 4MHz
TRISA4 = 1; // input

T0CS = 1;   // counter
T0SE = 1;

PSA = 1;   // 1 t?t b? chia  0: b?t b? chia

//PS2 = 0;
//PS1 = 1;
//PS0 = 0;

T0IE = 1;
T0IF = 0;
GIE = 1;

TMR0 =255;
dem = 0;

tx_init();
rx_init();

while(1)
{	
	lcd_gotoxy(0,0);
	printf("So lan nhan : %d",dem); 
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

//interrupt cua CCP1 & uart
	
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

	if(T0IF && T0IE)
	{
		dem++;
	}
 	TMR0 = 255;
	T0IF = 0;

}


void putch(char c)
	{lcd_putc(c);}
*/

//////////////////////DÙNG TIMER 1////////////////////////////////////////
#include<htc.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include "lcd(16).h"

#define _XTAL_FREQ 20000000

// Th?ch anh n?i 
//#define _XTAL_FREQ 4000000
__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_ON & MCLRE_OFF & CP_OFF & CPD_OFF 
 & BOREN_ON & IESO_OFF & FCMEN_OFF & LVP_OFF & DEBUG_OFF);


// Th?ch anh n?i 4MHz 
//__CONFIG(FOSC_INTRC_NOCLKOUT & WDTE_OFF & PWRTE_ON & MCLRE_OFF & CP_OFF & CPD_OFF & BOREN_ON & IESO_OFF & FCMEN_OFF & LVP_OFF & DEBUG_OFF);

char rxdata;
unsigned int dem;

void sendchar (char c);
void sendstring (const char* s);
void tx_init();
void rx_init();
char buffer[6];
char ptr;
char chuoi[10];




void main()
{
lcd_init();
ANSEL=ANSELH=0;

//OSCCON = 0b01100001; // khai báo th?ch anh n?i 4MHz
TRISC0 = 1; // input

TMR1CS = 1;   // Timer 1 counter
T1CKPS1 = 0;  // prescale 1:1
T1CKPS1 = 0;
T1SYNC = 1;
T1OSCEN= 1;
T1GINV = 1;
TMR1GE  = 0; //non-control gate


TMR1IF=0;
TMR1IE=1;
PEIE=1;
GIE=1;

TMR1=65535;
dem = 0;

TMR1ON = 1;

tx_init();
rx_init();

while(1)
{	
	
	lcd_gotoxy(0,0);
	printf("So lan nhan : %d",dem); 
	sprintf(chuoi,"%d\t",dem);
	sendstring(chuoi);
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

//interrupt cua CCP1 & uart

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

	if(TMR1IF &&TMR1IE)
	{
		dem++;
	}
 	TMR1= 65535;
	TMR1IF = 0;

}
	
void putch(char c)
	{lcd_putc(c);}
