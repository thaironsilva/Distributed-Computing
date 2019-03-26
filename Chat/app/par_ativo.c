// A simple client 
#include "mysocket.h"  

int main(int argc, char *argv[]) {
	TSocket sock;
	char *servIP;			/* server IP */
	unsigned short servPort;	/* server port */
	char str[100];
	char ipPort[30];
	int n, check, i;
	
	servIP = malloc(sizeof(char));

	check = 1;	
	servPort = 0;
	free(servIP);
	
	// Escreve IP		
	printf("<IP>\n");
	scanf("%s", ipPort);
	n = strlen(ipPort);
	
	printf("%d\n", n);
	servIP = malloc(n*sizeof(char));
	for(i=0;i<n;i++)
		servIP[i] = ipPort[i];
	printf("IP: %s\n", servIP);
	
	// Escreve Port		
	printf("<Port>\n");
	scanf("%hd", &servPort);		

	/* Create a connection */
	sock = ConnectToServer(servIP, servPort);
	printf("Conectado com %d\n.", sock);

	while(check){
		/* Write msg */
		scanf(" %[^\n]",str);
		n = strlen(str);
		str[n] = '\n';

		if (WriteN(sock, str, ++n) <= 0)
			{ ExitWithError("WriteN() failed"); }
	
		if (CheckStr(str)){
			check = 0;
			printf("Conexão encerrada.\n");
		}
		else{
			/* Receive the response */
			if (ReadLine(sock, str, 99) < 0)
				{ExitWithError("ReadLine() failed");} 
			else printf("%d: %s", sock, str);
			if (CheckStr(str)){
				check = 0;
				printf("Conexão encerrada.\n");
			}
		}
	}
	close(sock);
	return 0;
}
