#include <LPC21xx.H>
#include"lcd_header.h"

#define sw_left 14
#define sw_right 15
#define sw_wiper 16


typedef struct CAN2

{
    unsigned char id;
   
    unsigned char rtr;

    unsigned char dlc;

    unsigned char byteA;

    unsigned char byteB;

}CAN2_MSG;

void can2_init(void){

    PINSEL1|=0x00014000;     //P0.23-->TD2 & P0.24-->RD2

    VPBDIV=1;                //PCLK=60MHz


    C2MOD=0x1;               //CAN1 into Reset  Mode

    AFMR=0x2;                //accept all receiving messages(data/remote)

    C2BTR=0x001C001D;        //B125Kbps @ PLCK=60MHz

    C2MOD=0x0;               //CAN1 into Normal Mode

}


void can2_tx(CAN2_MSG m1)
{

  C2TID1=m1.id;

    C2TFI1=(m1.dlc<<16);   

    if(m1.rtr==0)            //if data frame

    {

        C2TFI1&=~(1<<30);     //RTR=0

        C2TDA1=m1.byteA;     //lower 4bytes of data

        C2TDB1=m1.byteB;     //upper 4bytes of data

    }

    else{

        C2TFI1|=(1<<30);     //RTR=1

    }

    C2CMR=(1<<0)|(1<<5);      //Start Xmission & select Tx Buf1

    while((C2GSR&(1<<3))==0);  //wait for data Xmission.

}



main()
{   
   
    CAN2_MSG m1;
    lcd_init();
    can2_init();
   
             
  //  m1.id=1;

    m1.rtr=0;

    m1.dlc=1;
   
    m1.byteA=0xAA;
    m1.byteB=0;
   
    lcd_string("BODY CONTROL MODULE");
       // delay_ms(500);
       // lcd_command(0x01);
    while(1)
        {
                       
        if(((IOPIN0>>sw_left)&1)==0)
        {
		m1.id=0x01;
        can2_tx(m1);
        lcd_command(0x01);
        lcd_string("left indicator");
		delay_ms(500);
        }
        else if(((IOPIN0>>sw_right)&1)==0)
        {
		m1.id=0x02;
        can2_tx(m1);
        lcd_command(0x01);
        lcd_string("right indicator");
		delay_ms(500);
        }
        else if(((IOPIN0>>sw_wiper)&1)==0)
        {m1.id=0x03;
        can2_tx(m1);
        lcd_command(0x01);
        lcd_string("wiper");
		delay_ms(500);

        }

        }

}