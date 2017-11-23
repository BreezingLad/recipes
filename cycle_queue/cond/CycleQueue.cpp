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

#include<iostream>
using namespace std;

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
    
    //pthread_mutex_init(&q->m_mEmpty, NULL);
    //pthread_mutex_init(&q->m_mFull, NULL);
    pthread_cond_init(&q->m_full, NULL);
    pthread_cond_init(&q->m_empty, NULL);

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

        //pthread_mutex_destroy(&q->m_mEmpty);
        //pthread_mutex_destroy(&q->m_mFull);
        pthread_cond_destroy(&q->m_full);
        pthread_cond_destroy(&q->m_empty);
        free(q);
        q = NULL;
    }
}

void* CycleQueue_push(CycleQueue* q)
{
    void* current = NULL;

    pthread_mutex_lock(&q->m_mutex);
    if(isFull(q))
    {
        //cout<<"Produce wait"<<endl;
        pthread_cond_wait(&q->m_empty, &q->m_mutex);
    }

    current = (unsigned char*) q->item + q->itemLen * q->front;
    q->front = ++q->front % q->size;

    //pthread_mutex_lock(&q->m_mutex);
    q->used++;
    //pthread_mutex_unlock(&q->m_mutex);

    pthread_mutex_unlock(&q->m_mutex);

    pthread_cond_signal(&q->m_full);

    return current;
}

int CycleQueue_pop(CycleQueue* q, void* des)
{
    void* current = NULL;

    pthread_mutex_lock(&q->m_mutex);
    if(isEmpty(q))
    {
        //cout<<"consume wait"<<endl;
        pthread_cond_wait(&q->m_full, &q->m_mutex);
    }
    //current = (unsigned char*)q->item + q->rear * q->itemLen;
    current = CycleQueue_at(q, q->rear);
    if(des)
    {
        memcpy(des, current, q->itemLen);
    }
    q->rear = ++q->rear % q->size;

    //pthread_mutex_lock(&q->m_mutex);
    q->used--;
    //pthread_mutex_unlock(&q->m_mutex);

    pthread_mutex_unlock(&q->m_mutex);

    pthread_cond_signal(&q->m_empty);

    return  0;
}
