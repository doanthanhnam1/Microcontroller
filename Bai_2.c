#include<htc.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#define _XTAL_FREQ 20000000

// Thach anh ngoai
//#define _XTAL_FREQ 4000000

__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_ON & MCLRE_OFF & CP_OFF & CPD_OFF 
 & BOREN_ON & IESO_OFF & FCMEN_OFF & LVP_OFF & DEBUG_OFF);





// Th?ch anh n?i 4MHz 
//__CONFIG(FOSC_INTRC_NOCLKOUT & WDTE_OFF & PWRTE_ON & MCLRE_OFF & CP_OFF & CPD_OFF & BOREN_ON & IESO_OFF & FCMEN_OFF & LVP_OFF & DEBUG_OFF);
char rxdata;
unsigned int dem;
unsigned int xung;
float v;

void sendchar (char c);
void sendstring (const char* s);
void tx_init();
void rx_init();

char buffer[6];
char ptr;
char chuoi[10];

void main()
{
ANSEL=ANSELH=0;
//OSCCON = 0b01100001; // khai báo th?ch anh n?i 4MHz
TRISA4 = 1; // input
dem = 0;
xung = 0;


///////////TIMER0//////////
T0CS = 1;   // counter
T0SE = 1;

PSA = 1;   //Khong dùng b? chia prescale + Dùng thì PSA= 0;

//PS2 = 0;
//PS1 = 1;
//PS0 = 0;

//Ng?t TIMER0
T0IE = 1;   //Cho phép ng?t 
T0IF = 0;	//T?t c? ng?t
GIE = 1;	// Luôn b?ng 1 

TMR0 =5;    //Giá tr? ban d?u

tx_init();
rx_init();

while(1)
{	
    __delay_ms(5000);   // Thoi gian lay mau( neu không có thì LCD s? không k?p l?y m?u vì code ch?y quá nhanh)
    xung = dem*250 + TMR0;
    TMR0 = 5;
    dem = 0;

    v =  (xung/24)*12.0;  // delay bao nhiêu thì nhân sao cho b?ng 1 phút
	sprintf(chuoi,"%3.2f",v);
	sendstring("\t");
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
	T0IF = 0;

}




