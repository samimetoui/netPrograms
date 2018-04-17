/*****************************************************************************************************************/
/*Program: daytimeclient.c                                                                                       */
/*Description: Program that read the time and the date from given time server name on port 13 and display it.    */
/*The program must be run as root otherwise you'll have an error message on when connection is established       */
/*****************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sysexits.h"

#ifdef _WIN32
#include <winsock.h>

#elif defined __unix__
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#define USAGE "Usage: %s Adresse du serveur"
#define MAXMSG 1024
#define NPORT 13

int main (int argc, char*argv[])
{
	int n, sfd;
	char buf[MAXMSG];
	struct sockaddr_in saddr;
	struct hostent* phostEnt;

   	#ifdef _WIN32
	int err;
	WORD wVersionRequested;
	WSADATA wsaData;

	//Load the winsock.dll
	wVersionRequested = MAKEWORD(1,1);						//This section
	err=WSAStartup(wVersionRequested,&wsaData);				//must be removed to run
	if(err != 0) { printf("winsock.dll error.");}			//under none msft OSs
    #endif

	if(argc != 2)
		{
			(void) fprintf(stderr,USAGE,argv[0]);
			#ifdef _WIN32
			WSACleanup();
			#endif
			exit (EX_USAGE);
		}

	//Create socket
	if((sfd=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP))<0)
		{
			perror("socket");
			#ifdef _WIN32
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
			#ifdef _WIN32
			WSACleanup();
			#endif
			exit(EX_OSERR);
		}

	//read the date and the time from the server
	if ((n=recv(sfd,buf,MAXMSG-1,0)) < 0)
		{
			perror("receive");
			#ifdef _WIN32
			WSACleanup();
			#endif
			exit(EX_OSERR);
		}

	//mark the end of the string
	buf[n]='\0';

	//display the server date and time
	(void)printf("La date du serveur: %s est %s\n",argv[1],buf);

	//close the socket
	close(sfd);		//close the socket
	#ifdef _WIN32
	WSACleanup();	//unload winsock.dll - must be removed to run under none msft OSs
	#endif

	return 0;
}
