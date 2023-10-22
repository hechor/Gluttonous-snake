#include <REGX52.H>

void Timer0Init()
{
	TMOD&=0XF0;
	TMOD|=0X01;
	TL0=0XFF;
	TH0=0XFF;
	TR0=1;
	TF0=0;
	EA=1;
	ET0=1;
	PT0=1;	//设置高优先级
}

void Timer1Init()
{
	TMOD&=0X0F;
	TMOD|=0X10;
	TL1=0XFF;
	TH1=0XFF;
	TR1=1;
	TF1=0;
	EA=1;
	ET1=1;
	PT1=0;	//设置低优先级
}