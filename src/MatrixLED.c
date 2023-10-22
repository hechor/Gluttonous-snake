#include <REGX52.H>
#include "Define.h"

sbit _RCLK=P3^5;	//���Ŷ���
sbit _SRCLK=P3^6;
sbit _SER=P3^4;
static unsigned char (*Matrix)[MATRIX_COLUMN_SIZE];

static void _SendByte(unsigned char Byte);	//������λ�Ĵ���

void MatrixLEDInit()
{
	_SRCLK=0;
	_RCLK=0;
}

void SetMatrixLED(unsigned char **m)	//������ʾͼ��
{
	Matrix=m;
}

void _SendByte(unsigned char Byte)
{
	unsigned char i;
	
	for(i=0;i<8;i++)
	{
		_SER=Byte&(0x80>>i);
		_SRCLK=1;
		_SRCLK=0;
	}
	
	_RCLK=1;
	_RCLK=0;
}

void MatrixLEDLoop()	
{
	unsigned char row=0x00;
	static unsigned char i;
	unsigned char j;
	
	P0=0xFF;
	for(j=0;j<8;j++)	//�������j-1��ת��Ϊ����
	{
		if(Matrix[j][i]) row|=(0x80>>j);
	}
	_SendByte(row);
	
	P0=~(0X80>>i);
	i=(i+1)%MATRIX_COLUMN_SIZE;
}
