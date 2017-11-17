/*************************************************************************
	> File Name: testCycleQueue.c
	> Author: liulianglu
	> Description: 
	> Created Time: Sun 27 Aug 2017 05:15:27 PM EDT
 ************************************************************************/
#include<unistd.h>
#include<stdlib.h>

#include<iostream>
#include<vector>
using namespace std;

#include"../tools/stl_char.h"
#include"CycleQueue.h"
#include"../tools/tool_time.h"

void testCycleQueue();

int gTestTimes;

int gProducerNum;
int gConsumerNum;

CycleQueue*     gQueue;

class Student
{
public:
    Student() {}
    Student(const char* strName, int iAge, char chSex = 1):name(strName), age(iAge), sex(chSex){}
    stl_char32  name;
    int         age;
    char        sex;
};

class testThread
{
public:

    pthread_t  handle;

    int seq;
    int times;
    
    int success;
    char    statues;

    TimeStastics timer;

};

int main(int argc, char *argv[])
{
    gTestTimes = 10000;
    gProducerNum = 20;
    gConsumerNum = 20;

    int iQueueSize = 10;

    if(argc > 1)
    {
        if(argv[1][0] == '-')
        {
            cout<<"Message Num:         [10000]\n"
                <<"Produce Num:         [10]\n"
                <<"Consume Num:         [10]\n"
                <<"CycleQueue size :    [10]"<<endl;
        }else 
        {
            gTestTimes = atoi(argv[1]);
        }

        if(argc > 2)
        {
            gProducerNum = atoi(argv[2]);
        }

        if(argc > 3)
        {
            gConsumerNum = atoi(argv[3]);
        }

        if(argc > 4)
        {
            iQueueSize = atoi(argv[4]);
        }
    }
 
    gQueue = CreateCycleQueue(iQueueSize, sizeof(Student));

    testCycleQueue();
    return 0;
}






vector<testThread*>     producerThread;
vector<testThread*>     consumerThread;



void PrintStudentQuqueue(CycleQueue* q)
{
    Student* current = NULL;
    cout<<"print stuendt queue:";
    if(isFull(q))
    {
        for(int i = 0; i < q->size; i++)
        {
            current = (Student*)CycleQueue_at(q, i);
            cout<<current->name.data <<' ' <<current->sex<<' '<<current->age<<"\n";
        }
    }else 
    {
        for(int i = q->rear; i != q->front; i = ++i%q->size)
        {
            current = (Student*)CycleQueue_at(q, i);
            cout<<current->name.data << ' '<<current->sex<<' '<<current->age<<"\n";

        }   
    }
    cout<<endl;
}

void* Produce(void* args)
{
    static int seq = 0;
    int     index = seq++;

    producerThread[index]->timer.start();
    Student* producer = NULL;
    for(register int i = 0; i < gTestTimes; i++)
    {
        producer = (Student*)CycleQueue_push(gQueue);
        if(producer)
        {
            producer->name = "liulianglu";
            producer->sex   = 1;
            producer->age   = i;
            producerThread.at(index)->success++;
            //cout<<"produce"<<endl;
        }else
        {
            //cout<<"full"<<endl;
            PrintStudentQuqueue(gQueue);
        }

    }

    producerThread[index]->timer.stop();
    producerThread.at(index)->statues = 0;
    return NULL;

}

void* Consume(void* args)
{
    static int seq = 0;
    int index = seq++;

    consumerThread[index]->timer.start();
    for(register int i = 0; i < gTestTimes; i++)
    {
        Student* consumer = new Student();
        if(!CycleQueue_pop(gQueue, consumer))
        {
            consumer->name = "lll";
            consumer->sex = 0;
            consumer->age = i;
            consumerThread.at(index)->success++;
            //cout<<"consume"<<endl;

        }
        else 
        {
            //cout<<"empty"<<endl;
        }
    }
    consumerThread[index]->timer.stop();

    consumerThread.at(index)->statues = 0;
    return NULL;
}
void testCycleQueue()
{
    
    vector<testThread*>::iterator it = producerThread.begin();

    testThread* newThread = NULL;
    //int sequence = 0;
    register int i = 0;
    
    for(i = 0; i < gProducerNum; i++)
    {
        newThread = new testThread();
        newThread->statues = 1;
        newThread->seq = i;

        pthread_create(&newThread->handle, NULL, Produce, NULL);

        producerThread.push_back(newThread);
        
        //pthread_join(newThread->handle, NULL);

    }

    for( i = 0; i < gConsumerNum; i++)
    {
        newThread = new testThread();
        newThread->statues = 1;
        newThread->seq = i;
        
        pthread_create(&newThread->handle, NULL, Consume, NULL);

        consumerThread.push_back(newThread);
        //pthread_join(newThread->handle, NULL);
    
    }

    bool isAlive = true;

    do
    {
        
        it = producerThread.begin();
        isAlive = true;
        while(it != producerThread.end())
        {
            if((*it)->statues == 1)
            {
                isAlive = false;
                break;
            }
            it++;
        }
        if(isAlive)
        {
            it = consumerThread.begin();
            while(it != consumerThread.end())
            {
                if((*it)->statues == 1)
                {
                    isAlive = false;
                    break;
                }
                it++;
            }
            if(isAlive)
            {
                int totals = 0;
                for( it = producerThread.begin(); it != producerThread.end(); it++ )
                {
                    totals+=(*it)->success;
                }

                cout<<"Produce:"<<totals;
                totals = 0;
                for(it = consumerThread.begin(); it != consumerThread.end(); it++)
                {
                    totals+= (*it)->success;
                }

                cout<<"Consume:"<<totals<<"Queue used:"<<gQueue->used<<endl;
                break;
            }

            sleep(500);
        }
    }while(true);

    cout<<"performance statistics :\n";
    int iProduceTime = 0, iConsumeTime = 0;
    it = producerThread.begin();
    while(it != producerThread.end())
    {
        iProduceTime += (*it)->timer.total_us();
        cout<<"produce time: "<<(*it)->timer.total_us()<<"\n";
        it++;
    }

    it = consumerThread.begin();
    while(it != consumerThread.end())
    {
        iConsumeTime += (*it)->timer.total_us();
         cout<<"consume time: "<<(*it)->timer.total_us()<<"\n";
        it++;

    }

    cout<<"Total Speed : "<<(gProducerNum + gConsumerNum) * gTestTimes / ((iProduceTime + iConsumeTime) * 0.001)<<"笔/mS\n"
        //<<iProduceTime<<" "<<iConsumeTime
        <<"Produce Speed: "<<gProducerNum * gTestTimes  / (iProduceTime * 0.001) <<"笔/mS"
        <<"Consume Speed: "<<gConsumerNum * gTestTimes  / (iConsumeTime * 0.001) <<"笔/ms"<<endl;

    DestroyCycleQueue(gQueue);
}
