#include "reg52.h"    //此文件中定义了单片机的一些特殊功能寄存器
#include "intrins.h"
typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;
	
sbit STA=P3^2;  //蓝牙连接信号
sbit dianji1g=P0^1;		//电机1运动控制端口
sbit dianji1d=P0^2;	 	//电机1运动控制端口
sbit dianji2g=P0^3;		//电机2运动控制端口
sbit dianji2d=P0^4;		//电机2运动控制端口
sbit dianji1sudu=P0^0;	 //电机1速度控制端口(PWM)
sbit dianji2sudu=P0^5;	 //电机2速度控制端口(PWM)
sbit deng=P2^7;

void delay(u16 ii);	 //延时函数声明
void zhengzhuan_1();  //电机1正转
void tingzhi_1();	//电机1停止
void fanzhuan_1();		//电机1反转
void zhengzhuan_2();  //电机2正转
void tingzhi_2();	//电机2停止
void fanzhuan_2();		//电机2反转

void xiaoche_qianjin();	 //小车前进
void xiaoche_houtui();	 //小车后退
void xiaoche_zuozhuan();  //小车左转
void xioache_youzhuan();  //小车右转
void xiaoche_tingzhi();   //小车停止


#define USART_MAX_RECV_LEN	40
u8 USART_RX_BUF[USART_MAX_RECV_LEN];
u16 USART_RX_STA=0; 

void delay(u16 ii)
{
	while(ii--);；=
}

void UsartInit()
{
	SCON=0X50;			//设置为工作方式1
	TMOD=0X20;			//设置计数器工作方式2
	PCON=0X80;			//波特率加倍
	TH1=0XFa;			//计数器初始值设置，注意波特率是9600的
	TL1=0XFa;
	ES=1;						//打开接收中断
	EA=1;						//打开总中断
	TR1=1;					//打开计数器
}

void USART_SendData(u8 datbuf[])
{
	u8 i=0;
	for(i=0;i<2;i++)
	{
		SBUF=datbuf[i];//将接收到的数据放入到发送寄存器
		while(!TI);//等待发送数据完成
		TI=0;//清除发送完成标志位	
	}
}

void main()
{ u16 kk=100;
	u8 i=0,j=0;
	u16 sudu=0;
	STA=0;
	delay(100);
  P0=0x00;  //屏蔽数码管
	UsartInit();  //串口初始化
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
    
		if(USART_RX_BUF[0]==0x34)			//小车后退
		{ 
		  xiaoche_qianjin();
			if(kk!=0){
			USART_SendData("H");
			}
			kk=0;
		}
		else if(USART_RX_BUF[0]==0x33)	   //小车前进
		{ 
			xiaoche_houtui();
			if(kk!=1)
			{
			USART_SendData("Q");
			}
			kk=1;
		}
	    else if(USART_RX_BUF[0]==0x32)	   //小车右转
		{
			
			xiaoche_zuozhuan();
			if(kk!=2)
			USART_SendData("Y");
			kk=2;
		}
		else if(USART_RX_BUF[0]==0x31)	   //小车左转
		{
			xioache_youzhuan();
			if(kk!=3)
			USART_SendData("Z");
			kk=3;
		}
	    else if(USART_RX_BUF[0]==0x30)	   //小车停止
		{
			xiaoche_tingzhi();
			if(kk!=4)
			USART_SendData("T");
			kk=4;
		}


		if(USART_RX_BUF[0]==0x13)	 //彩灯灭
		{
		    deng=1;
			if(kk!=5)
			USART_SendData("M");
			kk=5;
		}
		else if(USART_RX_BUF[0]==0x12)		//彩灯亮
		{
		  deng=0;
			if(kk!=6){
			USART_SendData("L");
			}
			kk=6;
		}
//		else if(USART_RX_BUF[0]==0x11)		//小车加速
//		{
//			delay(5000);
//			if(10000>sudu)
//			{
//			sudu=sudu+2000; 
//			}
//			USART_SendData(USART_RX_BUF);
//		}
//	    if(USART_RX_BUF[0]==0x10)			//小车减速
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
	RI = 0;//清除接收中断标志位							 
}


//void diwei2gaowei(){	 //启动第二个


//}
//sbit STA=P3^2;  //蓝牙连接信号
//sbit dianji1g=P0^1;		//电机1运动控制端口
//sbit dianji1d=P0^2;	 	//电机1运动控制端口
//sbit dianji2g=P0^3;		//电机2运动控制端口
//sbit dianji2d=P0^4;		//电机2运动控制端口
//sbit dianji1sudu=P0^0;	 //电机1速度控制端口(PWM)
//sbit dianji2sudu=P0^5;	 //电机2速度控制端口(PWM)
//sbit deng=P2^7;
void zhengzhuan_1()  //电机1正转
{  		dianji1g=1;
		dianji1d=0;
		delay(5000);
}
void tingzhi_1()   //电机1停止
{ 
	   	dianji1g=1;
		dianji1d=1;
		delay(50);
}	
void fanzhuan_1()		//电机1反转
{
        dianji1g=0;
		dianji1d=1;
		delay(50);
}
void zhengzhuan_2()  //电机2正转
{   dianji2g=1;
    dianji2d=0;
	delay(50);
}
void tingzhi_2()	//电机2停止
{	dianji2g=1;
    dianji2d=1;
	delay(50);
}
void fanzhuan_2()		//电机2反转
{	 dianji2g=0;
     dianji2d=1;
	 delay(50);
}

void xiaoche_qianjin()	 //小车前进
{	 
	zhengzhuan_1();
	zhengzhuan_2();
}
void xiaoche_houtui()	 //小车后退	
{
	fanzhuan_1();
	fanzhuan_2();
}

void xiaoche_zuozhuan()  //小车左转
{
	 zhengzhuan_1();
	 fanzhuan_2();
}
void xioache_youzhuan()  //小车右转
{
     zhengzhuan_2();
	 fanzhuan_1();
}
void xiaoche_tingzhi()  //小车停止
{
   tingzhi_1();
   tingzhi_2();
}


