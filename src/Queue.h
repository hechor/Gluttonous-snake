#ifndef _QUEUE_H_
#define _QUEUE_H_
#include "Define.h"

typedef struct position {
	unsigned char x;
	unsigned char y;
}T;

typedef struct queue {
	T Data[SNAKE_MAX_SIZE+1];
	unsigned int first;
	unsigned int rear;
	unsigned int qsize;
}Queue;

void QueueInit(Queue* q);	//初始化队列
void Push(Queue* q,const T t);	//入队
T Pop(Queue* q);	//出队

#endif