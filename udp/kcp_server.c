/*************************************************************************
	> File Name: kcp_server.c
	> Author: liulianglu
	> Description: 
	> Created Time: Wed 08 Nov 2017 04:02:26 AM EST
 ************************************************************************/


#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<sys/socket.h>
#include<sys/errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/time.h>
#include<unistd.h>

#include<thread>
#include<vector>
#include<string>
#include<map>

using namespace std;

#include"./kudp/ikcp.h"

class task
{
public:
    ikcpcb      *kcp;
    string      name;
};

class taskcmp
{
public:
    bool operater () (const task& left, const task& right)
    {
        if(strcmp(left.name.c_str(), right.name.c_str()))
        {
            return true;
        }else
        {
            return false;
        }
    }
};

vector<task>     tasklist;
long kudp_clock()
{
    struct timeval timer;
    long lclock = 0;
    gettimeofday(&timer, NULL);
    if(timer.tv_sec)        //虽然可以不判断，但是很多时候会取小于1S, 此时可省去乘法。
    {
        lclock += (timer.tv_sec * 1000);
    }

    lclock += timer.tv_usec / 1000;

    return lclock;
    //if(timer.tv_usec *)
}

void kudp_msleep(unsigned long milsec)
{
    //struct timespec ts;
    //ts.tv_sec   = (time_t) (milsec / 1000);
    //ts.tv_nsec  = (long)((milsec % 1000) * 1000000);
    //milsec * (1024 - 16 -8)
    usleep((milsec << 10) - (milsec << 4) - (milsec << 3));
}
char isalive = 1;
int kudp()
{
    vector<task>::iterator it   = tasklist.begin();

    while(isalive)
    {
        while(it != tasklist.end())
        {
            ikcp_update(*it.kcp, kudp_clock());
        }
        kudp_msleep(20);

    }

    return 0;
}


int main(int argc, char *argv[])
{
    int iMsgSize    = 1024;
    char* strIP     = "192.168.66.66";
    int iPort       = 6666;

    if(argc > 1)
    {
        iMsgSize = atoi(argv[1]);
    }
    if(argc > 3)
    {
        strIP = argv[2];
        iPort = atoi(argv[3]);
    }

    char* szMsg = (char*)malloc(sizeof(char) * iMsgSize);

    struct sockaddr_in server;
    memset(&server, 0x00, sizeof(server));

    server.sin_family   = AF_INET;
    server.sin_port     = iPort;
    //server.sin_addr.s_addr = htonl(strIP);
    inet_pton(AF_INET, strIP, &server.sin_addr);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    bind(sockfd, (struct ockaddr*)&server, sizeof(server));

    //虽然是用于接收，但是必须要先初始化成这样，否则会得不到对端地址。
    struct sockaddr_in    peer_addr;
    socklen_t             peer_len = sizeof(peer_addr);

    thread t(kudp);
    
    int iTransport = 0;
    string key;
    while(1)
    {
        //memset(peer_addr, 0x00, sizeof(peer_addr));
        iTransport = recvfrom(sockfd, szMsg, iMsgSize, 0, (struct sockaddr* )&peer_addr, &peer_len);
        if(iTransport < 0)
        {
            printf("erron: %d \n", errno);
            continue;
        }else 
        {
            key = peer_addr.sin_addr.s_addr;
        }

        //printf("recieve %d from %s \n", iTransport, peer_addr.sin_addr.in_addr);
        iTransport = sendto(sockfd, szMsg, iTransport, 0, (struct sockaddr* )&peer_addr, peer_len);

        if(iTransport < 0)
        {
            printf("errno: %d \n", errno);
            continue;
        }
        printf("Send %d \n", iTransport);
    }

}
