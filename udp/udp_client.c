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
    inet_pton(AF_INET, strIP, &server.sin_addr);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr*    peer_addr;

    int iSend = sendto(sockfd, szMsg, iMsgSize, 0, &server, sizeof(server));

    printf("send :%d\n", iSend);

    iSend = recvfrom(sockfd, szMsg, iMsgSize, 0, NULL, NULL);

    printf("recieve: %d\n", iSend);


}

