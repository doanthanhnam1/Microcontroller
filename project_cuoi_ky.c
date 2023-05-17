//IR sensors' signal pins
#define OUTERMOST_LEFT  PIN_B0
#define LEFT            PIN_B1
#define MIDDLE          PIN_B2
#define RIGHT           PIN_B3
#define OUTERMOST_RIGHT PIN_B4
//L298 motor driver connections
#define IN1            PIN_C0
#define IN2_CCP2       PIN_C1
#define IN3_CCP1       PIN_C2
#define IN4            PIN_C3
#define ENA            PIN_C4
#define ENB            PIN_C5
//LCD module connections
#define LCD_RS_PIN     PIN_E0
#define LCD_RW_PIN     PIN_E1
#define LCD_ENABLE_PIN PIN_E2
#define LCD_DATA0      PIN_D0
#define LCD_DATA1      PIN_D1
#define LCD_DATA2      PIN_D2
#define LCD_DATA3      PIN_D3
#define LCD_DATA4      PIN_D4
#define LCD_DATA5      PIN_D5
#define LCD_DATA6      PIN_D6
#define LCD_DATA7      PIN_D7
//End LCD module connections
 
#include <16F887.h>
#device ADC=10
#fuses INTRC_IO, NOMCLR, NOBROWNOUT, NOLVP
#use delay(clock = 8MHz)
#include <lcd.c>

unsigned int16 i, distance,KQADC=0,cap_do1,cap_do2;

int1 wait_sensor(){
  i = 0;
  set_timer1(0);                                 // Reset Timer1
  while(!input(PIN_B7) && (i < 1000))
    i = get_timer1();                            // Read Timer1 and store its value in i
  if(i > 990)
    return 0;
  else
    return 1;
}
int16 get_distance()
{
  i = 0;
  set_timer1(0);
  while(input(PIN_B7) && (i < 25000))
    i = get_timer1();
  return i;
 
}
VOID LCD_HIENTHI_NHIETDO()
{
   lcd_gotoxy(17,2);delay_us(10);                    // Go to column 17 row 2
   lcd_putc("C");
   lcd_gotoxy(13,2);delay_us(10);                    // Go to column 13 row 2
   printf(lcd_putc, "%3Lu", KQADC);
   
} 
VOID LCD_RETRAI()
{   
   lcd_gotoxy(25,1); DELAY_US(10);                   // Go to column 5 row 3 ( (x,y): if x>20 then lcd goes to (x-20,y+2))  
   lcd_putc("DANG RE TRAI     ");
}
VOID LCD_REPHAI()
{   
   lcd_gotoxy(25,1); DELAY_US(10);                   // Go to column 5 row 3 ( (x,y): if x>20 then lcd goes to (x-20,y+2))  
   lcd_putc("DANG RE PHAI     ");
}
VOID LCD_DIDUNG()
{   
   lcd_gotoxy(25,1); DELAY_US(10);                   // Go to column 5 row 3 ( (x,y): if x>20 then lcd goes to (x-20,y+2))  
   lcd_putc("DI DUNG ROI     ");
}
VOID LCD_DUNGLAI()
{   
   lcd_gotoxy(25,1); DELAY_US(10);                   // Go to column 5 row 3 ( (x,y): if x>20 then lcd goes to (x-20,y+2))  
   lcd_putc("DUNG LAI       ");
}
VOID DI_THANG()
{     
      OUTPUT_HIGH(ENA);
      OUTPUT_HIGH(ENB);
     
     //IN1,2: high-low, IN3,4: high-low
      OUTPUT_HIGH(IN1);
      cap_do2=600;
      SET_PWM2_DUTY(cap_do2);
      OUTPUT_HIGH(IN2_CCP2);
      
      cap_do1=400;
      SET_PWM1_DUTY(cap_do1);
      OUTPUT_HIGH(IN3_CCP1);
      OUTPUT_LOW(IN4);
    
      LCD_DIDUNG();
}

VOID XOAY_TRAI()
{     
      OUTPUT_HIGH(ENA);
      OUTPUT_HIGH(ENB);
      
      //IN1,2: low-high, IN3,4: high-low
     
      cap_do2=700;
      SET_PWM2_DUTY(cap_do1);
      OUTPUT_HIGH(IN2_CCP2);
      OUTPUT_LOW(IN1);
      
      cap_do1=700;
      SET_PWM1_DUTY(cap_do1);
      OUTPUT_HIGH(IN3_CCP1);
      OUTPUT_LOW(IN4);
      LCD_RETRAI();
}
VOID XOAY_PHAI()
{
      OUTPUT_HIGH(ENA);
      OUTPUT_HIGH(ENB);
      
      //IN1,2: high-low , IN3,4: low-high
      cap_do2=300;
      SET_PWM2_DUTY(cap_do2);
      OUTPUT_HIGH(IN1);
      OUTPUT_HIGH(IN2_CCP2);
      
      cap_do1=300;   
      SET_PWM1_DUTY(cap_do1);
      OUTPUT_HIGH(IN3_CCP1);
      OUTPUT_HIGH(IN4);
     
      LCD_REPHAI();
}

void main(){
   setup_oscillator(OSC_8MHZ);                    // Set internal oscillator to 8MHz
   output_b(0);
   set_tris_B(0xFF);SET_TRIS_A(0x03);
   OUTPUT_C(0x00); 
   OUTPUT_HIGH(ENA);
   OUTPUT_HIGH(ENB);
   SETUP_ADC(ADC_CLOCK_DIV_8);
   SETUP_ADC_PORTS(SAN0);
   SET_ADC_CHANNEL(0);
   DELAY_US(20);
   
   
  
   lcd_init();                                    // Initialize LCD module
   lcd_putc('\f');                                // Clear LCD
   lcd_gotoxy(1, 2);                              // Go to column 1 row 2
   lcd_putc("Temperature:");
   delay_ms(100);
   
   set_tris_C(0x00);
   SETUP_CCP1(CCP_PWM);                           //Initialize PWM                            
   SETUP_TIMER_2(T2_DIV_BY_16,249,1);  
   cap_do1=800;                               
   SET_PWM1_DUTY(cap_do1);                         //500/10% duty cycle => half speed 
  
   SETUP_CCP2(CCP_PWM);                           //Initialize PWM         
   cap_do2=200;
   SET_PWM2_DUTY(cap_do2);                         //500/10% duty cycle => half speed
   
  SETUP_TIMER_1(T1_INTERNAL | T1_DIV_BY_2);       // Configure Timer 1 to increment by 1 every 1 us
  lcd_gotoxy(25,2); DELAY_US(10);                 // Go to column 5 row 4 ( (x,y): if x>20 then lcd goes to (x-20,y+2))  
  lcd_putc("XE BUG");
  while(TRUE)
  {
// measuring distance and displaying it on LCD
    // Send 10us pulse to HC-SR04 Trigger pin
    output_high(PIN_C7);
    delay_us(10);
    output_low(PIN_C7);
    // Read pulse comes from HC-SR04 Echo pin
    if(wait_sensor()){
      distance = get_distance();
         if(distance < 23000)                       // Range of measurment ends at 23000*0.017145 = 394.335 cm
         {
           lcd_gotoxy(1, 1);                        // Go to column 13 row 1
           lcd_putc("Distance: ");
           
           distance = i*0.017145;                   // Calculate the distance (s=v*t/2, v = 340.29 m/s = 0.034029 cm/us)
           lcd_gotoxy(13,1);                        // Go to column 13 row 1
           lcd_putc("  cm  ");
           lcd_gotoxy(11,1);                        // Go to column 11 row 1
           printf(lcd_putc, "%3Lu", distance);
         }
         else 
         {  
           lcd_gotoxy(1, 1);                        // Go to column 1 row 1
           lcd_putc("      Out Of Range");
         }
       }
    else {
      lcd_gotoxy(1, 1);                          // Go to column 1 row 2
      lcd_putc("    Time Out     ");
    }
    
   delay_ms(50);
   
   // Measuring temperature and displaying it on LCD
   KQADC=READ_ADC();
   DELAY_MS(10);
   KQADC= KQADC /2.046;
   LCD_HIENTHI_NHIETDO();

   
 // Line tracking
 IF (distance < 8)
 {
    OUTPUT_LOW(ENA);
    OUTPUT_LOW(ENB);
    LCD_DUNGLAI();
 }
 ELSE
       { 
    //Read IR sensor signal  (1: line below, 0: no line)
       //00100: go straight
       IF ((INPUT_STATE(OUTERMOST_LEFT) == 0) && (INPUT_STATE(LEFT) == 0) && (INPUT_STATE(MIDDLE) == 1) &&  (INPUT_STATE(RIGHT) == 0) && (INPUT_STATE(OUTERMOST_LEFT) == 0))
         {
         DI_THANG();
         }
       // 2 sensors on the left sense line: turn right
       ELSE IF ((INPUT_STATE(LEFT) == 1) ||  (INPUT_STATE(OUTERMOST_LEFT) == 1)) 
         {
         XOAY_PHAI();
         }
       // 2 sensors on the right sense line: turn left
       ELSE IF ((INPUT_STATE(RIGHT) == 1) ||  (INPUT_STATE(OUTERMOST_RIGHT) == 1)) 
         {
         XOAY_TRAI();  
         }
     }
  }
}
// End of code
