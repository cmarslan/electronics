// LCD module connections
sbit LCD_D7 at RB2_bit;
sbit LCD_D6 at RB3_bit;
sbit LCD_D5 at RB4_bit;
sbit LCD_D4 at RB5_bit;
sbit LCD_EN at RB6_bit;
sbit LCD_RS at RB7_bit;

sbit LCD_D7_Direction at TRISB2_bit;
sbit LCD_D6_Direction at TRISB3_bit;
sbit LCD_D5_Direction at TRISB4_bit;
sbit LCD_D4_Direction at TRISB5_bit;
sbit LCD_EN_Direction at TRISB6_bit;
sbit LCD_RS_Direction at TRISB7_bit;
// End LCD module connections
//******************************frequency meter variables*******
int value,freq,humidity;
char humid[7];
//**************************************************************
void data_converstion(void)
{
IntToStr(humidity, humid);
}
void display1(void)
{

lcd_out(1,1,"Humidity = ");
lcd_out(1,13, Ltrim(humid));
Lcd_Chr_Cp('%');
Lcd_Chr_Cp(' ');

}

void interrupt(void) // high portD
{
if(T1CON.TMR1ON==0)
{
T1CON.TMR1ON=1; // turn on the timer1
INTCON.INTF = 0; // clear the interrupt flag
}
else if(T1CON.TMR1ON==1)
{
T1CON.TMR1ON=0; // turn off the timer1
value=(TMR1H<<8)|(TMR1L);
INTCON.INTE = 0; //Enable RB0/INT external Interrupt
freq=(5018035/value);
humidity = 565 - freq/13;
// freq = freq /13.18;
//freq = 100 - freq;
TMR1H=0;
TMR1L=0;
INTCON.INTE = 1; //Enable RB0/INT external Interrupt
INTCON.INTF = 0; // clear the interrupt flag
//freq=0;
}
}

void main()
{
long count;

count=0;
TMR1H=0;
TMR1L=0; // intialization of timer one prescalar and internal clock
INTCON.GIE = 1; //Enable Global Interrupt
INTCON.INTE = 1; //Enable RB0/INT external Interrupt
OPTION_REG.INTEDG = 0; //Interrupt on rising edge
ADC_Init();
Lcd_Init(); // Initialize LCD
freq=0;
Lcd_Cmd(_LCD_CLEAR); // Clear display
lcd_cmd(_LCD_CURSOR_OFF);
lcd_out(1,4,"GREEN HOUSE");
lcd_out(2,6,"SYSTEM");
delay_ms(1000);
Lcd_Cmd(_LCD_CLEAR); // Clear display

while(1)
{

data_converstion();
display1();
}
}