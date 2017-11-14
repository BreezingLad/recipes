/*************************************************************************
	> File Name: udp_client.c
	> Author: liulianglu
	> Description: 
	> Created Time: Thu 19 Oct 2017 09:25:15 PM EDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<sys/socket.h>
#include<sys/errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>


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

    int iTransport = 0;
    while(1)
    {
        //memset(peer_addr, 0x00, sizeof(peer_addr));
        iTransport = recvfrom(sockfd, szMsg, iMsgSize, 0, (struct sockaddr* )&peer_addr, &peer_len);
        if(iTransport < 0)
        {
            printf("erron: %d \n", errno);
            continue;
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

