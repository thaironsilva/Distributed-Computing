// A simple ack server 
#include "mysocket.h"  
#include <pthread.h>

pthread_t thread;

struct TArgs {
	unsigned short port;   /* socket descriptor for client */
};

/* Handle client request */
void *Listener(void* args)
{
	TSocket srvSock, sock;
	unsigned short port;
	char str[140];
	
	port = ((struct TArgs *) args) -> port;
	sock = CreateServer(port);
	
	for(;;)
	{
		printf("Aguardando Publicação...\n");
		srvSock = AcceptConnection(sock);
		printf("Conexão Estabelecida.\n");
		
		if (ReadLine(srvSock, str, 99) < 0) 
			{ ExitWithError("ReadLine() failed"); 
		} else printf("Publicação: %s", str);
		
		close(srvSock);
	}
}

int main(int argc, char *argv[])
{
	TSocket sock;
	char *servIP;
	unsigned short srvPort, port;
	char str[180], msg[20], sub[1000], cod;
	int n, i, j, init = 0;
	struct TArgs *args;

	if (argc != 3) {
		ExitWithError("Usage: <Srv IP> <Port>");    
	}
	
	servIP = argv[1];
	srvPort = 4018;
	port = atoi(argv[2]);
	
	if ((args = (struct TArgs *) malloc(sizeof(struct TArgs))) == NULL)
			{ ExitWithError("malloc() failed"); }
		args->port = port;
	
	if(pthread_create(&thread, NULL, Listener, (void *) args))
        ExitWithError("pthread_create() failed");
	
	for (;;) 
	{ // run forever
		printf("Digite opção <2> Subescrição ou <3> Cancelar Subescrição:\n");
		scanf("%s", msg);
		cod = msg[0];
		if ((cod == '2' || cod == '3') &&  msg[1] == '\0')
		{
			if (cod == '2')
			{
				str[0] = '2';
				str[1] = ' ';
				printf("Escreva o nome do Usuário.\n");
				scanf("%s", msg);
				
				n = strlen(msg);
				for( i = 0; i < n; i++)
					str[i+2] = msg[i];
					
				i += 2;
				str[i++] = ' ';
				
				n = strlen(argv[2]);
				for( j = 0; j < n; j++)
					str[j+i] = argv[2][j];
					
				j += i;
				str[j++] = ' ';
				str[j++] = '\n';
				n = j;
			}
			else if (cod == '3')
			{
				str[0] = '3';
				str[1] = ' ';
				printf("Escreva o Código do Usuário.\n");
				scanf("%s", msg);
				
				n = strlen(msg);
				for( i = 0; i < n; i++)
					str[i+2] = msg[i];
					
				i += 2;
				str[i++] = ' ';
				str[i++] = '\n';
				n = i;
			}
			
			sock = ConnectToServer(servIP, srvPort);
			if (WriteN(sock, str, sizeof(char) * n) <= 0) { ExitWithError("WriteN() failed"); }
			if (ReadLine(sock, str, 180) < 0) { ExitWithError("ReadLine() failed"); }
			if (cod == '2')
			{
				i = 0;
				while (str[i] != ' ')
					i++;
				i++;
				
				while (str[i] != '\n')
					sub[init++] = str[i++];
				sub[init++] = '\n';
			}
			
			i = 0;
			while (str[i] != '\n')
				printf("%c", str[i++]);
			printf("\n");
		
			close(sock);
		} 
		else if (str[0] != 'F' || str[1] != 'I' || str[2] != 'M' || str[3] != '\0')
		{	
			break;
		}
		else
			printf("Escreva uma opção válida.\n");
		
	}
	printf("Preparande cancelamentos. Init = %d\n", init);
	str[0] = '3';
	str[1] = ' ';
	i = 0;
	while ( i < init)
	{	
		j = 0;
		while (sub[i] != '\n')
		{
			str[j + 2] = sub[i];
			j++;
			i++;
		}
		j += 2;
		str[j++] = ' ';
		str[j++] = '\n';
		// Debug
		int k = 0;
		while (str[k] != '\n')
		{
			printf("%c", str[k]);
			k++;
		}
		printf("\n");
		//
		sock = ConnectToServer(servIP, srvPort);
		if (WriteN(sock, str, sizeof(char) * n) <= 0) { ExitWithError("WriteN() failed"); }
		if (ReadLine(sock, str, 180) < 0) { ExitWithError("ReadLine() failed"); }
		
		i++;	
	}
}
