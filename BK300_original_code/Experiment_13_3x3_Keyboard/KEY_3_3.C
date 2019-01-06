
/********************************************************************************************
* ʵ��ʮ��: 3*3����ʽ����ʵ��
* CPU�ͺţ� PIC16F877A 
* ����    4MHz 
* ˵��:     �ֱ𰴼�K1-K16ʱ����ֱܷ���ʾ����0-F
* ��    �ߣ�MCU2000
* ��    �ڣ�2012��10��19��
��������<<BK300 PICʵ���>>�ϵ���ͨ��
*********************************************************************************************/
#include <pic.h>	      //����PICͷ�ļ� �����Դ���ͷ�ļ�������< >
#include "BoardConfig.h"  //����������Ӳ����ʼ��ͷ�ļ� ����д��ͷ�ļ���" " ���PICC��������
__CONFIG(0x3F32);         //оƬ������

//����
uchar LED_CODE[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
                  0x80,0x90,0X88,0X83,0XC6,0XA1,0X86,0X8E,0XFF};
void delay(uchar x_ms);
key_scan(void);
uchar key_num;

void main(void)
{
     uchar key_val=0;

     Board_init();              //���ÿ�����Ӳ����ʼ������

//*****ʹ�ܵ�һλ�����λѡ*****************************************************
     RC4=1;
     PORTD=0X80;            //
     RC4=0;                 //
//****************************************************************************** 

     RC3=1;                 

     while(1)
    {
     TRISB=0X0F;            //
     PORTB=0X0F;            //������������     
     key_num=PORTB & 0X0F;
     if(key_num !=0x0F)
      {
         delay(1);
         if((key_num=PORTB & 0X0F)!=0X0F)
         {
         key_val=key_scan();
         key_val=key_val | key_num;
         }
         switch(key_val)
         {
            case   0xEE:    key_val=0x01; break;      
            case   0xED:    key_val=0x02; break;      
            case   0xEB:    key_val=0x03; break;      
            case   0xE7:    key_val=0x04; break;      
          
            case   0xDE:    key_val=0x05; break;
            case   0xDD:    key_val=0x06; break;
            case   0xDB:    key_val=0x07; break;
            case   0xD7:    key_val=0x08; break;    
           
            case   0xBE:    key_val=0x09; break;      
            case   0xBD:    key_val=0x0A; break;      
            case   0xBB:    key_val=0x0B; break;      
            case   0xB7:    key_val=0x0C; break;      
          
            case   0x7E:    key_val=0x0D; break;
            case   0x7D:    key_val=0x0E; break;
            case   0x7B:    key_val=0x0F; break;
            case   0x77:    key_val=0x00; break;    
         }//switch
      }   //if(key_num !=0x07)
      delay(2);
      PORTD=LED_CODE[key_val]; //��ʾ����
      delay(2); 
     }    //while(1)
} 


//������ɨ���ӳ���
key_scan()
{
   uchar key_temp=0,read_io=0xEF;          
   uchar i;
   for(i=0;i<4;i++) 
   {
        TRISB=0X00;
        PORTB=read_io |0x0F;
        TRISB=0X0F;       //������Ϊ���� 
        if((key_temp=PORTB & 0X0F)!=0X0F)
        return(read_io & 0xF0);
        read_io<<=1;
   }
}

//��ʱ�ӳ���(ʹ�ò�ͬ����ʱ��Ӧ�޸�)
void delay(uchar x_ms)
{
   uint ns;
   for(  ;x_ms>0;x_ms--)
   for(ns=0;ns<1000;ns++);
}