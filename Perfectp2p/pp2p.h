/* 
 * Module PerfectP2P 
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <pthread.h>

//Max buffer size for messages and sources
#define BUFSIZE 1024

// wrapper for perror
void error(char *msg);

// PerfectP2P init event
pthread_t pp2pInit(int pp2psendPort, int pp2precvPort);

// Request PerfectP2P event: send
void pp2pSend (char *dest, char *msg);

// Indication PerfectP2P event: delivery
void pp2pDelivery(char *src, char *msg);
