// A simple client 
#include "mysocket.h"  

int main(int argc, char *argv[]) {
	TSocket sock;
	char *servIP;                /* server IP */
	unsigned short servPort;     /* server port */
	char str[100];
	char cod[2];
	int i, count = 0;
	
	if (argc != 2) {
		ExitWithError("Usage: client <remote server IP>");    
	}

	servIP = argv[1];
	servPort = 2018;

	/* Create a connection */
	sock = ConnectToServer(servIP, servPort);
	
	cod[0] = '2';
	cod[1] = '\n';
	printf("%s", cod);

	if (WriteN(sock, cod, 2*sizeof(char)) <= 0)
		{ ExitWithError("WriteN() failed"); }

	if (ReadLine(sock, str, 99) < 0)
		{ExitWithError("ReadLine() failed");}

	printf("Count: %s\n", str);
	count = atoi(str);
	printf("Count: %d\n", count);

	for (i=0; i<count; i++) {
		if (ReadLine(sock, str, 99) < 0)
			{ExitWithError("ReadLine() failed");}
		else printf("%s",str);
	}
	close(sock);

	for(;;){
		
		scanf("%s", cod);
		cod[1] = '\n';

		if (cod[0] == '2'){
			sock = ConnectToServer(servIP, servPort);
			if (WriteN(sock, cod, 2*sizeof(char)) <= 0)
				{ ExitWithError("WriteN() failed"); }

			if (ReadLine(sock, str, 99) < 0)
				{ExitWithError("ReadLine() failed");}
			printf("Count: %s\n", str);
			count = atoi(str);
			printf("Count: %d\n", count);

			for (i=0; i<count; i++) {
				if (ReadLine(sock, str, 99) < 0)
					{ExitWithError("ReadLine() failed");}
				else printf("%s",str);
			}
			close(sock);
		} else if(cod[0] == '0')
			break;
	}
	printf("Sessão encerrada.\n");
	return 0;
}
