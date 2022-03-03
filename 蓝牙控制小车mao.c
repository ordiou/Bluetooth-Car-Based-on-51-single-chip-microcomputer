#include "reg52.h"    //���ļ��ж����˵�Ƭ����һЩ���⹦�ܼĴ���
#include "intrins.h"
typedef unsigned int u16;	  //���������ͽ�����������
typedef unsigned char u8;
	
sbit STA=P3^2;  //���������ź�
sbit dianji1g=P0^1;		//���1�˶����ƶ˿�
sbit dianji1d=P0^2;	 	//���1�˶����ƶ˿�
sbit dianji2g=P0^3;		//���2�˶����ƶ˿�
sbit dianji2d=P0^4;		//���2�˶����ƶ˿�
sbit dianji1sudu=P0^0;	 //���1�ٶȿ��ƶ˿�(PWM)
sbit dianji2sudu=P0^5;	 //���2�ٶȿ��ƶ˿�(PWM)
sbit deng=P2^7;

void delay(u16 ii);	 //��ʱ��������
void zhengzhuan_1();  //���1��ת
void tingzhi_1();	//���1ֹͣ
void fanzhuan_1();		//���1��ת
void zhengzhuan_2();  //���2��ת
void tingzhi_2();	//���2ֹͣ
void fanzhuan_2();		//���2��ת

void xiaoche_qianjin();	 //С��ǰ��
void xiaoche_houtui();	 //С������
void xiaoche_zuozhuan();  //С����ת
void xioache_youzhuan();  //С����ת
void xiaoche_tingzhi();   //С��ֹͣ


#define USART_MAX_RECV_LEN	40
u8 USART_RX_BUF[USART_MAX_RECV_LEN];
u16 USART_RX_STA=0; 

void delay(u16 ii)
{
	while(ii--);��=
}

void UsartInit()
{
	SCON=0X50;			//����Ϊ������ʽ1
	TMOD=0X20;			//���ü�����������ʽ2
	PCON=0X80;			//�����ʼӱ�
	TH1=0XFa;			//��������ʼֵ���ã�ע�Ⲩ������9600��
	TL1=0XFa;
	ES=1;						//�򿪽����ж�
	EA=1;						//�����ж�
	TR1=1;					//�򿪼�����
}

void USART_SendData(u8 datbuf[])
{
	u8 i=0;
	for(i=0;i<2;i++)
	{
		SBUF=datbuf[i];//�����յ������ݷ��뵽���ͼĴ���
		while(!TI);//�ȴ������������
		TI=0;//���������ɱ�־λ	
	}
}

void main()
{ u16 kk=100;
	u8 i=0,j=0;
	u16 sudu=0;
	STA=0;
	delay(100);
  P0=0x00;  //���������
	UsartInit();  //���ڳ�ʼ��
  dianji1sudu=1;		  ////////////////////////////////
  dianji2sudu=1;		  /////////////////////////////////
	while(1)
{       
//      dianji1sudu=1;
//      dianji2sudu=1;
//		delay(10000+sudu);
//		dianji1sudu=0;
//		dianji2sudu=0; 
//		delay(10000-sudu);  
    
		if(USART_RX_BUF[0]==0x34)			//С������
		{ 
		  xiaoche_qianjin();
			if(kk!=0){
			USART_SendData("H");
			}
			kk=0;
		}
		else if(USART_RX_BUF[0]==0x33)	   //С��ǰ��
		{ 
			xiaoche_houtui();
			if(kk!=1)
			{
			USART_SendData("Q");
			}
			kk=1;
		}
	    else if(USART_RX_BUF[0]==0x32)	   //С����ת
		{
			
			xiaoche_zuozhuan();
			if(kk!=2)
			USART_SendData("Y");
			kk=2;
		}
		else if(USART_RX_BUF[0]==0x31)	   //С����ת
		{
			xioache_youzhuan();
			if(kk!=3)
			USART_SendData("Z");
			kk=3;
		}
	    else if(USART_RX_BUF[0]==0x30)	   //С��ֹͣ
		{
			xiaoche_tingzhi();
			if(kk!=4)
			USART_SendData("T");
			kk=4;
		}


		if(USART_RX_BUF[0]==0x13)	 //�ʵ���
		{
		    deng=1;
			if(kk!=5)
			USART_SendData("M");
			kk=5;
		}
		else if(USART_RX_BUF[0]==0x12)		//�ʵ���
		{
		  deng=0;
			if(kk!=6){
			USART_SendData("L");
			}
			kk=6;
		}
//		else if(USART_RX_BUF[0]==0x11)		//С������
//		{
//			delay(5000);
//			if(10000>sudu)
//			{
//			sudu=sudu+2000; 
//			}
//			USART_SendData(USART_RX_BUF);
//		}
//	    if(USART_RX_BUF[0]==0x10)			//С������
//		{
//			delay(5000);
//			if(sudu>0)
//			{
//		    sudu=sudu-2000; 
//			}
//			USART_SendData(USART_RX_BUF);
//		}
	}		
}

void Usart() interrupt 4
{
	u8 res;
	while(!RI);
	res=SBUF;
	USART_RX_BUF[0]=res;
	RI = 0;//��������жϱ�־λ							 
}


//void diwei2gaowei(){	 //�����ڶ���


//}
//sbit STA=P3^2;  //���������ź�
//sbit dianji1g=P0^1;		//���1�˶����ƶ˿�
//sbit dianji1d=P0^2;	 	//���1�˶����ƶ˿�
//sbit dianji2g=P0^3;		//���2�˶����ƶ˿�
//sbit dianji2d=P0^4;		//���2�˶����ƶ˿�
//sbit dianji1sudu=P0^0;	 //���1�ٶȿ��ƶ˿�(PWM)
//sbit dianji2sudu=P0^5;	 //���2�ٶȿ��ƶ˿�(PWM)
//sbit deng=P2^7;
void zhengzhuan_1()  //���1��ת
{  		dianji1g=1;
		dianji1d=0;
		delay(5000);
}
void tingzhi_1()   //���1ֹͣ
{ 
	   	dianji1g=1;
		dianji1d=1;
		delay(50);
}	
void fanzhuan_1()		//���1��ת
{
        dianji1g=0;
		dianji1d=1;
		delay(50);
}
void zhengzhuan_2()  //���2��ת
{   dianji2g=1;
    dianji2d=0;
	delay(50);
}
void tingzhi_2()	//���2ֹͣ
{	dianji2g=1;
    dianji2d=1;
	delay(50);
}
void fanzhuan_2()		//���2��ת
{	 dianji2g=0;
     dianji2d=1;
	 delay(50);
}

void xiaoche_qianjin()	 //С��ǰ��
{	 
	zhengzhuan_1();
	zhengzhuan_2();
}
void xiaoche_houtui()	 //С������	
{
	fanzhuan_1();
	fanzhuan_2();
}

void xiaoche_zuozhuan()  //С����ת
{
	 zhengzhuan_1();
	 fanzhuan_2();
}
void xioache_youzhuan()  //С����ת
{
     zhengzhuan_2();
	 fanzhuan_1();
}
void xiaoche_tingzhi()  //С��ֹͣ
{
   tingzhi_1();
   tingzhi_2();
}


