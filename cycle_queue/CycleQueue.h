/*************************************************************************
	> File Name: CycleQueue.h
	> Author: liulianglu
	> Description: 
	> Created Time: Sat 26 Aug 2017 12:26:11 PM EDT
 ************************************************************************/

#ifndef _CYCLEQUEUE_H
#define _CYCLEQUEUE_H

#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

typedef struct CycleQueue
{
    void*   item;
    int     itemLen;
    int     size;

    int     front;
    int     used;
    int     rear;

    pthread_mutex_t  m_mutex;
    sem_t       hFull;
    sem_t       hEmpty;

}CycleQueue;

inline int isEmpty(CycleQueue* q)
{
    return q->front == q->rear;
}

inline int  isFull(CycleQueue* q)
{
    return q->used == q->size;
}

CycleQueue* CreateCycleQueue(int size, int itemSize);
void DestroyCycleQueue(CycleQueue* q);

void * CycleQueue_push(CycleQueue* q);
int CycleQueue_pop(CycleQueue* q, void* des);

inline void* CycleQueue_at(CycleQueue* q, int current)
{
    return (unsigned char*)q->item + current * q->itemLen;

}

#endif
