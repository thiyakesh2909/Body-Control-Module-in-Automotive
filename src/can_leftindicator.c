#include <LPC21xx.H>
#define led 1<<17

typedef struct CAN2

{
	unsigned char id;
	
	unsigned char rtr;

    unsigned char dlc;

    unsigned char byteA;

    unsigned char byteB;

}CAN2_MSG;

void delay_ms(unsigned int x)
{
T0PR=60000-1;
T0TCR=0X01;
while(T0TC<x);
T0TCR=0X03;
T0TCR=0X00;
}

void can2_init(void){

    PINSEL1|=0x00014000;     //P0.23-->TD2 & P0.24-->RD2

    VPBDIV=1;                //PCLK=60MHz


    C2MOD=0x1;               //CAN1 into Reset  Mode

    AFMR=0x2;                //accept all receiving messages(data/remote)

    C2BTR=0x001C001D;        //B125Kbps @ PLCK=60MHz

    C2MOD=0x0;               //CAN1 into Normal Mode

}
void can2_rx(CAN2_MSG *m1){

    while((C2GSR&0x1)==0);

    m1->id=C2RID;

    m1->dlc=(C2RFS>>16)&0xF;

    m1->rtr=(C2RFS>>30)&0x1;

    if(m1->rtr==0)             //if data frame

    {
        m1->byteA=C2RDA;

        m1->byteB=C2RDB;

    }

    C2CMR=(1<<2);              //free receiver buffer(imp)

}

main()
{   

    CAN2_MSG m1;
    IODIR0|=led;
    IOSET0|=led;
    can2_init();
         while(1)
        {     
        can2_rx(&m1);
        if(m1.id==1)
        {
        IOCLR0=led;
        delay_ms(500);
        IOSET0=led;
        delay_ms(500);
        }
        }
    }
