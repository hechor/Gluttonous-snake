#include <REGX52.H>
#include "Define.h"

static unsigned char KeyNum;

unsigned char GetMatrixKey()	//接收按键值
{
	unsigned char ret_val;
	ET0=0;
	ret_val=KeyNum;
	KeyNum=0;
	ET0=1;
	return ret_val;
}

unsigned char ScanKeyState()	//获取矩阵键盘状态
{
	unsigned char ret_val=0;
	
	P1=0X0F;
	if(P1!=0X0F)
	{
		switch(P1)	//获取列状态
		{
			case 0x07:ret_val=1; break;
			case 0x0B:ret_val=2; break;
			case 0x0D:ret_val=3; break;
			case 0x0E:ret_val=4; break;
		}
		P1=0xF0;
		switch(P1)	//获取行状态
		{
			case 0x70:ret_val+=0; break;
			case 0xB0:ret_val+=4; break;
			case 0xD0:ret_val+=8; break;
			case 0xE0:ret_val+=12; break;
		}
	}
	
	return ret_val;
}

void MatrixKeyLoop()	//更新按键值，每隔20ms更新一次，防止产生误判
{
	static unsigned char NowState,PreState;
	PreState = NowState;
	NowState = ScanKeyState();
	
	if(PreState==1 && NowState==0)
	{
		KeyNum=1;
	}
	if(PreState==2 && NowState==0)
	{
		KeyNum=2;
	}
	if(PreState==3 && NowState==0)
	{
		KeyNum=3;
	}
	if(PreState==4 && NowState==0)
	{
		KeyNum=4;
	}
	if(PreState==5 && NowState==0)
	{
		KeyNum=5;
	}
	if(PreState==6 && NowState==0)
	{
		KeyNum=6;
	}
	if(PreState==7 && NowState==0)
	{
		KeyNum=7;
	}
	if(PreState==8 && NowState==0)
	{
		KeyNum=8;
	}
	if(PreState==9 && NowState==0)
	{
		KeyNum=9;
	}
	if(PreState==10 && NowState==0)
	{
		KeyNum=10;
	}
	if(PreState==11 && NowState==0)
	{
		KeyNum=11;
	}
	if(PreState==12 && NowState==0)
	{
		KeyNum=12;
	}
	if(PreState==13 && NowState==0)
	{
		KeyNum=13;
	}
	if(PreState==14 && NowState==0)
	{
		KeyNum=14;
	}
	if(PreState==15 && NowState==0)
	{
		KeyNum=15;
	}
	if(PreState==16 && NowState==0)
	{
		KeyNum=16;
	}
}

void Timer0_Rountine()	interrupt 1	//矩阵键盘中断
{
	static unsigned char count;
	
	TL0=0X66;	//设置定时器初值
	TH0=0XFC;
	count++;
	if(count>=20)
	{
		count=0;
		MatrixKeyLoop();
	}
}