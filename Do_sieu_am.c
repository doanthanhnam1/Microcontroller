#include<htc.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>

#include "lcd(16).h"
#define _XTAL_FREQ 20000000

__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_ON & MCLRE_OFF & CP_OFF & CPD_OFF 
 & BOREN_ON & IESO_OFF & FCMEN_OFF & LVP_OFF & DEBUG_OFF);

char rxdata;
float time , distance;
int  pulse_rise, pulse_fall ;

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
TRISC0= 0;  // TRIGER
TRISC1= 1;	// ECHO
TRISC2= 1;	// ECHO
/// THIET LAP NGO RA VA LCD
	ANSEL = ANSELH=0;
	lcd_init();
	
	////////////////////////////////////////////////////////////
    //// config timer 1 - counter
    TRISC2 = 1; //input CCP1
	TMR1CS = 0; // timer 
    T1CKPS1 = 0;  // prescale 1:1
	T1CKPS1 = 0;
    
    TMR1GE  = 0; //non-control gate

	TMR1H = 0;  //clear TMR1
	TMR1L = 0;
    
	//Khoi tao CCP1
	CCP1CON= 0b00000101;  // 0101 CCP1 th?c hi?n s? ki?n khi nh?n xung c?nh lên
	CCPR1H = CCPR1L = 0; // reset hai thanh ghi khoi CCPx


	CCP1IE=1; //Cho phép ngat CCP	
	CCP1IF=0; //Reset co ngat;	

//Khoi tao CCP2
	CCP2CON= 0b00000100; // 0100 CCP2 th?c hi?n s? ki?n khi nh?n xung c?nh xu?ng
	CCPR2H = CCPR2L = 0; // reset hai thanh ghi khoi CCPx


	CCP2IE=1; //Cho phép ngat CCP	
	CCP2IF=0; //Reset co ngat;	
	PEIE=1; //Cho phép ngat ngoai vi	
	GIE=1; //Cho phép ngat toàn cuc. 


  	////enable timter 1
    TMR1ON = 1;	

	// Khoi t?o UART
	tx_init();
	rx_init();

	while(1)
	{
		RC0=1;
		__delay_us(10);
		RC0=0;
		__delay_ms(500);
		time=(pulse_fall-pulse_rise)*((1.0*4.0)/20000000.0); //pulse_fall-pulse_rise= s? xung c?a chân ECHO di và v? // Chu kì = 4/20MHz
															// N?u thay d?i 20MHZ thì thay d?i ch? này
		distance=(time/2.0)*34480.0;    // 34480 cm/s : v?n t?c âm thanh 
		lcd_gotoxy(0,0);
		printf("d:%3.2f", distance);
		sprintf(chuoi,"%3.2f",distance);
		sendstring("\t");
		sendstring(chuoi);   

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
	if(CCP1IE && CCP1IF)
		{	
			pulse_rise= CCPR1;		
		}
		CCP1IF=0;
		if(CCP2IE && CCP2IF)
		{	
			pulse_fall = CCPR2;	
		}
		CCP2IF=0;
}

