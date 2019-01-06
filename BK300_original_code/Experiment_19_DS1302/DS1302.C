/********************************************************
ʵ��ʮ��:   DS1302ʱ��оƬʵ��
˵��:       ��Ϊ���ͺſ�����ֻ����λ����ܣ�
                Ϊ�˷��㿴��ʵ��Ч��ֻ��ʾ������
ʵ������:   ���������ʾ������ 
          
CPU�ͺ�:    PIC16F877A
ʱ��:       4MHZ

* ��    �ߣ�MCU2000
* ��    �ڣ�2012��10��19��
��������<<BK300 PICʵ���>>�ϵ���ͨ��
 ********************************************************/

#include <pic.h>	      //����PICͷ�ļ� �����Դ���ͷ�ļ�������< >
#include "BoardConfig.h"  //����������Ӳ����ʼ��ͷ�ļ� ����д��ͷ�ļ���" " ���PICC��������
__CONFIG(0x3F32);         //оƬ������

//����DS1302�Ķ˿ڶ���
#define ds1302_rst  RC2                      //����1302��RST����PC4
#define ds1302_io   RC1                      //����1302��IO����PC3
#define ds1302_sclk RC0                      //����1302��ʱ�ӽ���PC2
#define set_ds1302_rst_ddr() TRISC2=0        //��λ����Ϊ��� 
#define set_ds1302_rst() ds1302_rst=1        //��λ����1
#define clr_ds1302_rst() ds1302_rst=0        //��λ����0
#define set_ds1302_io_ddr() TRISC1=0         //���ݶ���Ϊ���
#define set_ds1302_io() ds1302_io=1          //���ݶ���1
#define clr_ds1302_io() ds1302_io=0          //���ݶ���0

#define clr_ds1302_io_ddr() TRISC1=1         //���ݶ���Ϊ����
#define in_ds1302_io() PORTC&0X02            //���ݶ���������
#define set_ds1302_sclk_ddr() TRISC0=0       //ʱ�Ӷ���Ϊ���
#define set_ds1302_sclk() ds1302_sclk=1      //ʱ�Ӷ���1
#define clr_ds1302_sclk() ds1302_sclk=0      //ʱ�Ӷ���0


#define ds1302_sec_add 0x80                  //�����ݵ�ַ
#define ds1302_min_add 0x82                  //�����ݵ�ַ
#define ds1302_hr_add 0x84                   //ʱ���ݵ�ַ
#define ds1302_date_add 0x86                 //�����ݵ�ַ
#define ds1302_month_add 0x88                //�����ݵ�ַ
#define ds1302_day_add 0x8a                  //�������ݵ�ַ
#define ds1302_year_add 0x8c                 //�����ݵ�ַ
#define ds1302_control_add 0x8e              //�������ݵ�ַ
#define ds1302_charger_add 0x90       
#define ds1302_clkburst_add 0xbe

uchar timer[8];    //ʱ������

//���������0-F��ʾ����
uchar Table[]={0xc0,0xf9,0xa4,0xb0,0x99,
               0x92,0x82,0xf8,0x80,0x90,0xff};      
//ת�������ʾ����
uchar s[4];
//����ɨ�������
uchar sel=0;
uchar temp_pa=0xFF;
/*************************************
*         DS1302����������    *
*************************************/
//д��1302���ݺ�����
//��ڣ�addΪд���ַ�룬dataΪд������
//���أ���
void ds1302_write(uchar add,uchar data) 
{ 
     uchar i=0; 
     set_ds1302_io_ddr();     //����IOΪ���
     NOP();NOP();  
     clr_ds1302_rst();        //�帴λ��ֹͣ���в���
     NOP();NOP();  
     clr_ds1302_sclk();       //��ʱ�ӣ�׼������
     NOP();NOP();  
     set_ds1302_rst();        //�ø�λ����ʼ����
     NOP();NOP();  
     for(i=8;i>0;i--)         //��ѭ��д�������
     { 
         if(add&0x01)
         set_ds1302_io();     //��ǰλΪ1��������λ
         else
         clr_ds1302_io();     //��ǰλΪ0��������λ
         NOP();NOP();  
         set_ds1302_sclk();   //����ʱ�����壬д������
         NOP();NOP();  
         clr_ds1302_sclk(); 
         NOP();NOP();  
         add>>=1;             //��λ��׼��д����1λ
      } 
     for(i=8;i>0;i--)         //��ѭ��д�������� 
     { 
         if(data&0x01)
         set_ds1302_io(); 
         else
         clr_ds1302_io(); 
         NOP();NOP(); 
         set_ds1302_sclk(); 
         NOP();NOP();  
         clr_ds1302_sclk(); 
         NOP();NOP();  
         data>>=1; 
     } 
     clr_ds1302_rst(); 
     NOP();NOP();  
     clr_ds1302_io_ddr();      //�����״̬
     NOP();NOP();  
}
//��1302�ж������ݣ�
//��ڣ�addΪ���������ڵ�ַ
//���أ�����������data
uchar ds1302_read(uchar add) 
{ 
      uchar data=0; 
      uchar i=0; 
      add+=1;                //����־ 
      set_ds1302_io_ddr();   //�˿���� 
      NOP();NOP(); 
      clr_ds1302_rst();      //�帴λ 
      NOP();NOP(); 
      clr_ds1302_sclk();     //��ʱ��
      NOP();NOP();  
      set_ds1302_rst();      //�ø�λ
      NOP();NOP(); 
      for(i=8;i>0;i--)       //��ѭ��д���ַ��
      { 
          if(add&0x01)
          {set_ds1302_io();} 
          else
          {clr_ds1302_io();}
  
          NOP();NOP(); 
          set_ds1302_sclk(); 
          NOP();NOP();  
          clr_ds1302_sclk(); 
          NOP();NOP(); 
          add>>=1; 
       } 
       clr_ds1302_io_ddr();    //�˿�����
       NOP();NOP(); 
       for(i=8;i>0;i--)        //��ѭ������1302������
       { 
          data>>=1; 
          if(in_ds1302_io())
          {data|=0x80;}
          NOP();NOP(); 
          set_ds1302_sclk(); 
          NOP();NOP(); 
          clr_ds1302_sclk(); 
          NOP();NOP(); 
       } 
       clr_ds1302_rst(); 
       NOP();NOP();
       return(data); 
} 
//���1302״̬
uchar check_ds1302(void) 
{ 
     ds1302_write(ds1302_control_add,0x80); 
     if(ds1302_read(ds1302_control_add)==0x80) 
     return 1; 
     return 0; 
} 
//��1302��д��ʱ������
void ds1302_write_time(void) 
{ 
     ds1302_write(ds1302_control_add,0x00);    //�ر�д���� 
     ds1302_write(ds1302_sec_add,0x80);        //��ͣ 
     ds1302_write(ds1302_charger_add,0xa9);    //������ 
     ds1302_write(ds1302_year_add,timer[1]);   //�� 
     ds1302_write(ds1302_month_add,timer[2]);  //�� 
     ds1302_write(ds1302_date_add,timer[3]);   //�� 
     ds1302_write(ds1302_day_add,timer[7]);    //�� 
     ds1302_write(ds1302_hr_add,timer[4]);     //ʱ 
     ds1302_write(ds1302_min_add,timer[5]);    //�� 
     ds1302_write(ds1302_sec_add,timer[6]);    //�� 
     ds1302_write(ds1302_control_add,0x80);    //��д���� 
}
//��1302�ж�����ǰʱ��
void ds1302_read_time(void) 
{ 
     timer[1]=ds1302_read(ds1302_year_add);    //�� 
     timer[2]=ds1302_read(ds1302_month_add);   //�� 
     timer[3]=ds1302_read(ds1302_date_add);    //�� 
     timer[7]=ds1302_read(ds1302_day_add);     //�� 
     timer[4]=ds1302_read(ds1302_hr_add);      //ʱ 
     timer[5]=ds1302_read(ds1302_min_add);     //�� 
     timer[6]=ds1302_read(ds1302_sec_add);     //�� 
}


//��ʱ����1
void delay_us(uchar i)
{
     for(;i;i--);    
}
   
//��ʱ����2
void delay(uint i)  
{
     uchar j;
     for(;i;i--)
     for(j=220;j;j--);    
}
	

void timer0_init(void)
{
     OPTION=0x07;     //TMR0----256��Ƶ
     INTCON=0XA0;     //�����жϼ�TMR0���������
     TMR0=0xE8;       //��ʱ����ֵ
}

/* ��ʱ��0�ж���ں��� */

void interrupt TMR0INT()
{
     T0IF=0;
     TMR0=0xE8;	

//****************�ر����������***********************************************
     RC4=1;
     PORTD=0X00;
     RC4=0; 
//*****************************************************************************

//****************����ܶ���****************************************************
     RC3=1;
     PORTD=s[sel];
     RC3=0;
//******************************************************************************


//****************�����λ��****************************************************
     RC4=1;                      //U3�������Ϊ�ߵ�ƽ����˵�ƽ������˶��仯

	 switch(sel)
	 {
		case 0x00: PORTD=0X10;break;     //��һλ�������ʾ
		           
	    case 0x01: PORTD=0X20;break;
		           
	    case 0x02: PORTD=0X40;break;
		           
		case 0x03: PORTD=0X80;break;           
	 }
     RC4=0;                      //U3�������Ϊ�͵�ƽ����˲���
     if(++sel>3)sel=0;   
//******************************************************************************       
}
//��ʾ����ת������
void Process(void)//(uint i)
{
     s[3]=Table[(timer[5]&0xF0)>>4];
     s[2]=Table[(timer[5]&0x0F)]&0x7F;  //��ʼ����ʾ���ݣ�����ʱ��֮���С����
     s[1]=Table[(timer[6]&0xF0)>>4];
     s[0]=Table[(timer[6]&0x0F)];
}
   
//������
void main(void)
{
     uint n,m=0;
     
     ADCON1=0X07;       //����A��Ϊ��ͨ���ֿ�

     Board_init();              //���ÿ�����Ӳ����ʼ������

     timer[1]=0x09;   //��
     timer[2]=0x08;   //��
     timer[3]=0x20;   //��
     timer[4]=0x12;   //ʱ
     timer[5]=0x12;   //��
     timer[6]=0x00;   //��
     timer[7]=0x04;   //��
     ds1302_write_time();    //д���ʼʱ��
   
     timer0_init();          //�趨��ʱ��0

     while(1)
     {
     ds1302_read_time();     //������ǰʱ��
     Process();              //��ʾ����ת��
     delay(100);             //ÿ100MS��һ��
	 }
}
