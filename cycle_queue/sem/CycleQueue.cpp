/*************************************************************************
	> File Name: CycleQueue.c
	> Author: liulianglu
	> Description: 
	> Created Time: Sat 26 Aug 2017 09:33:39 PM EDT
 ************************************************************************/

#include<errno.h>
#include<string.h>
#include<stdlib.h>
#include"CycleQueue.h"

CycleQueue* CreateCycleQueue(int size, int itemSize)
{
    CycleQueue* q = (CycleQueue*) malloc(sizeof(CycleQueue));
    if(!q)
    {
        return NULL;
    }

    q->item = malloc(size * itemSize);
    if(!q->item)
    {
        free(q);
        return NULL;
    }

    q->size     =   size;
    q->itemLen  =   itemSize;
    q->front    =   0;
    q->rear     =   0;
    q->used     =   0;

    pthread_mutex_init(&q->m_mutex, NULL);
    sem_init(&q->hFull, 0, 0);
    sem_init(&q->hEmpty, 0, q->size);


    return q;
}

void DestroyCycleQueue(CycleQueue* q)
{
    if(q)
    {
        if(q->item)
        {
            free(q->item);
            q->item = NULL;
        }

        pthread_mutex_destroy(&q->m_mutex);
        sem_close(&q->hFull);
        sem_close(&q->hEmpty);
        free(q);
        q = NULL;
    }
}

void* CycleQueue_push(CycleQueue* q)
{
    void* current = NULL;

    int rv = 0;
    if((rv = sem_wait(&q->hEmpty)) != 0 && errno == EINTR)
    {

    }

    pthread_mutex_lock(&q->m_mutex);

    current = (unsigned char*) q->item + q->itemLen * q->front;
    q->front = ++q->front % q->size;
    q->used++;
    pthread_mutex_unlock(&q->m_mutex);

    sem_post(&q->hFull);

    return current;
}

int CycleQueue_pop(CycleQueue* q, void* des)
{
    void* current = NULL;

    int rv = 0;
    if((rv = sem_wait(&q->hFull))!= 0 && errno == EINTR)
    {
        
    }
    pthread_mutex_lock(&q->m_mutex);
    //current = (unsigned char*)q->item + q->rear * q->itemLen;
    current = CycleQueue_at(q, q->rear);
    if(des)
    {
        memcpy(des, current, q->itemLen);
    }
    q->rear = ++q->rear % q->size;
    q->used--;
    pthread_mutex_unlock(&q->m_mutex);

    sem_post(&q->hEmpty);
    return  0;
}
