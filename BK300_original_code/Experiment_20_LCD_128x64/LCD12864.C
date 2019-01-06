
/********************************************************************************************
��    �ƣ�LCD12864_ST7920���в��Գ���
���鹦�ܣ����Ի���ST7920����оƬ��LCD12864����ʾ
          ���з�ʽ����(�ó���ֻ֧��ST7920����оƬ��LCD12864)

ע����� ����ʵ��ʱ��ʹ�ÿ��������͵�USB��Ϊ�����幩������5Vֱ����Դ

* CPU�ͺţ� PIC16F877A 
* ����    4MHz 
* ˵��:    
* ��    �ߣ�MCU2000
* ��    �ڣ�2012��10��19��
��������<<BK300 PICʵ���>>�ϵ���ͨ��
*********************************************************************************************/

#include <pic.h>	      //����PICͷ�ļ� �����Դ���ͷ�ļ�������< >
#include "BoardConfig.h"  //����������Ӳ����ʼ��ͷ�ļ� ����д��ͷ�ļ���" " ���PICC��������
__CONFIG(0x3F32);         //оƬ������

/*****LCD�ӿڶ���*****/
#define  LCD_DATA_PORT    PORTD        //����LCD��D0-D7���ڶ˿�
#define  LCD_DATA_POUT    TRISD=0x00   //����LCD���ݶ�

#define  LCD_Control_OUT  TRISC&=0XC7

#define LCD_RS_1  RC3=1
#define LCD_RS_0  RC3=0

#define LCD_RW_1  RC4=1
#define LCD_RW_0  RC4=0

#define LCD_EN_1  RC5=1
#define LCD_EN_0  RC5=0

/*****LCD���ܳ�ʼ��ָ��*****/
#define CLEAR_SCREEN 0x01     //����ָ�������ACֵΪ00H
#define AC_INIT      0x02     //��AC����Ϊ00H�����α��Ƶ�ԭ��λ��
#define CURSE_ADD    0x06     //�趨�α��Ƶ�����ͼ�������ƶ�����Ĭ���α����ƣ�ͼ�����岻����
#define FUN_MODE     0x30     //����ģʽ��8λ����ָ�
#define DISPLAY_ON   0x0c     //��ʾ��,��ʾ�α꣬���α�λ�÷���
#define DISPLAY_OFF  0x08     //��ʾ��
#define CURSE_DIR    0x14     //�α������ƶ�:AC=AC+1
#define SET_CG_AC    0x40     //����AC����ΧΪ��00H~3FH
#define SET_DD_AC    0x80



/*****���ֵ�ַ��*****/
uchar  addr_tab[]={                     //���ڸ��ݺ������������ַ
0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,//��һ�к���λ��
0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,//�ڶ��к���λ��
0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,//�����к���λ��
0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,//�����к���λ��
};

/*****n��ms����ʱ�ӳ���*****/
void delayms(uint t)     //Լ��ʱn��ms��
{
     uint i;
     while(t--)
     {
     for(i=0;i<125;i++);
     }        
}

/*****�ȴ�LCD����*****/
void WaitBusy(void)      
{
     uchar i=5;
     while(i--);
}

/*****дָ�����*****/
void Lcd_WriteCmd(uchar cmdcode)
{
     LCD_RS_0;
     LCD_RW_0;
     LCD_EN_1;
     WaitBusy();
     LCD_DATA_PORT = cmdcode;
     LCD_EN_0;
}

/*****д����*****/
void Lcd_WriteData(uchar dispdata)
{
     LCD_RS_1;
     LCD_RW_0;
     LCD_EN_1;
     WaitBusy();
     LCD_DATA_PORT = dispdata;
     LCD_EN_0;
}

/*****��ʼ��LCD*****/
void Lcd_Init(void)
{ 
     delayms(50);
     Lcd_WriteCmd(0x30);                  //ѡ�����ָ�
     delayms(1);
     Lcd_WriteCmd(0x30);                  //ѡ��8bit������
     delayms(1);
     Lcd_WriteCmd(0x0c);                  //����ʾ(���αꡢ������)
     delayms(1);
     Lcd_WriteCmd(0x01);                  //�����ʾ�������趨��ַָ��Ϊ00H
     delayms(20);
}

/*****��ʾ����*****/
void hanzi_Disp(uchar x,uchar y,uchar  *s)
{          //x��yΪ��������
     Lcd_WriteCmd(addr_tab[8*x+y]);       //д��ַ
     while(*s>0)
     { 
     Lcd_WriteData(*s);                   //д����
     s++;
     delayms(100);
     }
}

void main(void)
{

     uchar company[]="  �麣���Ƶ���";
     uchar Product[]="  BK300 ������";
     uchar Contact[]="  ��ǿ�Լ۱�";

     Board_init();              //���ÿ�����Ӳ����ʼ������
  
     RA3=1;                     //�ر�LED������ܵ�����оƬ�����              
     delayms(100);
     while(1)
     {
        Lcd_Init();
        delayms(1000);
        hanzi_Disp(0,0,company);
        hanzi_Disp(1,0,Product);        
        hanzi_Disp(2,0,Contact);
        delayms(2000);
     }
}
