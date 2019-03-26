 /* 
 * Algorithm PerfectP2P 
 */

#include "pp2p.h"
#include "fairp2p.h"

//ports to receive and send messages
int sendPort;
int recvPort;

char sendBuf[5000];
int sent[100];
int sendInit[100];
int sentMsgs;
int sendPos;

char destBuf[5000];
int destInit[100];
int destPos;

char recBuf[5000];
int recMsgs; // number of messages
int recPos; // position in buffer

/*
struct TArgs {
	char dest[20];
	char msg[100];
};
*/

// Request PerfectP2P event: send

void pp2pSend (char *dest, char *msg)
{
    int i;
	int msglen = strlen(msg);
	
	sendInit[sentMsgs] = sendPos;
	
	for (i = 0; i < msglen; i++)
		sendBuf[i + sendPos] = msg[i];
	sendPos += msglen;
	sendBuf[sendPos++] = '\n';
	
	for (i = 0; i < strlen(dest); i++)
		destBuf[i + destPos] = dest[i];
	destPos += strlen(dest);
	destBuf[destPos++] = '\n';
	
	fp2pSend(dest, msg);
	sent[sentMsgs] = 1;
	sentMsgs++;
	
	// debug
	i = 0;
	while(sendBuf[sendInit[sentMsgs-1] + i] != '\n')
	{
		printf("%c", sendBuf[sendInit[sentMsgs-1] + i]);
		i++;
	}
	printf(" -> Enviada");
	printf("%c", sendBuf[sendInit[sentMsgs-1] + i]);
	
}

void * sender()
{
	int n = 0, i = 0;
	char msg[100], dest[100];
	
	printf("Pronto para enviar.\n");
	while(1)
	{
		sleep(0.2);
		if (sentMsgs > 0)
		{
			if (n >= sentMsgs)
				n = 0;
			if(sent[n] < 5)
			{
				i = 0;
				while(sendBuf[sendInit[n] + i] != '\n')
				{
					msg[i] = sendBuf[sendInit[n] + i];
					i++;
				}
				msg[i] = '\0';
				i = 0;
				while(destBuf[destInit[n] + i] != '\n')
				{
					dest[i] = destBuf[destInit[n] + i];
					i++;
				}
				dest[i] = '\0';
				
				printf("Menssagen reeviada %d vezes: <%s> <%s>\n", sent[n], dest, msg);
				
				fp2pSend(dest, msg);
				sent[n]++;
			}
			n++;
			sleep(0.1);
		}
	}
	pthread_exit(NULL);
}

/*
void * sender(void *args)
{
	int i = 0;
	char *dest, *msg;
	
	dest = ((struct TArgs *) args)->dest;
	msg = ((struct TArgs *) args)->msg;
	
	sleep(0.4);
	while(i < 3)
	{
		fp2pSend(dest, msg);
		i++;
	}
	pthread_exit(NULL);
}

void pp2pSend (char *dest, char *msg)
{
	struct TArgs *args;
	
	if ((args = (struct TArgs *) malloc(sizeof(struct TArgs))) == NULL)
		{ error("malloc() failed"); }
	strcpy(args->dest, dest);
	strcpy(args->msg, msg);

	pthread_t tid;
    if(pthread_create(&tid, NULL, sender, args))
       error("error pthread_create()");
}
*/

// receive a message
int pp2pReceive (char *src, char *msg)
{
	int i = 0, j = 0, k = 0, flag = 0, pos = 0, msglen = strlen(msg);
	
	for(i = 0; i < recMsgs; i++)
	{
		while(recBuf[j] != '\n' && j - pos  < msglen)
		{
			if(recBuf[j] != msg[k])
				flag = 1;
			j++;
			k++;
		}		
		if (!flag && recBuf[j] == '\n' && j - pos == msglen)
			return 0;
			
		while(recBuf[j] != '\n')
			j++;
		j++;
		k = 0;
		flag = 0;
		pos = j;
	}
	
	for (i = 0; i < msglen; i++)
		recBuf[i + recPos] = msg[i];
	recPos += msglen;
	recBuf[recPos++] = '\n';
	recMsgs++;
	
	/* return  the  number  of bytes received, or -1 if an error occurred */
	return msglen * sizeof(char);
}

void fp2pDelivery (char *src, char *msg)
{
	if (pp2pReceive(src, msg) > 0)
		pp2pDelivery(src, msg);
}

// PerfectP2P init event
pthread_t pp2pInit(int pp2psendPort, int pp2precvPort)
{
	sendPos = 0;
	sentMsgs = 0;
	
	destPos = 0;
	
	recPos = 0;
	recMsgs = 0;
	
    sendPort = pp2psendPort; //port to send messages to dst nodes
    recvPort = pp2precvPort; //port to receive messages from src nodes
    
    pthread_t tid;
    if(pthread_create(&tid, NULL, sender, NULL))
       error("error pthread_create()");
 
    return fp2pInit(sendPort, recvPort);
}
