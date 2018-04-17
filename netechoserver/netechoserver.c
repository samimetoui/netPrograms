/************************************************************************************************/
/* Program: netechoserver.c                                                                     */
/* Description: echo server program that receive string from client and resend it to the client */
/************************************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sysexits.h"
#include "../netsocket/netsocket.h"

#define MAXBUF 1024
#define NPORT 7
#define BKLOG 10

int main (int argc, char* argv[])
{
    int sfd,sfd2,rlen;
    char cl[MAXBUF];
    char tbuf[MAXBUF];

initNet();

//create socket
if ((sfd=socketServ(NPORT))<0) return 0;

    while (1)
    {
        sfd2=acceptServ(sfd, cl);

        if (sfd2>0)
        {
			printf("Connection with host ip = %s \n",cl);
		
            do
            {
                rlen=receiveData(sfd2, tbuf, MAXBUF);
                if(rlen>0) /* try if(rlen>=0)*/
                {
                    printf("%s\n",tbuf);
                    sendData(sfd2, tbuf);
                } else break;

            } while(1);

        }
        	disconnect(sfd2);
			printf("Disconnected from the host\n");
			fflush(stdout);
    }
	disconnect(sfd);
	closeNet();

    return 0;
}
