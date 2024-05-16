
// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define _XTAL_FREQ 6000000

void init(void);
void lcdcmd(unsigned char);
void lcddata(unsigned char);
void lcdoutput(unsigned int);
unsigned int lovalve,hivalve;
unsigned long valve,voltage;
unsigned int d4,d3,d2;
void main()
{
    init();
    while(1)
    {
        //ADCON0=0x81; // For one channel this line is not required if n of channel this needed
        ADCON0|=0x04;  //0000 0010
        while(ADCON0&0x04); //it will check when ADCON gets Zero
        lovalve = ADRESL; //store low value
        hivalve = ADRESH;  //store high value
        valve=((unsigned int)hivalve<<8)+(unsigned int)lovalve;//value will be stored
        voltage=(float)valve*48/1023+0.5;//calibrate the value for digital
        lcdcmd(0x80); //first line data show
        lcdoutput(voltage);//display voltage in lcd
        __delay_ms(1000);
        
        
        
        
        
        
    }
}

void init()
{
    TRISA=0x01;  //setting RA0=1 as an input
    TRISC=0x00;  //output for RS
    TRISD=0x00;  //8 data pins output
    ADCON0=0x81; // 1000 0001 => 0x81
    ADCON1=0x8E; // 1000 1110 => 0x8E
    lcdcmd(0x3F); // 0011 1111
    __delay_ms(100);
    lcdcmd(0x3F); //0011 1111
    __delay_ms(100);
    lcdcmd(0x3F); //0011 1111
    __delay_ms(100);
    lcdcmd(0x38); // N=No of lines And F= size of the font (0011 1000)
    __delay_ms(100);
    lcdcmd(0x06); //right shift 0000 0110
    __delay_ms(100);
    lcdcmd(0x0C); //0000 1100  cursor stops when lcd on
    __delay_ms(100);
    lcdcmd(0x01); //clear the previous data
    __delay_ms(100);
    
    
    
  
}
void lcdoutput(unsigned int i)
{
    
   d4=0;
   d3=0;
   
    
    
    d4=(voltage%10);//voltage=50 means 50%10 =0
    d3=(voltage/10);//voltage=50 means 50/10 =5
    
    
    
    lcdcmd(0x80);
    lcddata(0x30+d3);//sending the data to lcd at position 0x80
    
    lcdcmd(0x81);
    lcddata(0x30+d4);//sending the data to lcd at position 0x81
    
    
   
}

void lcdcmd(unsigned char i)
{
    PORTC&=0xF7;  //RS =0
    PORTD=i; // data set to portd
    PORTC|=0x01; //EN=1
    PORTC&=~0x01;  //EN=0
    __delay_ms(100);
}
void lcddata(unsigned char i)
{
    PORTC|=0x08; //RS =1
    PORTD=i;     // data set to portd
    PORTC|=0x01; //EN=1
    PORTC&=~0x01;   //EN=0
    __delay_ms(100);
}
