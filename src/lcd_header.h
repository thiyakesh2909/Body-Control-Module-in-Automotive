//lcd header:-
#include<lpc21xx.h>

#define lcd_d 0x0f<<20
#define rs  1<<17
#define rw  1<<18
#define e  1<<19

void delay_ms(unsigned int x)
{
T0PR=60000-1;
T0TCR=0X01;
while(T0TC<x);
T0TCR=0X03;
T0TCR=0X00;
}

void lcd_command(unsigned char cmd)
{
IOCLR1=lcd_d;
IOSET1=((cmd&0xf0)<<16);
IOCLR1=rs;
IOCLR1=rw;
IOSET1=e;
delay_ms(2);
IOCLR1=e;

IOCLR1=lcd_d;
IOSET1=((cmd&0x0f)<<20);
IOCLR1=rs;
IOCLR1=rw;
IOSET1=e;
delay_ms(2);
IOCLR1=e;
}

void lcd_data(unsigned char d)
{
IOCLR1=lcd_d;
IOSET1=((d&0xf0)<<16);
IOSET1=rs;
IOCLR1=rw;
IOSET1=e;
delay_ms(2);
IOCLR1=e;

IOCLR1=lcd_d;
IOSET1=((d&0x0f)<<20);
IOSET1=rs;
IOCLR1=rw;
IOSET1=e;
delay_ms(2);
IOCLR1=e;
}

void lcd_init(void)
{
IODIR1|=lcd_d|rs|rw|e;
lcd_command(0x01);
lcd_command(0x02);
lcd_command(0x0c);
lcd_command(0x28);
lcd_command(0x80);
}
void lcd_string(unsigned char*s)
{
while(*s)
{
lcd_data(*s);
*s++;
}
}