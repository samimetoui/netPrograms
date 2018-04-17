/************************************************************************************************/
/* Program: echoserver.c                                                                        */
/* Description: echo server program that receive string from client and resend it to the client */
/************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "sysexits.h"

#ifdef _WIN32
#include <winsock.h>

#elif defined __unix__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#define MAXBUF 1024
#define NPORT 7
#define BKLOG 10

int main (int argc, char* argv[])
{
    int sfd,sfd2,rlen;
    struct sockaddr_in saddr;
    struct sockaddr_in caddr;
    int saddrlen;
    char tbuf[MAXBUF];

    #ifdef WIN32
    int err;
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(1,1);
    err=WSAStartup(wVersionRequested, &wsaData);
    if(err!=0)
    {
        printf("\nErreur lors du chargement de winsock.dll.");
        return 0;
    }
    #endif


        //create socket
    if ((sfd=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0)
    {
        perror("socket");
        #ifdef WIN32
        WSACleanup();
		closesocket(sfd);
        #endif
        close(sfd);
        exit(EX_OSERR);
    }

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(NPORT);
    saddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sfd,(struct sockaddr*)(&saddr),sizeof(saddr)) < 0)
    {
        perror("bind");
        #ifdef WIN32
        WSACleanup();
		closesocket(sfd);
        #endif
        close(sfd);
        exit(EX_OSERR);
    }

    if (listen(sfd,BKLOG) < 0)
    {
        perror("listen");
        #ifdef WIN32
        WSACleanup();
		closesocket(sfd);
        #endif
        close(sfd);
        exit(EX_OSERR);
    }
    saddrlen=sizeof saddr;
    while (1)
    {
        if((sfd2=accept(sfd,(struct sockaddr*)&caddr,&saddrlen)) < 0)
        {
            perror("accept");
            #ifdef WIN32
			closesocket(sfd);
			#endif
			close(sfd);
        }
        printf("connexion\n");
        if (sfd2>0) //!=INVALID_SOCKET
        {
            do
            {
                rlen=recv(sfd2,tbuf,MAXBUF,0);
                if(rlen>0)
                {
                    tbuf[rlen]='\0';
                    printf("%s\n",tbuf);
                    send(sfd2,tbuf,rlen,0);
                } else break;

            } while(1);

        }
        #ifdef WIN32
	closesocket(sfd2);
	#endif
	close(sfd2);

    }
    #ifdef WIN32
    WSACleanup();
	closesocket(sfd);
    #endif
    close(sfd);
    return 0;
}


