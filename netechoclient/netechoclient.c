/*************************************************************************************************************************/
/* Program: netechoclient.c                                                                                              */
/*Description: echo client program that send a string to an echo server, receive the echo from the server and display it */
/*************************************************************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sysexits.h"
#include "../netsocket/netsocket.h"

#define USAGE "Usage: %s Chaine de caractères"
#define NPORT 7
#define MAXMSG 256

int main (int argc, char*argv[])
{
	int n, sfd;
	char bufs[MAXMSG];
	char bufc[MAXMSG];

    initNet();

	if(argc != 2)
		{
			(void) fprintf(stderr,USAGE,argv[0]);
		}

    if((sfd=connectClient(argv[1],NPORT))<0)
		{
			exit(EX_OSERR);
		}
    while(1)
    {

    printf("\nEnter the string to send and press enter: ");
    fgets(bufs,MAXMSG,stdin);
    n=strlen(bufs);
	bufs[n]='\0';
	

    if (sendData(sfd,bufs)<0)
        {
            exit(EX_OSERR);
        }
	printf("sent: %s\n",bufs);
	
	//read the date and the time from the server
	if ((n=receiveData(sfd,bufc,MAXMSG)) < 0)
		{
			exit(EX_OSERR);
		}

    printf("Echo : %s",bufc);

    }
	disconnect(sfd);
	closeNet();
	return 0;

}
