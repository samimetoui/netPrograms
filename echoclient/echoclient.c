/********************************************************************************/
/* Program: echoclient.c                                                        */
/* Description: echo client program that send a string to an echo server,       */
/* receive the echo from the server and display the received sting              */
/********************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "sysexits.h"

#ifdef _WIN32
#include <winsock.h>

#elif defined __unix__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#endif

#define USAGE "Usage: %s Chaine de caractères"
#define NPORT 7
#define MAXMSG 256

int main (int argc, char*argv[])
{
	int n, sfd;
	char bufs[MAXMSG];
	char bufc[MAXMSG];
	struct sockaddr_in saddr;
	struct hostent* phostEnt;
    #ifdef WIN32
	int err;
	WORD wVersionRequested;
	WSADATA wsaData;

	//Load the winsock.dll
	wVersionRequested = MAKEWORD(1,1);					//This section
	err=WSAStartup(wVersionRequested,&wsaData);			//must be removed to run
	if(err != 0) { printf("winsock.dll error.");}		//under none msft OSs
    #endif

	if(argc != 2)
		{
			(void) fprintf(stderr,USAGE,argv[0]);
			#ifdef WIN32
			WSACleanup();
			#endif
			exit (EX_USAGE);
		}

	//Create socket
	if((sfd=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP))<0)
		{
			perror("socket");
			#ifdef WIN32
			WSACleanup();
			#endif
			exit(EX_OSERR);
		}

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(NPORT);

	phostEnt = gethostbyname (argv[1]); //get host ip 32 bits string from time server name

	//convert to ip 32 bits from ip 32 bits string
	saddr.sin_addr.s_addr = ((struct in_addr*)(phostEnt->h_addr))->s_addr;

	//convert to ip 32 bits from decimal separate with dots ip adress string
    //saddr.sin_addr.s_addr = inet_addr(argv[1]);

	//connect to the time server
	if (connect(sfd,(struct sockaddr*)&saddr,sizeof saddr ) < 0)
		{
			perror("connect");
			#ifdef WIN32
			WSACleanup();
			#endif
			exit(EX_OSERR);
		}
        //copy the string to buf
    while(1)
    {

    printf("\nEnter the string to send and press enter: ");
    fgets(bufs,MAXMSG,stdin);
    n=strlen(bufs);
	bufs[n]='\0';

    if ((n=send(sfd,bufs,n,0))<0)
        {
            perror("send");
            #ifdef WIN32
            WSACleanup();
            #endif
            exit(EX_OSERR);
        }
	printf("sent: %s\n",bufs);
	//read the date and the time from the server
	if ((n=recv(sfd,bufc,MAXMSG-1,0)) < 0)
		{
			perror("receive");
			#ifdef WIN32
			WSACleanup();
			#endif
			exit(EX_OSERR);
		}
    bufc[n]='\0';
	printf("Echo : %s",bufc);

    }
	//close the socket
	closesocket(sfd);		//close the socket
	#ifdef WIN32
	WSACleanup();	//unload winsock.dll - must be removed to run under none msft OSs
    #endif
	return 0;

}
