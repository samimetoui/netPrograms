/*****************************************************************************************************************/
/*Program: daytimeserver.c                                                                                       */
/*Description: Date time server program that send date and time to client when it receive a request on port13    */
/*****************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sysexits.h"
#include <time.h>

#ifdef _WIN32
#include <winsock.h>

#elif defined __unix__
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif
#define NPORT 13
#define BKLOG 10

int main()
{
    int n,sfd,sfd2;
    struct sockaddr_in saddr;
    struct sockaddr_in caddr;

    #ifdef _WIN32
    int err;
    WORD wVersionRequested;
    WSADATA wsaData;
    #endif
    time_t sec;
    char tbuf[25];

    int adlen=sizeof(saddr);
    #ifdef _WIN32
    wVersionRequested = MAKEWORD(1,1);
    err=WSAStartup(wVersionRequested,&wsaData);

    if(err != 0) printf("winsock.dll error.");
    #endif

	//create socket
    if ((sfd=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0)
    {
        perror("socket");
        #ifdef _WIN32
        WSACleanup();
        #endif
        close(sfd);
        exit(EX_OSERR);
    }

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(NPORT);
    saddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sfd,(struct sockaddr*)(&saddr),sizeof(struct sockaddr_in)) < 0)
    {
        perror("bind");
        #ifdef _WIN32
        WSACleanup();
        #endif
        close(sfd);
        exit(EX_OSERR);
    }

    if (listen(sfd,BKLOG) < 0)
    {
        perror("listen");
        #ifdef _WIN32
        WSACleanup();
        #endif
        close(sfd);
        exit(EX_OSERR);
    }

    while (1)
    {
        if((sfd2=accept(sfd,(struct sockaddr*)&caddr,&adlen)) < 0)
        {
            perror("accept");
            #ifdef _WIN32
            WSACleanup();
            #endif
            close(sfd);
            exit(EX_OSERR);
        }
        sec=time(NULL);
        n=strlen(ctime(&sec));
        strcpy(tbuf,ctime(&sec));
        tbuf[n-1]='\0';
        send(sfd2,tbuf,n,0);
        close(sfd2);
    }
    #ifdef _WIN32
    WSACleanup();
    #endif
    close(sfd);
    return 0;
}
