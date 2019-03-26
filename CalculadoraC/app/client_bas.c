// A simple client 
#include "mysocket.h"  

int main(int argc, char *argv[]) {
	TSocket sock;
	char *servIP;                /* server IP */
	unsigned short servPort;     /* server port */
	char sign;
	double n1[1], n2[1];
	int quit = 0;

	if (argc != 3) {
		ExitWithError("Usage: client <remote server IP> <remote server Port>");    
	}

	servIP = argv[1];
	servPort = atoi(argv[2]);

	/* Create a connection */
	sock = ConnectToServer(servIP, servPort);
	
	while(!quit){
		/* Write msg */
		scanf("%lf %c %lf", &(*n1), &sign, &(*n2));
		if(sign == '0'){
			printf("Encerra Cliente.\n");
			quit = 1;
		}
		
		WriteDouble(sock, n1);
		WriteN(sock, &sign, 1);
		WriteDouble(sock, n2);

		/* Receive the response */
		ReadDouble(sock, n1);
		printf("%.1lf\n", *n1);
   }
	close(sock);
	return 0;
}
