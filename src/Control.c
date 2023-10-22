#include <REGX52.H>
#include <stdlib.h>
#include "Define.h"
#include "Queue.h"
#include "MatrixKey.h"
#include "MatrixLED.h"
#include "Timer.h"

enum { up=7, left=10, right=12, down=15 };	//�����Ӧ����ֵ
typedef struct position Position;

static unsigned char xdata Matrix[MATRIX_ROW_SIZE][MATRIX_COLUMN_SIZE];	//���󱣴���ʾͼ��
static Queue xdata Snake;	//���б�������
static Position Head;	//��ͷ����
static Position Tail;	//��β����
static Position Food;	//ʳ������
static unsigned char isOver;	//��Ϸ������־
static unsigned char Eat;	//�Ե�ʳ���־
static unsigned char DirectionState;	//����״̬
static unsigned char isUpdate;	//�������״̬
static unsigned int offset;	//����ͼ��ƫ����
static unsigned char code OverMatrix[][8]={	//����ͼ��
	{0,0,1,1,1,1,0,0},
	{0,1,0,0,0,0,1,0},
	{1,0,0,0,0,0,0,0},
	{1,0,0,0,0,1,1,0},
	{1,0,0,0,0,0,1,0},
	{0,1,0,0,0,0,1,0},
	{0,0,1,1,1,1,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,1,1,1,0,0,0},
	{0,1,0,0,0,1,0,0},
	{1,0,0,0,0,0,1,0},
	{1,0,0,0,0,0,1,0},
	{1,0,0,0,0,0,1,0},
	{0,1,0,0,0,1,1,0},
	{0,0,1,1,1,0,1,0},
	{0,0,0,0,0,0,0,0},
	{0,1,1,0,1,1,0,0},
	{1,0,0,1,0,0,1,0},
	{1,0,0,1,0,0,1,0},
	{1,0,0,1,0,0,1,0},
	{1,0,0,1,0,0,1,0},
	{1,0,0,1,0,0,1,0},
	{1,0,0,1,0,0,1,0},
	{0,0,0,0,0,0,0,0},
	{0,0,1,1,1,0,0,0},
	{0,1,0,0,0,1,0,0},
	{1,0,0,0,0,0,1,0},
	{1,1,1,1,1,1,1,0},
	{1,0,0,0,0,0,0,0},
	{0,1,0,0,0,1,0,0},
	{0,0,1,1,1,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,1,1,1,0,0,0},
	{0,1,0,0,0,1,0,0},
	{1,0,0,0,0,0,1,0},
	{1,0,0,0,0,0,1,0},
	{1,0,0,0,0,0,1,0},
	{0,1,0,0,0,1,0,0},
	{0,0,1,1,1,0,0,0},
	{0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,1,0},
	{1,0,0,0,0,0,1,0},
	{0,1,0,0,0,1,0,0},
	{0,1,0,0,0,1,0,0},
	{0,0,1,0,1,0,0,0},
	{0,0,1,0,1,0,0,0},
	{0,0,0,1,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,1,1,1,0,0,0},
	{0,1,0,0,0,1,0,0},
	{1,0,0,0,0,0,1,0},
	{1,1,1,1,1,1,1,0},
	{1,0,0,0,0,0,0,0},
	{0,1,0,0,0,1,0,0},
	{0,0,1,1,1,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,1,0,0,0,1,1,0},
	{0,1,0,0,1,0,0,0},
	{0,1,0,1,0,0,0,0},
	{0,1,1,0,0,0,0,0},
	{0,1,0,0,0,0,0,0},
	{0,1,0,0,0,0,0,0},
	{0,1,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
};

static void MatrixInit();	//��ʼ������
static void SnakeInit();	//��ʼ������
static void SetKey();	//���ð���
static void SetFood();	//����ʳ������
static void DirectionLogic();	//�жϷ���
static void FoodLogic();	//�ж��Ƿ�Ե�ʳ��
static void OverLogic();	//�ж���Ϸ�Ƿ����
static void Logic();
static void Update();	//���¾���

void Init()
{
	Timer0Init();
	Timer1Init();
	MatrixInit();
	SnakeInit();
	DirectionState=right;
	Eat=1;
	isUpdate=1;
	MatrixLEDInit();
	SetMatrixLED(Matrix);
}

void Start()
{
	if(isUpdate)
	{
		ET1=0;
		SetKey();
		SetFood();
		Logic();
		isUpdate=0;
		ET1=1;
	}
}

void MatrixInit()
{
	unsigned char i, j;
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			Matrix[i][j]=0;
		}
	}
	
	Matrix[4][3]=1;
	Matrix[4][4]=1;
}

void SnakeInit()
{
	Head.x=4;
	Head.y=4;
	Tail.x=4;
	Tail.y=3;
	QueueInit(&Snake);
	Push(&Snake, Tail);
	Push(&Snake, Head);
}

void SetKey()
{
	unsigned char NowKey;
	NowKey=GetMatrixKey();
	
	if(NowKey==up || NowKey==left || NowKey==right || NowKey==down)
	{
		if(NowKey!=DirectionState)
		{
			if(NowKey==up && DirectionState!=down)
			{
				DirectionState=NowKey;
			}
			else if(NowKey==left && DirectionState!=right)
			{
				DirectionState=NowKey;
			}
			else if(NowKey==down && DirectionState!=up)
			{
				DirectionState=NowKey;
			}
			else if(NowKey==right && DirectionState!=left)
			{
				DirectionState=NowKey;
			}
		}
	}
}

void SetFood()
{
	unsigned char i, j;
	
	if(Eat)
	{
		do
		{
			i=rand()%MATRIX_ROW_SIZE;
			j=rand()%MATRIX_COLUMN_SIZE;
		}
		while(Matrix[i][j]==1);
		
		Food.x=i;
		Food.y=j;
		Matrix[i][j]=1;
		Eat=0;
	}
}

void DirectionLogic()
{
	unsigned char Direction;
	char i;
	
	Direction=DirectionState;
	
	if(Direction==up)
	{
		Tail=Pop(&Snake);
		i=Head.x;
		i--;
		if(i<0)i=MATRIX_ROW_SIZE-1;
		Head.x=i;
		Push(&Snake, Head);
	}
	if(Direction==left)
	{
		Tail=Pop(&Snake);
		i=Head.y;
		i--;
		if(i<0)i=MATRIX_COLUMN_SIZE-1;
		Head.y=i;
		Push(&Snake, Head);
	}
	if(Direction==right)
	{
		Tail=Pop(&Snake);
		i=Head.y;
		i=(i+1)%MATRIX_COLUMN_SIZE;
		Head.y=i;
		Push(&Snake, Head);
	}
	if(Direction==down)
	{
		Tail=Pop(&Snake);
		i=Head.x;
		i=(i+1)%MATRIX_ROW_SIZE;
		Head.x=i;
		Push(&Snake, Head);
	}
}

void FoodLogic()
{
	if(Head.x==Food.x && Head.y==Food.y)
	{
		Push(&Snake, Food);
		Eat=1;
	}
}

void OverLogic()
{
	if(Matrix[Head.x][Head.y]==1 && Head.x!=Food.x && Head.y!=Food.y)
	{
		isOver=1;
	}
}

void Logic()
{
	DirectionLogic();
	OverLogic();
	FoodLogic();
}

void Update()
{
	isUpdate=1;
	Matrix[Tail.x][Tail.y]=0;
	Matrix[Head.x][Head.y]=1;
}

void Timer1_Rountine()	interrupt 3
{
	static unsigned int count1, count2, count3;
	
	TL1=0X66;	//���ö�ʱ����ֵ
	TH1=0XFC;
	count1++;
	if(count1>=3)
	{
		count1=0;
		MatrixLEDLoop();
	}
	if(!isOver)
	{
		count2++;
		if(count2>=200+SPEED*100)
		{
			count2=0;
			Update();
		}
	}
	else
	{
		count3++;
		if(count3>=500)
		{
			count3=0;
			SetMatrixLED(OverMatrix+offset);
			offset=(offset+2)%56;
		}
	}
}