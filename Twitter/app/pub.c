// A simple client 
#include "mysocket.h"  

int main(int argc, char *argv[]) {
	TSocket sock;
	char *servIP;                /* server IP */
	unsigned short servPort;     /* server port */
	char login [20], str[180], msg[140];
	int i, j, n, loginLen;
	
	if (argc != 2)
	{
		ExitWithError("<server IP>");    
	}

	servIP = argv[1];
	servPort = 4018;
	
	printf("Escreva o login do Usuário:\n");
	scanf("%s", login);
	
	str[0] = '1';
	str[1] = ' ';
	
	loginLen = strlen(login);
	
	for( i = 0; i < loginLen; i++)
		str[i+2] = login[i];
		
	i += 2;
	str[i++] = ' ';
	str[i++] = '\n';
	
	/* Create a connection */
	sock = ConnectToServer(servIP, servPort);
	
	if (WriteN(sock, str, sizeof(char) * i) <= 0) { ExitWithError("WriteN() failed"); }
	
	if (ReadLine(sock, str, 180) < 0)	{ ExitWithError("ReadLine() failed"); }

	close(sock);

	for(;;)
	{
		printf("Publicação:\n");
		fgets(msg, 140 , stdin);
		n = strlen(msg);
		msg[n] = '\n';

		if (msg[0] != 'F' || msg[1] != 'I' || msg[2] != 'M' || msg[3] != '\n')
		{
			str[0] = '4';
			str[1] = ' ';
			
			for( i = 0; i < loginLen; i++)
				str[i+2] = login[i];
				
			i += 2;
			str[i++] = ' ';
			
			for( j = 0; j < n; j++)
				str[j+i] = msg[j];
				
			j += i;
			str[j++] = ' ';
			str[j++] = '\n';
			
			sock = ConnectToServer(servIP, servPort);
			if (WriteN(sock, str, sizeof(char) * j) <= 0) { ExitWithError("WriteN() failed"); }
			
			if (ReadLine(sock, str, 180) < 0) { ExitWithError("ReadLine() failed"); }
			
			close(sock);
		} else
			break;
	}
	
	str[0] = '5';
	str[1] = ' ';
	
	for( i = 0; i < loginLen; i++)
		str[i+2] = login[i];
	
	i += 2;
	str[i++] = ' ';
	str[i++] = '\n';
	
	/* Create a connection */
	sock = ConnectToServer(servIP, servPort);
	
	if (WriteN(sock, str, sizeof(char) * i) <= 0) { ExitWithError("WriteN() failed"); }
	
	if (ReadLine(sock, str, 180) < 0) { ExitWithError("ReadLine() failed"); }
		
	close(sock);
	printf("%s", str);
	
	return 0;
}
