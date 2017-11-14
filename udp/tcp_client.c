/*************************************************************************
	> File Name: tcp_client.c
	> Author: liulianglu
	> Description: 
	> Created Time: Wed 15 Apr 2015 10:18:50 AM EDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<sys/socket.h>
#include<sys/errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
//#include<math.h>

#define max(a,b)    ((a) > (b)) ? (a) : (b)

int set_nonblack(int fd)
{
    int flags;
    if((flags = fcntl(fd, F_GETFL)) == -1)
    {
        return -1;
    }

    flags |= O_NONBLOCK;

    if(fcntl(fd, F_SETFL, flags) == -1)
    {
        return -1;
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

    char* szMsg = (char*)malloc(sizeof(char*) * iMsgSize);
    memset(szMsg, 0x66, iMsgSize);

    struct sockaddr_in server;
    memset(&server, 0x00, sizeof(server));

    server.sin_family   = AF_INET;
    server.sin_port     = htons(iPort);
    inet_pton(AF_INET, strIP, &server.sin_addr);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        printf("Create socket error: %d", errno);
        exit(0);
    }
    if(connect(sockfd, (struct sockaddr*) &server, sizeof(server)) < 0)
    {
        printf("Connect server error: %d", errno);
        exit(0);
    }

    

    set_nonblack(sockfd);

    int     iput = fileno(stdin);
    int     maxfd, stdineof = 0;
    fd_set  rset;
    FD_ZERO(&rset);
    

    int iTransport = 0;

    int iRead = 0, iWrite = 0, i = 0;
    while(1)
    {
        if(stdineof == 0)
            FD_SET(iput, &rset);
        FD_SET(sockfd, &rset);

        maxfd = max(iput, sockfd) + 1;
        if(select(maxfd, &rset, NULL, NULL, NULL) < 0)
        {
            
        }

        if(FD_ISSET(sockfd, &rset))
        {
            i = read(sockfd, szMsg, iMsgSize);
            if(i < 0)
            {
                continue;
            }
            iRead += i;

            printf("read: %d, total: %d, buffer size:%d\n", i, iRead, iMsgSize);
            if(iRead >= iMsgSize)
            {
                printf("recieve %d \n", iRead);
                //break;
            }
            /*
            int nRead = 0;
            iTransport = 0;

            do
            {
                nRead = read(sockfd, szMsg, iMsgSize);
                if(nRead < 0)
                {
                    if(errno == EAGAIN)
                    {
                        continue;
                    }
                }
                iTransport += nRead;
                printf("read: %d, total: %d, buffer size:%d\n", nRead, iTransport, iMsgSize);
            }while(iTransport < iMsgSize);
            //iTransport = read(sockfd, szMsg, iMsgSize);
            printf("recieve %d \n", iTransport);
            break;
            */
        }

        if(FD_ISSET(iput, &rset))
        {
            if(stdineof == 2)
            {
                continue;
            }

            i = write(sockfd, szMsg, iMsgSize - iWrite);
            if(i < 0)
            {
                continue;
            }
            iWrite += i;

            printf("write: %d, total: %d, buffer size: %d\n", i, iWrite, iMsgSize);

            if(iWrite < iMsgSize)
            {
                continue;
            }

            /*
            int nWrite = 0;
            do
            {
                
                nWrite = write(sockfd, szMsg, iMsgSize);
                iTransport += nWrite;
                printf("write: %d, total: %d, buffer size: %d\n", nWrite, iTransport, iMsgSize);


            }while(iTransport < iMsgSize);


            printf("send %d \n", iTransport);

            */

            printf("send %d \n", iWrite);
            stdineof = 2;

        }


    }

       




    return 0;




}
