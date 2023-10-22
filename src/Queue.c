#include "Queue.h"

void QueueInit(Queue* q)
{
	q->first=SNAKE_MAX_SIZE;
	q->rear=SNAKE_MAX_SIZE;
	q->qsize=SNAKE_MAX_SIZE;
}

void Push(Queue* q,const T t)
{
	q->Data[q->rear]=t;
	q->rear=(q->rear+1)%(q->qsize);
}

T Pop(Queue* q)
{
	T retVal;
	retVal=q->Data[q->first];
	q->first=(q->first+1)%(q->qsize);
	return retVal;
}