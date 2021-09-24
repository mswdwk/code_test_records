#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdio.h>
#include <stdlib.h>

/*
 * 普通的
 * 链式队列
 * */
typedef struct QNode
{
    int data;
    struct QNode *next;
}QNode, *QueuePtr;

typedef struct LinkQueue
{
    QueuePtr front;
    QueuePtr rear;
}LinkQueue;

void init_Queue(LinkQueue *q);//初始化队列
void push_Queue(LinkQueue *q, int e);//队尾入队
int pop_Queue(LinkQueue *q, int *e);//队头出队
int is_Empty(LinkQueue *q);
void show(LinkQueue *q);

#endif /* QUEUE_H_ */
